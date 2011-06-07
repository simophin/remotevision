/*
 * TCPSocketAddress.h
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#ifndef TCPSOCKETADDRESS_H_
#define TCPSOCKETADDRESS_H_

#include "platform/posix/PosixSocketAddress.h"
#include "RString.h"


struct sockaddr ;

class TCPSocketAddress: public PosixSocketAddress {
public:
	TCPSocketAddress();
	TCPSocketAddress(const String &, unsigned int port);
	TCPSocketAddress(const sockaddr *, size_t addr_len);
	TCPSocketAddress(const TCPSocketAddress &);
	virtual ~TCPSocketAddress();

	void setAddress(const String &);
	void setAddress(const String &,unsigned int);
	void setPort(unsigned int);

	unsigned int getPort() const;
	String getAddress() const;

protected:
	virtual String doGetReadable() const;

private:
	void init();

private:
	class TCPSocketAddressImpl;
	TCPSocketAddressImpl *d;
};

#endif /* TCPSOCKETADDRESS_H_ */
