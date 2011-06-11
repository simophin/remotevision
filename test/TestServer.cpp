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
#include <time.h>
#ifdef OS_UNIX
#include <signal.h>
#endif

#ifdef OS_WIN32
#include <windows.h>
#define sleep(n) Sleep(1000 * n)
#endif

TCPFFMpegServer *server = 0;

void onExit (int) {
	if (server) {
		server->stop();
	}

	sleep(1);
}

int main () {
#ifdef OS_WIN32
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif


#ifdef OS_UNIX
	signal(SIGTERM, onExit);
	signal(SIGABRT, onExit);
	signal(SIGKILL, onExit);
#endif

	Error rc;
	FFMpeg::init();
	server = new TCPFFMpegServer(TCPFFMpegServer::RELAY,"127.0.0.1",15000);
	rc = server->init("/dev/video0");
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
	return 0;
}
