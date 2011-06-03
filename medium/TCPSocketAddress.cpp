/*
 * TCPSocketAddress.cpp
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#include "TCPSocketAddress.h"

#include <sys/types.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include <iosfwd>
#include <streambuf>
#include <string>
#include <boost/cast.hpp>
#include <boost/lexical_cast.hpp>
#include "platform/posix/PosixCompactHeader.h"


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

std::string TCPSocketAddress::doGetReadable() const
{
	std::string ret = getAddress();
	ret += ":";
	ret += boost::lexical_cast<std::string>(getPort());
	return ret;
}

unsigned int TCPSocketAddress::getPort() const {
	size_t addr_len;
	const sockaddr_in * addr = (sockaddr_in *)getPosixAddress(&addr_len);
	assert(addr != 0);
	unsigned short ret = ::ntohs(addr->sin_port);
	return ret;
}
std::string TCPSocketAddress::getAddress() const {
	size_t addr_len;
	const sockaddr_in * addr = (sockaddr_in *)getPosixAddress(&addr_len);
	assert(addr != 0);
	return std::string(inet_ntoa(addr->sin_addr));
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

	addr.sin_addr.s_addr = inet_addr(saddr.c_str());
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
