/*
 * IODevice.cpp
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#include "IODevice.h"

ssize_t IODevice::
read(unsigned char *data, size_t size) {
	return doRead(data,size);
}
ssize_t IODevice::
write(const unsigned char *data, size_t size) {
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

}
