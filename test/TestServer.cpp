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
#include "RemoteVision.h"

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
	assert( WSAStartup(MAKEWORD(2, 2), &wsaData) == 0);
#endif

	RemoteVisionApp rv ("/dev/video0");
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

	Server s (socket,socket,rv.getProvider());
	s.start();

	socket = (TCPSocket *)(server.accept(0));
	return 0;
}
