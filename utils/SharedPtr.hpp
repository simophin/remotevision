#ifndef __SHARED_PTR_HPP_
#define __SHARED_PTR_HPP_

#include "Log.h"
#include <assert.h>

namespace Utils {

template <class T>
class SharedPtr {
public:
	typedef void (* Deleter) (T *);

	SharedPtr (T * ptr = 0, Deleter deleter = 0)
	:d(new struct impl){
		d->mDeleter = deleter;
		d->mPtr = ptr;
		d->mRefCount = ptr ? 1 : 0;
	}

	~SharedPtr () {
		detach();
	}

	SharedPtr(const SharedPtr &rhs)
	:d(rhs.d){
		d->mRefCount ++;
	}

	SharedPtr & operator = (const SharedPtr &rhs) {
		this->detach();
		d = rhs.d;
		d->mRefCount ++;
		return *this;
	}

	const T * get() const {
		return d->mPtr;
	}


	T * operator -> () {
		assert (d->mPtr != 0);
		return d->mPtr;
	}

protected:
	void detach() {
		d->mRefCount--;
		if ( d->mRefCount == 0 ) {
			if (d->mPtr) {
				if (d->mDeleter) d->mDeleter(d->mPtr);
				else delete d->mPtr;
				d->mPtr = 0;
				delete d;
				d = 0;
			}
		}

	}

private :
	struct impl {
		T * mPtr;
		Deleter mDeleter;
		int mRefCount;
	} *d;

};

}
#endif
