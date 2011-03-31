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
createAddressInstance(const sockaddr *addr, size_t addr_len) const{
	return new TCPSocketAddress(addr,addr_len);
}
PosixSocket * TCPServerSocket::
createInstance(int fd) const {
	return new TCPServerSocket(fd);
}
