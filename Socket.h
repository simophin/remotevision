/*
 * Socket.h
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#include <boost/noncopyable.hpp>
#include <string>
#include "IODevice.h"

class SocketAddress;
class Socket: public boost::noncopyable, public  IODevice{
public:
	Socket();
	virtual ~Socket();

	const SocketAddress * getAddress() const;
	const SocketAddress * getPeerAddress() const;
	int connect(const SocketAddress *);

protected:
	virtual const SocketAddress * doGetAddress() const = 0;
	virtual const SocketAddress * doGetPeerAddress() const = 0;
	virtual int doConnect(const SocketAddress *) = 0;
};

#endif /* SOCKET_H_ */
