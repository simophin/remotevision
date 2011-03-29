/*
 * PosixSocket.cpp
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#include "PosixSocket.h"
#include "PosixSocketAddress.h"
#include <unistd.h>

class PosixSocket::PosixSocketImpl {
public:
	int fd;
	PosixSocketAddress addr;
};

PosixSocket::PosixSocket(int fd)
:d(new PosixSocket::PosixSocketImpl)
{
	d->fd = fd;
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
	return &d->addr;
}


ssize_t PosixSocket::
doWrite(const unsigned char *data, size_t size)
{
	return ::write(d->fd,data,size);
}


