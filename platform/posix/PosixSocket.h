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

protected:
	virtual ssize_t doRead (char *, size_t);
	virtual ssize_t doWrite (const char *, size_t);
	virtual int doPoll(PollType,int);
	virtual void doClose();
	virtual const SocketAddress * doGetAddress() const;
	virtual const SocketAddress * doGetPeerAddress() const;
	virtual int doConnect(const SocketAddress *);
	virtual std::string doGetLastError();

private:
	void init();

private:
	class PosixSocketImpl;
	std::auto_ptr<PosixSocketImpl> d;
};

#endif /* POSIXSOCKET_H_ */
