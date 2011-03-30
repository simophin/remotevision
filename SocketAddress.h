/*
 * SocketAddress.h
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#ifndef SOCKETADDRESS_H_
#define SOCKETADDRESS_H_

#include <string>

class SocketAddress {
public:
	SocketAddress();
	virtual ~SocketAddress();

	std::string getReadable() const;

protected:
	virtual std::string doGetReadable() const = 0;
};

#endif /* SOCKETADDRESS_H_ */
