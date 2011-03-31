/*
 * PosixSocketAddress.cpp
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#include "PosixSocketAddress.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

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
};

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

const sockaddr * PosixSocketAddress::
getPosixAddress(size_t *addr_len) const {
	if (addr_len) *addr_len = d->addressLength;
	return d->address;
}

void PosixSocketAddress::
setPosixAddress(const sockaddr *addr, size_t addr_len) {
	d->address = (sockaddr *)::realloc(d->address,addr_len);
	::memcpy(d->address, addr, addr_len);
	d->addressLength = addr_len;
}
