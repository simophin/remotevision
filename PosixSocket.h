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

class PosixSocket: public Socket {
public:
	PosixSocket(int fd);
	virtual ~PosixSocket();

	int getFileDescriptor() const;

protected:
	virtual ssize_t doRead (unsigned char *, size_t);
	virtual ssize_t doWrite (const unsigned char *, size_t);
	virtual void doClose();
	virtual const SocketAddress * doGetAddress() const;
	virtual const SocketAddress * doGetPeerAddress() const;

private:
	void init();

private:
	class PosixSocketImpl;
	std::auto_ptr<PosixSocketImpl> d;
};

#endif /* POSIXSOCKET_H_ */
