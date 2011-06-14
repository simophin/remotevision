/*
 * TestFFMPEG.cpp
 *
 *  Created on: 2011-5-22
 *      Author: simophin
 */

#include "providers/FFMpegVideoProvider.h"
#include "3rdparty/ffmpeg/FFMpeg.h"
#include "utils/SharedPtr.hpp"
#include <iostream>
#include "Error.h"

void deleter (unsigned char *b) {
	::free(b);
}

int main () {
	FFMpeg::init();
	FFMpegVideoProvider provider("/dev/video0");

	Error ec = provider.initDevice();

	if (ec.isError()) {
		std::cerr << ec << std::endl;
		return 1;
	}

	ec = provider.startCapture();
	if (ec.isError()) {
		std::cerr << ec << std::endl;
		return 1;
	}

	unsigned char *buffer = (unsigned char *)::malloc(1024000);
	Utils::SharedPtr<unsigned char> buf (buffer, deleter);
	size_t got = 0;

	for (int i=0; i<10; i++) {
		ec = provider.getData(buffer, 1024000,&got);
		if (ec.isError()) {
			std::cerr << ec << std::endl;
			return 2;
		}else{
			std::cout << "Got " << got << " bytes data" << std::endl;
		}
	}

	provider.stopCapture();

	return 0;
}


