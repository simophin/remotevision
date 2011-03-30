/*
 * TCPSocket.h
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#ifndef TCPSOCKET_H_
#define TCPSOCKET_H_

#include "PosixSocket.h"

class TCPSocket: public PosixSocket {
public:
	TCPSocket(int);
	virtual ~TCPSocket();

protected:
	virtual PosixSocketAddress *doCreateAddress(const sockaddr *, size_t addr_len);
	virtual PosixSocket * doCreateInstance(int) const;
};

#endif /* TCPSOCKET_H_ */
