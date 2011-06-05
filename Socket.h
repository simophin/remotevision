/*
 * Socket.h
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#include <string>
#include "IODevice.h"
#include "Utils.h"

class SocketAddress;
class Socket: public Utils::NonCopyable, public  IODevice{
public:
	Socket();
	virtual ~Socket();

	const SocketAddress * getAddress() const;
	const SocketAddress * getPeerAddress() const;
	int connect(const SocketAddress *);
	int bind (const SocketAddress *);
	int listen(int);
	Socket * accept(SocketAddress ** addr = 0);

protected:
	virtual const SocketAddress * doGetAddress() const = 0;
	virtual const SocketAddress * doGetPeerAddress() const = 0;
	virtual int doConnect(const SocketAddress *) = 0;
	virtual Socket * doAccept(SocketAddress ** addr = 0) = 0;
	virtual int doBind(const SocketAddress *) = 0;
	virtual int doListen(int) = 0;
};

#endif /* SOCKET_H_ */
