/*
 * TestNetwork.cpp
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#include "medium/TCPSocket.h"
#include "medium/TCPServerSocket.h"
#include "medium/TCPSocketAddress.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


int main() {
	int sock = socket(AF_INET, SOCK_STREAM, 0);

	TCPServerSocket server (sock);
	TCPSocketAddress localAddr("0.0.0.0", 10001);
	if (server.bind(&localAddr)) {
		perror("While binding");
		return -1;
	}

	if (server.listen(10)) {
		perror("While listening");
		return -2;
	}

	TCPSocketAddress *addr = 0;
	TCPSocket *socket = (TCPSocket *)(server.accept((SocketAddress **)&addr));
	if (socket == NULL) {
		std::cout << "Error while accepting new connection" <<std::endl;
		return -3;
	}

	std::cout << "Get remote "<<socket->getPeerAddress()->getReadable()<<std::endl;

	socket->close();
	server.close();
}
