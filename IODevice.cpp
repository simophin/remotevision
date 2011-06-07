/*
 * IODevice.cpp
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#include "IODevice.h"

Error IODevice::
read(char *data, size_t size, size_t *read_size) {
	return doRead(data,size, read_size);
}
Error IODevice::
write(const char *data, size_t size, size_t *read_size) {
	return doWrite(data,size, read_size);
}

void IODevice::close() {
	return doClose();
}

void IODevice::doClose(){

}

Error IODevice::
poll(IODevice::PollType type, int timeout) {
	return doPoll(type,timeout);
}

Error IODevice::doPoll(PollType, int) {
	return Error();
}

void IODevice::flush()
{
	doFlush();
}

void IODevice::doFlush()
{
}


