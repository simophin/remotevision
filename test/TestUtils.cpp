#include "utils/SharedPtr.hpp"
#include "Thread.h"
#include "Log.h"

#include <malloc.h>
#include <stdlib.h>
#include <string.h>

using namespace Utils;

class MyThread: public Thread {
public:
	virtual void entry () {
		SharedPtr<char> buf = buffer;
		sleep(time);
	}

	MyThread(SharedPtr<char> buf, int t)
	:buffer(buf), time(t) {}

private:
	SharedPtr<char> buffer;
	int time;
};

void freeFunction (char *buf) {
	Log::logDebug("Free buffer !");
	::free(buf);
}

char * cloneFunction (const char *buf) {
	char *ret = ::malloc(1024);
	::memcpy(ret,buf,1024);
	Log::logDebug("Clone buffer!");
	return ret;
}

int main () {
	MyThread *thread1, *thread2;
	{
		SharedPtr<char> buf ( (char *)::malloc(1024), &freeFunction, &cloneFunction);
		thread1 = new MyThread(buf, 1000);
		thread2= new MyThread (buf,2000);
	}
	thread1->run();
	thread2->run();

	thread1->wait();
	thread2->wait();

	delete thread1;
	delete thread2;

	return 0;
}
