#ifndef __SHARED_PTR_HPP_
#define __SHARED_PTR_HPP_

#include "Log.h"
#include <assert.h>

namespace Utils {



template <typename T>
void DefaultDeleter (T * d) {
	delete d;
}

template <typename T>
T * DefaultCloner (const T *p) {
	return new T(*p);
}


template <class T>
class SharedPtr {
public:
	typedef void (*deleter_function) (T *);
	typedef T* (*clone_function) (const T *);


private :
	class impl {
	public:
		T * mPtr;
		deleter_function mDeleter;
		clone_function mCloner;
		int mRefCount;

	public:
		impl()
		:mPtr(0),mDeleter(0),mRefCount(0) {

		}

		impl (const impl &rhs)
		:mPtr(0),
		 mDeleter(rhs.mDeleter),
		 mCloner(rhs.mCloner),
		 mRefCount(1) {
			if (rhs.mPtr) mPtr = mCloner(rhs.mPtr);
		}

		~impl () {
			//Log::logDebug("An error instance has been deleted");
		}
	} *d;

public:

	SharedPtr (T * ptr = 0,
			deleter_function deleter = DefaultDeleter<T>,
			clone_function cloner = DefaultCloner<T>)
	:d(new impl){
		d->mDeleter = deleter;
		d->mPtr = ptr;
		d->mCloner = cloner;
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

	T * get() {
		detach(true);
		return d->mPtr;
	}

	T * operator-> () const{
		assert (d->mPtr != 0);
		return d->mPtr;
	}

	T * operator->() {
		detach(true);
		assert (d->mPtr != 0);
		return d->mPtr;
	}

	void reset(T * t, deleter_function del = DefaultDeleter<T>) {
		detach(false);
		d->mPtr = t;
		d->mDeleter = del;
	}

protected:
	void detach(bool copy = false) {
		impl *pd = d;

		if (copy) {
			if (pd->mRefCount == 1) return;
			d = new impl(*pd);
		}else{
			d = new impl();
		}

		pd->mRefCount--;
		if ( pd->mRefCount == 0 ) {
			if (pd->mPtr) {
				pd->mDeleter(pd->mPtr);
				pd->mPtr = 0;
			}
			delete pd;
		}
	}



};

}
#endif
