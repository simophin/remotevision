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


String SocketAddress::
getReadable() const {
	return doGetReadable();
}
