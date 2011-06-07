/*
 * Socket.h
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#include "RString.h"
#include "IODevice.h"
#include "utils/NonCopyable.hpp"

class SocketAddress;
class Socket: public IODevice{
public:
	Socket();
	virtual ~Socket();

	const SocketAddress * getAddress() const;
	const SocketAddress * getPeerAddress() const;
	Error connect(const SocketAddress *);
	Error bind (const SocketAddress *);
	Error listen(int);
	Error accept(Socket **sock,SocketAddress ** addr = 0);

protected:
	virtual const SocketAddress * doGetAddress() const = 0;
	virtual const SocketAddress * doGetPeerAddress() const = 0;
	virtual Error doConnect(const SocketAddress *) = 0;
	virtual Error doAccept(Socket **sock, SocketAddress ** addr = 0) = 0;
	virtual Error doBind(const SocketAddress *) = 0;
	virtual Error doListen(int) = 0;
};

#endif /* SOCKET_H_ */
