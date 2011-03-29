/*
 * PosixSocketAddress.h
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#ifndef POSIXSOCKETADDRESS_H_
#define POSIXSOCKETADDRESS_H_

#include "SocketAddress.h"

class PosixSocketAddress: public SocketAddress {
public:
	PosixSocketAddress();
	virtual ~PosixSocketAddress();

protected:
	virtual std::string doGetReadable() const;
	virtual std::string doGetAddress() const;
	virtual unsigned int doGetPort() const;
};

#endif /* POSIXSOCKETADDRESS_H_ */
