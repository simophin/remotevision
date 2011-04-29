/*
 * Win32Socket.h
 *
 *  Created on: 2011-4-29
 *      Author: launliu
 */

#ifndef WIN32SOCKET_H_
#define WIN32SOCKET_H_

#include "Socket.h"

class Win32Socket: public Socket {
public:
	Win32Socket();
	virtual ~Win32Socket();

protected:
	virtual const SocketAddress * doGetAddress() const;
	virtual const SocketAddress * doGetPeerAddress() const;
	virtual int doConnect(const SocketAddress *);
};

#endif /* WIN32SOCKET_H_ */
