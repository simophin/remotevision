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
	PosixSocket *ret = 0;

	int fd = ::accept(getFileDescriptor(),addr,(socklen_t *)&addr_len);
	if (fd < 0) {
		goto error_out;
	}

	{
		ret = createInstance(fd);
		if (accepted_addr) {
			*accepted_addr = ret->createAddressInstance(addr,addr_len);
		}
	}

	error_out:
	::free(addr);
	return ret;
}

int PosixServerSocket::
doBind(const SocketAddress *bind_addr) {
	const sockaddr *addr = 0;
	size_t addr_len = 0;
	const PosixSocketAddress *paddr = static_cast<const PosixSocketAddress *>(bind_addr);
	addr = paddr->getPosixAddress(&addr_len);
	assert(addr != 0 && addr_len > 0);
	int rc =  ::bind(getFileDescriptor(),addr,addr_len);
	return rc;
}

int PosixServerSocket::
doListen(int b) {
	return ::listen(getFileDescriptor(), b);
}
