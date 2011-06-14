#ifndef __SHARED_PTR_HPP_
#define __SHARED_PTR_HPP_

#include "Log.h"
#include <assert.h>

namespace Utils {

template <class T>
void DefaultDeleter (T *p) {
	delete p;
}

template <class X> class SharedPtr
{
public:
    typedef X element_type;

    typedef void (*DeleteFunction) (X *);

    explicit SharedPtr(X* p = 0, DeleteFunction f = DefaultDeleter<X>) // allocate a new counter
        : itsCounter(0) {
    		if (p) {
    			itsCounter = new counter(p);
    			itsCounter->deleter = f;
    		}
        }
    ~SharedPtr()
        {release();}
    SharedPtr(const SharedPtr& r)
        {acquire(r.itsCounter);}
    SharedPtr& operator=(const SharedPtr& r)
    {
        if (this != &r) {
            release();
            acquire(r.itsCounter);
        }
        return *this;
    }

    X& operator*()  const    {return *itsCounter->ptr;}
    X* operator->() const    {return itsCounter->ptr;}
    X* get()        const    {return itsCounter ? itsCounter->ptr : 0;}
    bool unique()   const
        {return (itsCounter ? itsCounter->count == 1 : true);}
    void reset(X *p, DeleteFunction f = DefaultDeleter<X>) {
    	release();
    	itsCounter = new counter;
    	itsCounter->count = 1;
    	itsCounter->deleter = f;
    	itsCounter->ptr = p;
    }

private:

    struct counter {
        counter(X* p = 0, unsigned c = 1) : ptr(p), count(c) {}
        X*          ptr;
        unsigned    count;
        DeleteFunction deleter;
    }* itsCounter;

    void acquire(counter* c)
    { // increment the count
        itsCounter = c;
        if (c) ++c->count;
    }

    void release()
    { // decrement the count, delete if it is 0
        if (itsCounter) {
            if (--itsCounter->count == 0) {
                itsCounter->deleter(itsCounter->ptr);
                delete itsCounter;
            }
            itsCounter = 0;
        }
    }
};

}
#endif
