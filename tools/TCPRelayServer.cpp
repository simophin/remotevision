/*
 * TCPRelayServer.cpp
 *
 *  Created on: 2011-6-9
 *      Author: simophin
 */


#include "medium/TCPFFMpegRelayServer.h"
#include "Error.h"

#include <iostream>

int main () {
	TCPFFMpegRelayServer relayServer ("0.0.0.0");

	Error ret = relayServer.init();
	if (ret.isError()) {
		std::cerr << ret << std::endl;
		return 1;
	}

	ret = relayServer.start();

	if (ret.isError()) {
		std::cerr << ret << std::endl;
		return 2;
	}

	ret = relayServer.wait();
	if (ret.isError()) {
		std::cerr << ret << std::endl;
		return 3;
	}


	return 0;
}

