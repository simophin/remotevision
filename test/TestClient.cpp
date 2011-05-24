/*
 * TestClient.cpp
 *
 *  Created on: 2011-5-14
 *      Author: simophin
 */

#include "medium/TCPSocket.h"
#include "medium/TCPSocketAddress.h"
#include "vsources/IOVideoSource.h"
#include "VideoFormat.h"

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
		std::cerr << socket.getLastError() << std::endl;
		return -1;
	}

	IOVideoSource source (&socket,NULL);
	IOVideoSource::Info info = source.getInformation();

	std::cout << "Supported geometry are: "<<std::endl;
	for (int i=0;i<info.providerInfo.supportedGeometries.size();i++) {
		std::cout << "(" << info.providerInfo.supportedGeometries[i].toString() << ")" << std::endl;
	}

	std::cout << "Supported codecs are: "<<std::endl;
	for (int i=0;i<info.providerInfo.supportedCodecs.size();i++) {
		std::cout << "(" << info.providerInfo.supportedCodecs[i].toString() << ")" << std::endl;
	}

	VideoFormat fmt( info.providerInfo.supportedCodecs[0],info.providerInfo.supportedGeometries[0]);
	if (!source.setFormat(fmt)) {
		std::cout << "Error is "<<source.getLastError() << std::endl;
	}

	return 0;
}
