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
