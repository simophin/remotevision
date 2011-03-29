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

class ThreadImpl {
	friend class Thread;
	typedef enum  {
		StatusRunning,
		StatusSleeping,
		StatusStopped,
	} RunningStatus;

	volatile bool shouldStop;
	pthread_t thread;
	pthread_cond_t threadCond;
	pthread_mutex_t threadMutex;
	RunningStatus threadStatus;

	ThreadImpl()
	:shouldStop(false){
		pthread_cond_init(&threadCond,NULL);
		pthread_mutex_init(&threadMutex,NULL);
	}

	static void *entry(void *);
};

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
	t->d->threadStatus = StatusRunning;
	pthread_cond_broadcast(&t->d->threadCond);
	return NULL;
}

void Thread::run() {
	assert(d->threadStatus == ThreadImpl::StatusRunning);
	int rc = pthread_create (&d->thread, NULL, &ThreadImpl::entry, this);
	assert(rc == 0);
	d->threadStatus =  ThreadImpl::StatusRunning;
}
bool Thread::wait(int ms){
	int rc;

	if (ms > 0) {
		struct timeval tv;
		::gettimeofday(&tv,NULL);

		struct timespec to;

		to.tv_sec = ms / 1000 + tv.tv_sec + (ms % 1000 + tv.tv_usec / 1000) / 1000;
		to.tv_nsec    = (ms % 1000 * 1000 + tv.tv_usec) % 1000000 * 1000;

		pthread_mutex_lock(&d->threadMutex);
		rc = pthread_cond_timedwait(&d->threadCond, &d->threadMutex,&to);
		if (rc != 0) return false;
	}

	rc = pthread_join(d->thread,NULL);
	assert(rc == 0);
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
	::usleep(ms*1000);
}
