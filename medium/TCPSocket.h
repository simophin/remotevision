/*
 * TCPSocket.h
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#ifndef TCPSOCKET_H_
#define TCPSOCKET_H_

#include "platform/PosixSocket.h"

class TCPSocket: public PosixSocket {
public:
	TCPSocket(int);
	virtual ~TCPSocket();

	virtual PosixSocketAddress * createAddressInstance (const sockaddr *, size_t addr_len) const;
	virtual PosixSocket * createInstance(int) const;
};

#endif /* TCPSOCKET_H_ */
