/*
 * SocketAddress.cpp
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#include "SocketAddress.h"

SocketAddress::SocketAddress() {
}

SocketAddress::~SocketAddress() {
}


std::string SocketAddress::
getReadable() const {
	return doGetReadable();
}
std::string SocketAddress::
getAddress() const {
	return doGetAddress();
}
unsigned int SocketAddress::
getPort() const {
	return doGetPort();
}
