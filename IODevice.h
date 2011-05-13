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

class IODevice {
public:
	typedef enum {
		POLL_READ,
		POLL_WRITE,
		POLL_ERROR,
	} PollType;

	ssize_t read(char *, size_t);
	ssize_t write(const char *,size_t);
	void close();
	int poll(PollType, int timeout);
	std::string getLastError();

protected:
	virtual ssize_t doRead (char *, size_t) = 0;
	virtual ssize_t doWrite (const char *, size_t) = 0;
	virtual int doPoll(PollType, int);
	virtual void doClose();
	virtual std::string doGetLastError();
};

#endif /* IODEVICE_H_ */
