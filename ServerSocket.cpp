/*
 * ServerSocket.cpp
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#include "ServerSocket.h"

ServerSocket::ServerSocket() {
}

ServerSocket::~ServerSocket() {
}

Socket * ServerSocket::accept(SocketAddress **addr) {
	return doAccept(addr);
}

int ServerSocket::bind(const SocketAddress *addr) {
	return doBind(addr);
}

int ServerSocket::listen(int backlog) {
	return doListen(backlog);
}
