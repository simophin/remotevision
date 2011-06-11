/*
 * PosixSocket.h
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#ifndef POSIXSOCKET_H_
#define POSIXSOCKET_H_

#include "Socket.h"
#include <memory>

struct sockaddr;
class PosixSocketAddress;
class PosixSocket: public Socket {
public:
	PosixSocket(int fd);
	virtual ~PosixSocket();

	int getFileDescriptor() const;

	virtual PosixSocketAddress * createAddressInstance (const sockaddr *, size_t addr_len) const= 0;
	virtual PosixSocket * createInstance(int) const = 0;

	void setBlockingMode (bool );

protected:
	virtual Error doRead (char *, size_t, size_t *);
	virtual Error doWrite (const char *, size_t, size_t *);
	virtual Error doListen(int);
	virtual Error doAccept(Socket **sock,SocketAddress ** addr = 0);
	virtual Error doBind(const SocketAddress *);
	virtual Error doPoll(int,int);
	virtual void doClose();
	virtual const SocketAddress * doGetAddress() const;
	virtual const SocketAddress * doGetPeerAddress() const;
	virtual Error doConnect(const SocketAddress *);
	virtual void doFlush();

private:
	void init();

private:
	class PosixSocketImpl;
	PosixSocketImpl *d;
};

#endif /* POSIXSOCKET_H_ */
