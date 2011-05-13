/*
 * TestNetwork.cpp
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#include "medium/TCPSocket.h"
#include "medium/TCPServerSocket.h"
#include "medium/TCPSocketAddress.h"

#include "platform/posix/PosixCompactHeader.h"
#include <iostream>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


int main() {

#ifdef OS_WIN32
	WSAData wsaData;
	int nCode;
	assert( WSAStartup(MAKEWORD(2, 2), &wsaData) == 0);
#endif
	int sock = socket(AF_INET, SOCK_STREAM, 0);

	TCPServerSocket server (sock);
	TCPSocketAddress localAddr("127.0.0.1", 10001);
	if (server.bind(&localAddr)){
		std::cout << server.getLastError() << std::endl;
		return -1;
	}

	if (server.listen(10)) {
		perror("While listening");
		return -2;
	}

	TCPSocketAddress *addr = 0;
	server.poll(IODevice::POLL_READ,-1);
	TCPSocket *socket = (TCPSocket *)(server.accept((SocketAddress **)&addr));
	if (socket == NULL) {
		std::wcout << "Error while accepting new connection: " << (wchar_t *)server.getLastError().c_str()<< std::endl;
		return -3;
	}

	std::cout << "Get remote "<<socket->getPeerAddress()->getReadable()<<std::endl;

	socket->close();
	server.close();
}
