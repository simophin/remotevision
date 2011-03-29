/*
 * TCPSocketAddress.cpp
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#include "TCPSocketAddress.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

class TCPSocketAddress::TCPSocketAddressImpl {
};

TCPSocketAddress::TCPSocketAddress()
:d(new TCPSocketAddressImpl){
	init();
}

TCPSocketAddress::TCPSocketAddress(const std::string &saddr, unsigned int port)
:d(new TCPSocketAddressImpl){
	setAddress(saddr,port);
	init();
}
TCPSocketAddress::TCPSocketAddress(const sockaddr *addr, size_t addr_len)
:d(new TCPSocketAddressImpl), PosixSocketAddress(addr,addr_len){
	init();
}

TCPSocketAddress::TCPSocketAddress(const TCPSocketAddress &rhs)
:d(new TCPSocketAddress::TCPSocketAddressImpl(*rhs.d)) {
	init();
}

TCPSocketAddress::~TCPSocketAddress() {
	delete d;
}

void TCPSocketAddress::init() {

}

void TCPSocketAddress::
setAddress(const std::string &saddr) {
	size_t len;
	const sockaddr *raddr = getPosixAddress(&len);
	sockaddr_in addr;
	if (!raddr || len != sizeof(addr)) {
		::memset(&addr,0,sizeof(addr));
		addr.sin_family = AF_INET;
	}else{
		addr = *(sockaddr_in *)(raddr);
	}

	addr.sin_addr.s_addr = inet_network(saddr.c_str());
	assert(addr.sin_addr.s_addr != (in_addr_t)(-1));
	setPosixAddress((sockaddr *)&addr,sizeof(addr));
}


void TCPSocketAddress::
setAddress(const std::string &addr,unsigned int p) {
	setAddress(addr);
	setPort(p);
}
void TCPSocketAddress::
setPort(unsigned int port) {
	size_t len;
	const sockaddr *raddr = getPosixAddress(&len);
	sockaddr_in addr;
	if (!raddr || len != sizeof(addr)) {
		::memset(&addr,0,sizeof(addr));
		addr.sin_family = AF_INET;
	}else{
		addr = *(const sockaddr_in *)(raddr);
	}

	addr.sin_port = htons(port);
	setPosixAddress((sockaddr *)&addr,sizeof(addr));
}
