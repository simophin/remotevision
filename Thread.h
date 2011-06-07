/*
 * Thread.h
 *
 *  Created on: 2011-2-16
 *      Author: simophin
 */

#ifndef THREAD_H_
#define THREAD_H_

#include <memory>
#include "utils/NonCopyable.hpp"
#include "Error.h"

class ThreadImpl;

class Thread: public Utils::NonCopyable {
	friend class ThreadImpl;
public:
	Thread();
	virtual ~Thread();

	virtual Error entry () = 0;
	Error run();
	Error wait(Error *returned = 0,int ms = -1);
	Error stop(int ms = -1);

	bool isRunning() const;
protected:
	bool shouldStop() const;
	void sleep(int ms);

private:
	std::auto_ptr<ThreadImpl> d;
};

class MutexImpl;
class Condition;
class Mutex: public Utils::NonCopyable {
	friend class MutexImpl;
	friend class Condition;
public:
	enum Type {
		Normal,
		Recursive
	};

	Mutex(Type t = Normal);
	virtual ~Mutex();

	virtual Error lock(int ms = -1);
	virtual void unlock();
	virtual bool trylock();

private:
	std::auto_ptr<MutexImpl> d;
};


class ConditionImpl;

class Condition: public Utils::NonCopyable {
	friend class ConditionImpl;
public:
	Condition();
	virtual ~Condition();

	virtual Error wait(Mutex &,int ms = -1);
	virtual void signal();
	virtual void broadcast();

private:
	std::auto_ptr<ConditionImpl> d;
};





#endif /* THREAD_H_ */
