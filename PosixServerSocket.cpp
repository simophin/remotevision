/*
 * PosixServerSocket.cpp
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#include "PosixServerSocket.h"
#include "PosixSocketAddress.h"
#include <sys/socket.h>
#include <assert.h>
#include <stdlib.h>

PosixServerSocket::PosixServerSocket(int fd)
:PosixSocket(fd){
}

PosixServerSocket::~PosixServerSocket() {
}


SocketAddress *PosixServerSocket::
doAccept(){
	//::accept(getFileDescriptor(),);
}
const SocketAddress * PosixServerSocket::
doGetAddress() const {

}

int PosixServerSocket::
doBind(const SocketAddress *bind_addr) {
	sockaddr *addr = 0;
	size_t addr_len = 0;
	const PosixSocketAddress *paddr = static_cast<const PosixSocketAddress *>(bind_addr);
	paddr->getPosixAddress(&addr, &addr_len);
	assert(addr != 0 && addr_len > 0);
	int rc =  ::bind(getFileDescriptor(),addr,addr_len);
	::free(addr);
	return rc;
}
