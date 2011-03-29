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
#include "config.h"

PosixServerSocket::PosixServerSocket(int fd)
:PosixSocket(fd){
}

PosixServerSocket::~PosixServerSocket() {
}


Socket *PosixServerSocket::
doAccept(SocketAddress **accepted_addr){
	sockaddr *addr = (sockaddr *)::malloc(MAX_ADDRESS_LENGTH);
	assert(addr != 0);
	size_t addr_len = 0;
	Socket *ret = 0;

	int fd = ::accept(getFileDescriptor(),addr,&addr_len);
	if (fd < 0) {
		goto error_out;
	}

	{
		ret = new PosixSocket(fd);
		if (accepted_addr) {
			*accepted_addr = new PosixSocketAddress(addr,addr_len);
		}
	}

	error_out:
	::free(addr);
	return ret;
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

int PosixServerSocket::
doListen(int b) {
	return ::listen(getFileDescriptor(), b);
}
