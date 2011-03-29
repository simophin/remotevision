/*
 * ServerSocket.h
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#ifndef SERVERSOCKET_H_
#define SERVERSOCKET_H_

class SocketAddress;
class ServerSocket {
public:
	ServerSocket();
	virtual ~ServerSocket();

	SocketAddress * accept();

protected:
	virtual SocketAddress * doAccept() = 0;
};

#endif /* SERVERSOCKET_H_ */
