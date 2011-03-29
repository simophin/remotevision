/*
 * PosixSocketAddress.cpp
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#include "PosixSocketAddress.h"
#include <stdlib.h>
#include <string.h>

class PosixSocketAddress::PosixSocketAddressImpl {
public:
	sockaddr *address;
	size_t addressLength;

	PosixSocketAddressImpl()
	:address(0), addressLength(0) {}

	PosixSocketAddressImpl(const PosixSocketAddressImpl &rhs)
	:address(0), addressLength(rhs.addressLength) {
		if (rhs.addressLength > 0) {
			address = (sockaddr *)::malloc(rhs.addressLength);
			::memcpy(address, rhs.address, rhs.addressLength);
		}
	}

	~PosixSocketAddressImpl() {
		::free(address);
	}
}

PosixSocketAddress::PosixSocketAddress(const sockaddr *addr, size_t addr_len)
:d(new PosixSocketAddress::PosixSocketAddressImpl){
	d->address = (sockaddr *)::malloc(addr_len);
	assert(d->address != 0);
	::memcpy(d->address, addr, addr_len);
	d->addressLength = addr_len;
}

PosixSocketAddress::PosixSocketAddress (const PosixSocketAddress &rhs)
:d(new PosixSocketAddress::PosixSocketAddressImpl(*rhs.d)) {

}

PosixSocketAddress::~PosixSocketAddress() {
	delete d;
}

void PosixSocketAddress::
getPosixAddress(sockaddr **addr, size_t *addr_len) const {
	*addr = (sockaddr *)::malloc(d->addressLength);
	::memcpy(*addr, d->address, d->addressLength);
	*addr_len = d->addressLength;
}

std::string PosixSocketAddress::
doGetReadable() const{
	return std::string();
}
std::string PosixSocketAddress::
doGetAddress() const{
	return std::string();
}
unsigned int PosixSocketAddress::
doGetPort() const{
	return 0;
}


void PosixSocketAddress::
setPosixAddress(const sockaddr *addr, size_t addr_len) {
	d->address = (sockaddr *)::realloc(d->address,addr_len);
	::memcpy(d->address, addr, addr_len);
	d->addressLength = addr_len;
}
