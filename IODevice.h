/*
 * IODevice.h
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#ifndef IODEVICE_H_
#define IODEVICE_H_

#include <sys/types.h>
#include "RString.h"
#include "Error.h"

#include "utils/NonCopyable.hpp"

class IODevice : public Utils::NonCopyable {
public:
	typedef enum {
		POLL_READ,
		POLL_WRITE,
		POLL_ERROR
	} PollType;

	Error read(char *, size_t, size_t * read_size = 0);
	Error write(const char *,size_t, size_t * write_size = 0);
	void flush();
	void close();
	Error poll(PollType, int timeout);

protected:
	virtual Error doRead (char *, size_t, size_t *) = 0;
	virtual Error doWrite (const char *, size_t, size_t *) = 0;
	virtual Error doPoll(PollType, int);
	virtual void doClose();
	virtual void doFlush();
};

#endif /* IODEVICE_H_ */
