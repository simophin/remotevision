/*
 * PosixServerSocket.h
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#ifndef POSIXSERVERSOCKET_H_
#define POSIXSERVERSOCKET_H_

#include "ServerSocket.h"
#include "PosixSocket.h"

class PosixServerSocket: public ServerSocket, public PosixSocket {
public:
	PosixServerSocket(int fd);
	virtual ~PosixServerSocket();

protected:
	virtual Socket * doAccept(SocketAddress **addr = 0);
	virtual const SocketAddress * doGetAddress() const;
	virtual int doBind(const SocketAddress *);
	virtual int doListen(int);
};

#endif /* POSIXSERVERSOCKET_H_ */
