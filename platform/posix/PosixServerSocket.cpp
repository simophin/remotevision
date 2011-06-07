/*
 * PosixServerSocket.cpp
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#include "PosixServerSocket.h"
#include "PosixSocketAddress.h"
#include <assert.h>
#include <stdlib.h>
#include <malloc.h>
#include "config.h"
#include "errno.h"

#include "PosixCompactHeader.h"

PosixServerSocket::PosixServerSocket(int fd)
:PosixSocket(fd){
}

PosixServerSocket::~PosixServerSocket() {
}


Error PosixServerSocket::
doAccept(Socket **sock, SocketAddress **accepted_addr){
	Error rc;

	sockaddr *addr = (sockaddr *)::malloc(MAX_ADDRESS_LENGTH);
	size_t addr_len = MAX_ADDRESS_LENGTH;
	PosixSocket *ret = 0;

	int fd = ::accept(getFileDescriptor(),addr,(socklen_t *)&addr_len);

#ifdef OS_WIN32
	//TODO: Implement win32
#endif

#ifdef OS_UNIX
	if (fd < 0) {
		rc.setSystemError(errno);
		goto error_out;
	}
#endif

	{
		*sock = createInstance(fd);
		if (accepted_addr) {
			*accepted_addr = ret->createAddressInstance(addr,addr_len);
		}
	}

	return rc;

	error_out:
	::free(addr);
	return rc;
}

Error PosixServerSocket::
doBind(const SocketAddress *bind_addr) {
	Error ret;
	const sockaddr *addr = 0;
	size_t addr_len = 0;
	const PosixSocketAddress *paddr = static_cast<const PosixSocketAddress *>(bind_addr);
	addr = paddr->getPosixAddress(&addr_len);
	assert(addr != 0 && addr_len > 0);
	int rc =  ::bind(getFileDescriptor(),addr,addr_len);

#ifdef OS_WIN32
	//TODO: Implement win32
#endif

#ifdef OS_UNIX
	ret.setSystemError(errno);
#endif
	return ret;
}

Error PosixServerSocket::
doListen(int b) {
	Error ret;
	int rc = ::listen(getFileDescriptor(), b);
#ifdef OS_WIN32
	//TODO: Implement win32
#endif

#ifdef OS_UNIX
	if (rc < 0)
		ret.setSystemError(errno);
#endif

	return ret;
}
