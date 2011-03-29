/*
 * Socket.cpp
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#include "Socket.h"
#include "SocketAddress.h"

Socket::Socket() {

}

Socket::~Socket() {
}

const SocketAddress * Socket::
getAddress() const {
	return doGetAddress();
}

ssize_t Socket::
read(unsigned char *data, size_t size) {
	return doRead(data,size);
}
ssize_t Socket::
write(const unsigned char *data, size_t size) {
	return doWrite(data,size);
}

void Socket::close() {
	return doClose();
}

void Socket::doClose(){

}
