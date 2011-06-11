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
		POLL_READ = 1,
		POLL_WRITE = 2,
		POLL_ERROR = 4
	} PollType;

	Error read(char *, size_t, size_t * read_size = 0);
	Error write(const char *,size_t, size_t * write_size = 0);
	void flush();
	void close();
	Error poll(int, int timeout);

protected:
	virtual Error doRead (char *, size_t, size_t *) = 0;
	virtual Error doWrite (const char *, size_t, size_t *) = 0;
	virtual Error doPoll(int, int);
	virtual void doClose();
	virtual void doFlush();
};

#endif /* IODEVICE_H_ */
