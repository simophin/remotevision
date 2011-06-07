/*
 * SocketAddress.h
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#ifndef SOCKETADDRESS_H_
#define SOCKETADDRESS_H_

#include "RString.h"

class SocketAddress {
public:
	SocketAddress();
	virtual ~SocketAddress();

	String getReadable() const;

protected:
	virtual String doGetReadable() const = 0;
};

#endif /* SOCKETADDRESS_H_ */
