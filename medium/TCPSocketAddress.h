/*
 * TCPSocketAddress.h
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#ifndef TCPSOCKETADDRESS_H_
#define TCPSOCKETADDRESS_H_

#include "PosixSocketAddress.h"
#include <string>
#include <sys/types.h>
#include <sys/socket.h>


class TCPSocketAddress: public PosixSocketAddress {
public:
	TCPSocketAddress();
	TCPSocketAddress(const std::string &, unsigned int port);
	TCPSocketAddress(const sockaddr *, size_t addr_len);
	TCPSocketAddress(const TCPSocketAddress &);
	virtual ~TCPSocketAddress();

	void setAddress(const std::string &);
	void setAddress(const std::string &,unsigned int);
	void setPort(unsigned int);

	unsigned int getPort() const;
	std::string getAddress() const;

protected:
	virtual std::string doGetReadable() const;

private:
	void init();

private:
	class TCPSocketAddressImpl;
	TCPSocketAddressImpl *d;
};

#endif /* TCPSOCKETADDRESS_H_ */
