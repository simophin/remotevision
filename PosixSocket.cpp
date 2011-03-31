/*
 * PosixSocket.cpp
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#include "PosixSocket.h"
#include "PosixSocketAddress.h"
#include <unistd.h>
#include <boost/scoped_ptr.hpp>

#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/poll.h>

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
}

void PosixSocket::
init() {

}


int PosixSocket::
getFileDescriptor() const
{
	return d->fd;
}



ssize_t PosixSocket::
doRead(unsigned char *data, size_t size)
{
	return ::read(d->fd,data,size);
}



void PosixSocket::
doClose()
{
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
			::getsockname(d->fd, addr, &addr_len);
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
			::getpeername(d->fd, addr, &addr_len);
			assert(addr_len <= MAX_ADDRESS_LENGTH);
			d->peerAddr = createAddressInstance(addr,addr_len);
			::free(addr);
		}
		assert(d->peerAddr != 0);
	}
	return d->peerAddr;
}


ssize_t PosixSocket::
doWrite(const unsigned char *data, size_t size)
{
	return ::write(d->fd,data,size);
}

int PosixSocket::doConnect(const SocketAddress *addr) {
	const PosixSocketAddress *paddr = static_cast<const PosixSocketAddress *>(addr);
	size_t addr_len = 0;
	const sockaddr * saddr = paddr->getPosixAddress(&addr_len);
	return ::connect(d->fd,saddr,addr_len);
}

int PosixSocket::
doPoll(PosixSocket::PollType p, int timeout) {
	pollfd pfd;
	pfd.fd = d->fd;

	switch(p){
	case POLL_READ:{
		pfd.events = POLLIN;
		break;
	}

	case POLL_WRITE:{
		pfd.events = POLLOUT;
		break;
	}

	case POLL_ERROR:{
		pfd.events = POLLERR;
		break;
	}
	}

	return ::poll(&pfd,1,timeout);
}


