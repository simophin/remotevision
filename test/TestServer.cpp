/*
 * TestServer.cpp
 *
 *  Created on: 2011-5-5
 *      Author: launliu
 */

#include "Server.h"
#include "medium/TCPSocket.h"
#include "medium/TCPServerSocket.h"
#include "medium/TCPSocketAddress.h"
#include "medium/TCPFFMpegServer.h"
#include "platform/posix/PosixCompactHeader.h"
#include "3rdparty/ffmpeg/FFMpeg.h"
#include <iostream>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

int main () {
#ifdef OS_WIN32
	WSAData wsaData;
	int nCode;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
	/*
	int sock = socket(AF_INET, SOCK_STREAM, 0);

	TCPServerSocket server (sock);
	TCPSocketAddress localAddr("0.0.0.0", 10001);
	if (server.bind(&localAddr)) {
		std::cerr << server.getLastError();
		return -1;
	}

	if (server.listen(10)) {
		perror("While listening");
		return -2;
	}

	TCPSocketAddress *addr = 0;
	TCPSocket *socket = (TCPSocket *)(server.accept((SocketAddress **)&addr));
	if (socket == NULL) {
		std::cerr << server.getLastError();
		return -3;
	}

	std::cout << "Remote: "<<addr->getReadable() << std::endl;

	TCPSocket *dataSocket = (TCPSocket *)(server.accept());
	if (dataSocket == NULL) {
		std::cerr << server.getLastError();
		return -4;
	}

	Server s (socket,dataSocket,rv.getProvider());
	s.start();

	socket = (TCPSocket *)(server.accept(0));
	return 0;
	*/

	Error rc;
	FFMpeg::init();
	TCPFFMpegServer *server = new TCPFFMpegServer("0.0.0.0",0);
	rc = server->init("list");
	if (rc.isError()) {
		std::cerr << rc << std::endl;
		delete server;
		return 1;
	}
	std::cout << "Server bound on "<<server->getBoundInfo() << std::endl;

	rc = server->start();
	if (rc.isError()) {
		std::cerr << rc << std::endl;
		delete server;
		return 2;
	}

	server->wait();
	delete server;
	return 3;
}
