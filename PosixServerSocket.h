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
	virtual SocketAddress * doAccept();
	virtual const SocketAddress * doGetAddress() const;
};

#endif /* POSIXSERVERSOCKET_H_ */
