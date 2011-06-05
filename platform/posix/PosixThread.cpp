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
	t->entry();
	t->d->threadStatus = StatusStopped;
	t->d->threadCond.broadcast();
	return NULL;
}

void Thread::run() {
	assert(d->threadStatus != ThreadImpl::StatusRunning);
	int rc = pthread_create (&d->thread, NULL, &ThreadImpl::entry, this);
	assert(rc == 0);
	d->threadStatus =  ThreadImpl::StatusRunning;
}
bool Thread::wait(int ms){
	int rc;


	if (ms > 0) {
		bool to;

		d->threadMutex.lock();
		if (!d->threadCond.wait(d->threadMutex,ms,&to)) {
			d->threadMutex.unlock();
			return false;
		}
		d->threadMutex.unlock();
	}

	pthread_join(d->thread,NULL);
	/*
	if (ms > 0){
		struct timespec to;
		getPthreadDelay(&to,ms);
		rc = pthread_timedjoin_np(d->thread,NULL,&to);
		if (rc == 0) return true;
		else return false;
	}else{
		pthread_join(d->thread,NULL);
		return true;
	}
	*/
	return true;
}
bool Thread::stop(int ms){
	d->shouldStop = true;
	return wait(ms);
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



bool Mutex::lock(int ms, bool *timeout)
{
	int rc;
	if (ms > 0) {
		struct timespec to;
		getPthreadDelay(&to,ms);
		rc = pthread_mutex_timedlock(&d->mInternalMutex, &to);
		if (timeout) {
			*timeout = (rc == ETIMEDOUT) ? true : false;
		}
	}else{
		rc = pthread_mutex_lock(&d->mInternalMutex);
	}

	if (rc != 0) {
		if (rc != ETIMEDOUT)
			Log::logError("%s: %s", __FUNCTION__, ::strerror(rc));
		return false;
	}
	return true;
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



bool Condition::wait(Mutex & mutex, int ms,bool *timeout)
{
	int rc;
	if (ms > 0) {
		struct timespec to;
		getPthreadDelay(&to,ms);

		rc = pthread_cond_timedwait(&d->mCond, &mutex.d->mInternalMutex, &to);
		if (timeout) {
			*timeout = (rc == ETIMEDOUT) ? true : false;
		}
	}else{
		rc = pthread_cond_wait(&d->mCond, &mutex.d->mInternalMutex);
	}

	if (rc != 0  ) {
		if (rc != ETIMEDOUT)
			Log::logError("%s: %s", __FUNCTION__, ::strerror(rc));
		return false;
	}
	return true;
}



void Condition::signal()
{
	pthread_cond_signal(&d->mCond);
}



void Condition::broadcast()
{
	pthread_cond_broadcast(&d->mCond);
}
