/*
 * PosixSocketAddress.h
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#ifndef POSIXSOCKETADDRESS_H_
#define POSIXSOCKETADDRESS_H_

#include "SocketAddress.h"

#include "PosixCompactHeader.h"

#include <memory>

class PosixSocketAddress: public SocketAddress {
public:
	PosixSocketAddress(const sockaddr *addr = 0, size_t addr_len = 0);
	virtual ~PosixSocketAddress();
	PosixSocketAddress (const PosixSocketAddress &);

	const sockaddr * getPosixAddress(size_t *addr_len) const;
	void setPosixAddress(const sockaddr *addr, size_t addr_len);

protected:
	virtual String doGetReadable() const = 0;

private:
	class PosixSocketAddressImpl;
	PosixSocketAddressImpl * d;
};

#endif /* POSIXSOCKETADDRESS_H_ */
