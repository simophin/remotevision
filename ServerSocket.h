/*
 * ServerSocket.h
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#ifndef SERVERSOCKET_H_
#define SERVERSOCKET_H_

class SocketAddress;
class Socket;
class ServerSocket {
public:
	ServerSocket();
	virtual ~ServerSocket();

	int bind (const SocketAddress *);
	Socket * accept(SocketAddress * addr = 0);

protected:
	virtual Socket * doAccept(SocketAddress * addr = 0) = 0;
	virtual int doBind(const SocketAddress *) = 0;
};

#endif /* SERVERSOCKET_H_ */
