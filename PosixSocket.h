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
	PosixSocketAddress * createAddress (const sockaddr *, size_t addr_len);
	PosixSocket * createInstance(int) const;

protected:
	virtual ssize_t doRead (unsigned char *, size_t);
	virtual ssize_t doWrite (const unsigned char *, size_t);
	virtual int doPoll(PollType,int);
	virtual void doClose();
	virtual const SocketAddress * doGetAddress() const;
	virtual const SocketAddress * doGetPeerAddress() const;
	virtual int doConnect(const SocketAddress *);
	virtual PosixSocketAddress *doCreateAddress(const sockaddr *, size_t addr_len) = 0;
	virtual PosixSocket * doCreateInstance(int) const = 0;

private:
	void init();

private:
	class PosixSocketImpl;
	std::auto_ptr<PosixSocketImpl> d;
};

#endif /* POSIXSOCKET_H_ */
