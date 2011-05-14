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
