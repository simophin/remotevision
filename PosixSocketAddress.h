/*
 * PosixSocketAddress.h
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#ifndef POSIXSOCKETADDRESS_H_
#define POSIXSOCKETADDRESS_H_

#include "SocketAddress.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <memory>

class PosixSocketAddress: public SocketAddress {
public:
	PosixSocketAddress(const sockaddr *addr = 0, size_t addr_len = 0);
	virtual ~PosixSocketAddress();
	PosixSocketAddress (const PosixSocketAddress &);

	void getPosixAddress(sockaddr **addr, size_t *addr_len) const;
	void setPosixAddress(const sockaddr *addr, size_t addr_len);

protected:
	virtual std::string doGetReadable() const;
	virtual std::string doGetAddress() const;
	virtual unsigned int doGetPort() const;

private:
	class PosixSocketAddressImpl;
	PosixSocketAddressImpl * d;
};

#endif /* POSIXSOCKETADDRESS_H_ */
