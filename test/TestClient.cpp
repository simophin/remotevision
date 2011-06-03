/*
 * TestClient.cpp
 *
 *  Created on: 2011-5-14
 *      Author: simophin
 */

#include "medium/TCPSocket.h"
#include "medium/TCPSocketAddress.h"
#include "vsources/IOVideoSource.h"
#include "3rdparty/ffmpeg/FFMpeg.h"

#include <iostream>
#include <assert.h>

int main () {
#ifdef OS_WIN32
	WSAData wsaData;
	int nCode;
	assert( WSAStartup(MAKEWORD(2, 2), &wsaData) == 0);
#endif

	int sock = ::socket(AF_INET, SOCK_STREAM, 0);
	TCPSocket socket (sock);
	sock = ::socket(AF_INET, SOCK_STREAM,0);
	TCPSocket dataSocket (sock);

	TCPSocketAddress remoteAddr ("127.0.0.1", 10001);

	if (socket.connect(&remoteAddr)) {
		std::cerr << socket.getLastError() << std::endl;
		return -1;
	}

	if (dataSocket.connect(&remoteAddr)) {
		std::cerr << dataSocket.getLastError() << std::endl;
		return -2;
	}


	FFMpeg::init();
	IOVideoSource source (&socket,&dataSocket);
	if (!source.init()) {
		std::cerr << source.getLastError() << std::endl;
		return 3;
	}
	IOVideoSource::Info info = source.getInformation();

	std::cout << "Supported geometry are: "<<std::endl;
	for (int i=0;i<info.providerInfo.supportedGeometries.size();i++) {
		std::cout << "(" << info.providerInfo.supportedGeometries[i].toString() << ")" << std::endl;
	}

	std::cout << "Supported codecs are: "<<std::endl;
	for (int i=0;i<info.providerInfo.supportedCodecs.size();i++) {
		std::cout << "(" << info.providerInfo.supportedCodecs[i].toString() << ")" << std::endl;
	}

	/*
	VideoFormat fmt( info.providerInfo.supportedCodecs[0],info.providerInfo.supportedGeometries[0]);
	if (!source.setFormat(fmt)) {
		std::cout << "Error is "<<source.getLastError() << std::endl;
	}
	*/

	if (!source.startCapture()) {
		std::cerr << source.getLastError() << std::endl;
		return 3;
	}

	for (int i=0; i<20; i++) {
		VideoSource::Buffer buf = source.getFilledBuffer();
		if (!buf.isValid()) {
			std::cerr << "Buffer is not valid" << std::endl;
			return 4;
		}

		//std::cout << "Get "<<buf.size<< " bytes data"<<std::endl;
		source.putBuffer(buf);
	}

	source.stopCapture();


	return 0;
}
