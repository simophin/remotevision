/*
 * IODevice.h
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#ifndef IODEVICE_H_
#define IODEVICE_H_

#include <sys/types.h>

class IODevice {
public:
	ssize_t read(unsigned char *, size_t);
	ssize_t write(const unsigned char *,size_t);
	void close();

protected:
	virtual ssize_t doRead (unsigned char *, size_t) = 0;
	virtual ssize_t doWrite (const unsigned char *, size_t) = 0;
	virtual void doClose();
};

#endif /* IODEVICE_H_ */