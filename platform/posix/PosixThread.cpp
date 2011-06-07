/*
 * Thread.cpp
 *
 *  Created on: 2011-2-16
 *      Author: simophin
 */

#include "Thread.h"
#include <pthread.h>
#include <assert.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>

#include "Log.h"

class ThreadImpl {
	friend class Thread;
	typedef enum  {
		StatusRunning,
		StatusSleeping,
		StatusStopped,
	} RunningStatus;

	volatile bool shouldStop;
	pthread_t thread;
	Condition threadCond;
	Mutex threadMutex;
	RunningStatus threadStatus;

	Error threadReturned;

	ThreadImpl()
	:shouldStop(false), threadStatus(StatusStopped){
	}

	static void *entry(void *);
};

static void getPthreadDelay (struct timespec *to, int ms) {
	struct timeval tv;
	::gettimeofday(&tv,NULL);


	to->tv_sec = ms / 1000 + tv.tv_sec + (ms % 1000 + tv.tv_usec / 1000) / 1000;
	to->tv_nsec    = (ms % 1000 * 1000 + tv.tv_usec) % 1000000 * 1000;
}

Thread::Thread()
:d(new ThreadImpl) {
}

Thread::~Thread(){
	if (isRunning()) {
		wait();
	}
}

void *ThreadImpl::
entry(void *arg) {
	Thread *t = static_cast<Thread *>(arg);
	t->d->threadReturned = t->entry();
	t->d->threadStatus = StatusStopped;
	t->d->threadCond.broadcast();
	return NULL;
}

Error Thread::run() {
	Error rc;

	if (d->threadStatus == ThreadImpl::StatusRunning) {
		rc.setErrorType(Error::ERR_STATE);
		return rc;
	}

	int r = pthread_create (&d->thread, NULL, &ThreadImpl::entry, this);
	if (r != 0) {
		rc.setSystemError(r);
		return rc;
	}

	d->threadStatus =  ThreadImpl::StatusRunning;
	return rc;
}


Error Thread::wait(Error *ret,int ms){
	Error rc;

	if (ret) *ret = d->threadReturned;

	if (!isRunning()) {
		return rc;
	}

	if (ms > 0) {
		d->threadMutex.lock();
		rc = d->threadCond.wait(d->threadMutex,ms);
		if ( !rc.isSuccess()) {
			d->threadMutex.unlock();
			return rc;
		}
		d->threadMutex.unlock();
	}

	int r = pthread_join(d->thread,NULL);
	if (r != 0) {
		rc.setSystemError(r);
	}
	return rc;

}


Error Thread::stop(int ms){
	d->shouldStop = true;
	return wait(NULL,ms);
}
bool Thread::shouldStop() const {
	return d->shouldStop;
}

bool Thread::isRunning() const {
	return (d->threadStatus == ThreadImpl::StatusRunning);
}

void Thread::sleep(int ms) {
	ThreadImpl::RunningStatus prevStatus = d->threadStatus;
	d->threadStatus = ThreadImpl::StatusSleeping;
	::usleep(ms*1000);
	d->threadStatus = prevStatus;
}

class MutexImpl {
public:
	pthread_mutex_t mInternalMutex;
};

Mutex::Mutex(Type t)
:d(new MutexImpl)
{
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, (t == Recursive) ? PTHREAD_MUTEX_RECURSIVE :
		PTHREAD_MUTEX_DEFAULT);
	pthread_mutex_init(&d->mInternalMutex, &attr);
	pthread_mutexattr_destroy(&attr);
}



Mutex::~Mutex()
{
	pthread_mutex_destroy(&d->mInternalMutex);
}



Error Mutex::lock(int ms)
{
	int rc;
	Error ret;
	if (ms > 0) {
#ifndef ANDROID
		struct timespec to;
		getPthreadDelay(&to,ms);
		rc = pthread_mutex_timedlock(&d->mInternalMutex, &to);
#else

		int pms = ms / 5;
		if (pms < 1) pms = 1;
		bool locked = false;
		for (int i=0; i<ms; i+= pms) {
			if (pthread_mutex_trylock(&d->mInternalMutex) == 0){
				locked = true;
				break;
			}
			usleep(pms*1000);
		}
		if (!locked) {
			ret.setErrorType(Error::ERR_TIMEOUT);
		}else return ret;
#endif

	}else{
		rc = pthread_mutex_lock(&d->mInternalMutex);
		ret.setSystemError(rc);
	}

	return ret;
}



void Mutex::unlock()
{
	pthread_mutex_unlock(&d->mInternalMutex);
}



bool Mutex::trylock()
{
	return pthread_mutex_trylock(&d->mInternalMutex);
}


class ConditionImpl {
public:
	pthread_cond_t mCond;
};

Condition::Condition()
:d(new ConditionImpl)
{
	pthread_cond_init(&d->mCond,NULL);
}



Condition::~Condition()
{
	pthread_cond_destroy(&d->mCond);
}



Error Condition::wait(Mutex & mutex, int ms)
{
	Error ret;
	int rc;
	if (ms > 0) {
		struct timespec to;
		getPthreadDelay(&to,ms);

		rc = pthread_cond_timedwait(&d->mCond, &mutex.d->mInternalMutex, &to);
	}else{
		rc = pthread_cond_wait(&d->mCond, &mutex.d->mInternalMutex);
	}

	ret.setSystemError(rc);
	return ret;
}



void Condition::signal()
{
	pthread_cond_signal(&d->mCond);
}



void Condition::broadcast()
{
	pthread_cond_broadcast(&d->mCond);
}
