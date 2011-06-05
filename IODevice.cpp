/*
 * IODevice.cpp
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#include "IODevice.h"
#include "Error.h"

ssize_t IODevice::
read(char *data, size_t size) {
	return doRead(data,size);
}
ssize_t IODevice::
write(const char *data, size_t size) {
	return doWrite(data,size);
}

void IODevice::close() {
	return doClose();
}

void IODevice::doClose(){

}

int IODevice::
poll(IODevice::PollType type, int timeout) {
	return doPoll(type,timeout);
}

int IODevice::doPoll(PollType, int) {
	return 0;
}

Error IODevice::getLastError() {
	return doGetLastError();
}

void IODevice::flush()
{
	doFlush();
}

Error IODevice::doGetLastError() {
	return Error();
}

void IODevice::doFlush()
{
}


