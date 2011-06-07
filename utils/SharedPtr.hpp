#ifndef __SHARED_PTR_HPP_
#define __SHARED_PTR_HPP_

#include "Log.h"

template <class T>
class SharedPtr {
public:
	typedef void (* Deleter) (T *);

	SharedPtr (T * ptr = 0, Deleter deleter = 0)
	:d(new struct impl){
		d->mDeleter = deleter;
		d->mPtr = ptr;
		d->mRefCount = ptr ? 1 : 0;
		static int id = 0;
		d->mId = id ++;
		init();
	}

	~SharedPtr () {
		detach();
		Log::logDebug("Shared ptr %d detached and ref count remains %d",d->mId, d->mRefCount);
	}

	SharedPtr(const SharedPtr &rhs)
	:d(rhs.d){
		d->mRefCount ++;
		init();
	}

	SharedPtr & operator = (const SharedPtr &rhs) {
		this->detach();
		d = rhs.d;
		return *this;
	}

	void init () {

		Log::logDebug("Shared ptr %d is created with ref count = %d", d->mId, d->mRefCount);
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
		int mId;
	} *d;

};


#endif
