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
};

#endif /* TCPSOCKET_H_ */
