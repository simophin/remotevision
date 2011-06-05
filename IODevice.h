/*
 * IODevice.h
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#ifndef IODEVICE_H_
#define IODEVICE_H_

#include <sys/types.h>
#include <string>

#include "Error.h"

class IODevice {
public:
	typedef enum {
		POLL_READ,
		POLL_WRITE,
		POLL_ERROR,
	} PollType;

	ssize_t read(char *, size_t);
	ssize_t write(const char *,size_t);
	void flush();
	void close();
	int poll(PollType, int timeout);
	Error getLastError();

protected:
	virtual ssize_t doRead (char *, size_t) = 0;
	virtual ssize_t doWrite (const char *, size_t) = 0;
	virtual int doPoll(PollType, int);
	virtual void doClose();
	virtual Error doGetLastError();
	virtual void doFlush();
};

#endif /* IODEVICE_H_ */
