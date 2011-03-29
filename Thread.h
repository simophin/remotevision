/*
 * Thread.h
 *
 *  Created on: 2011-2-16
 *      Author: simophin
 */

#ifndef THREAD_H_
#define THREAD_H_

#include <memory>
#include <boost/noncopyable.hpp>

class ThreadImpl;

class Thread: public boost::noncopyable {
	friend class ThreadImpl;
public:
	Thread();
	virtual ~Thread();

	virtual void entry () = 0;
	void run();
	bool wait(int ms = -1);
	bool stop(int ms = -1);

	bool isRunning() const;
protected:
	bool shouldStop() const;
	void sleep(int ms);

private:
	std::auto_ptr<ThreadImpl> d;
};

class MutexImpl;

class Mutex: public boost::noncopyable {
	friend class MutexImpl;
public:
	enum Type {
		Normal,
		Recursive,
	};

	Mutex(Type t = Normal);
	virtual ~Mutex();

	virtual bool lock(int ms = -1);
	virtual void unlock();
	virtual bool trylock();

private:
	std::auto_ptr<ThreadImpl> d;
};


class ConditionImpl;

class Condition: public boost::noncopyable {
	friend class ConditionImpl;
public:
	Condition();
	virtual ~Condition();

	virtual bool wait(int ms = -1);
	virtual void signal();
	virtual void broadcast();

private:
	std::auto_ptr<ConditionImpl> d;
};

#endif /* THREAD_H_ */
