/*
 * Socket.cpp
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#include "Socket.h"
#include "SocketAddress.h"

Socket::Socket() {

}

Socket::~Socket() {
}

const SocketAddress * Socket::
getAddress() const {
	return doGetAddress();
}

const SocketAddress * Socket::
getPeerAddress() const {
	return doGetPeerAddress();
}

int Socket::
connect(const SocketAddress * addr) {
	return doConnect(addr);
}

int Socket::bind(const SocketAddress *addr)
{
	return doBind(addr);
}



int Socket::listen(int backlog)
{
	return doListen(backlog);
}



Socket *Socket::accept(SocketAddress **addr)
{
	return doAccept(addr);
}


