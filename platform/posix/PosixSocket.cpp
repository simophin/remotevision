/*
 * PosixSocket.cpp
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#include "PosixSocket.h"
#include "PosixSocketAddress.h"
#include <unistd.h>

#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "PosixCompactHeader.h"

const int MAX_ADDRESS_LENGTH = 1024;

class PosixSocket::PosixSocketImpl {
public:
	int fd;
	PosixSocketAddress *addr, *peerAddr;

	~PosixSocketImpl() {
		if (addr) delete addr;
		if (peerAddr) delete peerAddr;
	}

	PosixSocketImpl()
	:addr(0), peerAddr(0) {}
};

PosixSocket::PosixSocket(int fd)
:d(new PosixSocket::PosixSocketImpl)
{
	d->fd = fd;
	init();
}

PosixSocket::~PosixSocket() {
	delete d;
}

Error PosixSocket::doListen(int backlog)
{
	return Error::ERR_INVALID;
}

Error PosixSocket::doAccept(Socket **sock,SocketAddress **addr)
{
	if (addr) *addr = NULL;
	return Error();
}

Error PosixSocket::doBind(const SocketAddress *addr)
{
	return Error::ERR_INVALID;
}

void PosixSocket::doFlush()
{
}

void PosixSocket::
init() {

}


int PosixSocket::
getFileDescriptor() const
{
	return d->fd;
}



Error PosixSocket::
doRead(char *data, size_t size, size_t *read_size)
{
	Error err;
	//int rc = ::read(d->fd,(void *)data,size);
	int rc = ::recv(d->fd, data, size, 0);
#ifdef OS_WIN32
	if (rc == SOCKET_ERROR) {
		err.setSystemError(WSAGetLastError());
	}else if (read_size) *read_size = rc;
#endif

#ifdef OS_UNIX
	if (rc < 0) err.setSystemError(errno);
	else if (read_size) *read_size = rc;
#endif

	return err;
}



void PosixSocket::
doClose()
{
	::shutdown(d->fd,1);
	::close(d->fd);
}



const SocketAddress *PosixSocket::
doGetAddress() const
{
	if (d->addr == 0) {
		// Get sockname
		sockaddr * addr = ((sockaddr *)::malloc(MAX_ADDRESS_LENGTH));
		size_t addr_len;
		assert(addr != 0);
		{
			addr_len = MAX_ADDRESS_LENGTH;
			::getsockname(d->fd, addr, (socklen_t *)&addr_len);
			assert(addr_len <= MAX_ADDRESS_LENGTH);
			d->addr = createAddressInstance(addr,addr_len);
			::free(addr);
		}
		assert(d->addr != 0);
	}

	return d->addr;
}

const SocketAddress *PosixSocket::
doGetPeerAddress() const
{
	if (d->peerAddr == 0){
		// Get peername
		sockaddr * addr = ((sockaddr *)::malloc(MAX_ADDRESS_LENGTH));
		size_t addr_len = MAX_ADDRESS_LENGTH;
		{
			::getpeername(d->fd, addr, (socklen_t *)&addr_len);
			assert(addr_len <= MAX_ADDRESS_LENGTH);
			d->peerAddr = createAddressInstance(addr,addr_len);
			::free(addr);
		}
		assert(d->peerAddr != 0);
	}
	return d->peerAddr;
}


Error PosixSocket::
doWrite(const char *data, size_t size, size_t *write_size)
{
	Error ret;
	//ssize_t rc = ::write(d->fd,data,size);
	int rc = ::send(d->fd,data, size, 0);

#ifdef OS_WIN32
	if (rc == SOCKET_ERROR) {
			ret.setSystemError(WSAGetLastError());
	}else {
		if (write_size) *write_size = rc;
	}
#endif

#ifdef OS_UNIX
	if (rc < 0) {
		ret.setSystemError(errno);
	}else{
		if (write_size) *write_size = rc;
	}
#endif

	return ret;
}

Error PosixSocket::doConnect(const SocketAddress *addr) {
	Error ret;
	const PosixSocketAddress *paddr = static_cast<const PosixSocketAddress *>(addr);
	size_t addr_len = 0;
	const sockaddr * saddr = paddr->getPosixAddress(&addr_len);
	int rc = ::connect(d->fd,saddr,addr_len);

#ifdef OS_WIN32
	if (rc == SOCKET_ERROR) {
			ret.setSystemError(WSAGetLastError());
	}
#endif

#ifdef OS_UNIX
	if (rc < 0) {
		ret.setSystemError(errno);
	}
#endif
	return ret;
}

Error PosixSocket::
doPoll(PosixSocket::PollType p, int timeout) {
	Error ret;
	fd_set readfds, writefds, exceptfds;
	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
	FD_ZERO(&exceptfds);

	switch(p){
	case POLL_READ:{
		FD_SET(d->fd,&readfds);
		break;
	}

	case POLL_WRITE:{
		FD_SET(d->fd,&writefds);
		break;
	}

	case POLL_ERROR:{
		FD_SET(d->fd,&exceptfds);
		break;
	}
	}

	struct timeval to, *tv = 0;
	if (timeout > 0) {
		to.tv_sec = 0;
		to.tv_usec = timeout*1000;
		tv = &to;
	}

	int rc = select(d->fd+1, &readfds, &writefds, &exceptfds, tv);

#ifdef OS_WIN32
	if (rc == 0) {
		ret.setErrorType(Error::ERR_TIMEOUT);
	}else if ( rc < 0) {
		ret.setSystemError(WSAGetLastError());
	}
	return ret;
#endif

#ifdef OS_UNIX
	if (rc == 0) {
		ret.setErrorType(Error::ERR_TIMEOUT);
	}else if ( rc < 0) {
		ret.setSystemError(errno);
	}
	return ret;
#endif
}




