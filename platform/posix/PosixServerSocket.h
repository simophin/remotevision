/*
 * PosixServerSocket.h
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#ifndef POSIXSERVERSOCKET_H_
#define POSIXSERVERSOCKET_H_

#include "PosixSocket.h"

class PosixServerSocket: public PosixSocket {
public:
	PosixServerSocket(int fd);
	virtual ~PosixServerSocket();

protected:
	virtual Socket * doAccept(SocketAddress **addr = 0);
	virtual int doBind(const SocketAddress *);
	virtual int doListen(int);
};

#endif /* POSIXSERVERSOCKET_H_ */
