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

SocketAddress * ServerSocket::accept() {
	return doAccept();
}
