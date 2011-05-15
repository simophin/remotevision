/*
 * TestClient.cpp
 *
 *  Created on: 2011-5-14
 *      Author: simophin
 */

#include "medium/TCPSocket.h"
#include "medium/TCPSocketAddress.h"
#include "vsources/IOVideoSource.h"

#include <iostream>
#include <assert.h>

int main () {
#ifdef OS_WIN32
	WSAData wsaData;
	int nCode;
	assert( WSAStartup(MAKEWORD(2, 2), &wsaData) == 0);
#endif

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	TCPSocket socket (sock);

	TCPSocketAddress remoteAddr ("127.0.0.1", 10001);

	if (socket.connect(&remoteAddr)) {
		std::wcerr << socket.getLastError() << std::endl;
		return -1;
	}

	IOVideoSource source (&socket,NULL);
	VideoInfo info = source.getInformation();

	std::cout << "Supported geometry are: "<<std::endl;
	for (int i=0;i<info.supportedGeometry.size();i++) {
		std::cout << "(" << info.supportedGeometry[i].toString() << ")" << std::endl;
	}

	std::cout << "Supported codecs are: "<<std::endl;
	for (int i=0;i<info.supportedVideoCodecs.size();i++) {
		std::cout << "(" << info.supportedVideoCodecs[i].toString() << ")" << std::endl;
	}

	return 0;
}
