/*
 * TCPServerSocket.cpp
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#include "TCPServerSocket.h"
#include "TCPSocketAddress.h"
#include "TCPSocket.h"

TCPServerSocket::TCPServerSocket(int fd)
:PosixServerSocket(fd){

}

TCPServerSocket::~TCPServerSocket() {
}
PosixSocketAddress *TCPServerSocket::
doCreateAddress(const sockaddr *addr, size_t addr_len) {
	return new TCPSocketAddress(addr,addr_len);
}
PosixSocket * TCPServerSocket::
doCreateInstance(int fd) const {
	return new TCPServerSocket(fd);
}
