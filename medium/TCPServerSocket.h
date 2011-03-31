/*
 * TCPServerSocket.h
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#ifndef TCPSERVERSOCKET_H_
#define TCPSERVERSOCKET_H_

#include "PosixServerSocket.h"

class TCPSocketAddress;
class TCPServerSocket: public PosixServerSocket {
public:
	TCPServerSocket(int);
	virtual ~TCPServerSocket();

public:
	virtual PosixSocketAddress *createAddressInstance(const sockaddr *, size_t addr_len) const;
	virtual PosixSocket * createInstance(int) const;


};

#endif /* TCPSERVERSOCKET_H_ */
