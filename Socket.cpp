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

Error Socket::
connect(const SocketAddress * addr) {
	return doConnect(addr);
}

Error Socket::bind(const SocketAddress *addr)
{
	return doBind(addr);
}



Error Socket::listen(int backlog)
{
	return doListen(backlog);
}


Error Socket::accept(Socket **sock, SocketAddress **addr)
{
	return doAccept(sock,addr);
}


