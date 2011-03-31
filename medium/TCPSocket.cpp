/*
 * TCPSocket.cpp
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#include "TCPSocket.h"
#include "TCPSocketAddress.h"

TCPSocket::TCPSocket(int fd)
:PosixSocket(fd){

}

TCPSocket::~TCPSocket() {
}

PosixSocketAddress *TCPSocket::createAddressInstance(const sockaddr *ret, size_t addr_len) const{
	return new TCPSocketAddress(ret,addr_len);
}

PosixSocket * TCPSocket::createInstance(int fd) const {
	return new TCPSocket(fd);
}
