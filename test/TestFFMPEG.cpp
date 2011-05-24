/*
 * TestFFMPEG.cpp
 *
 *  Created on: 2011-5-22
 *      Author: simophin
 */

#include "providers/FFMpegVideoProvider.h"
#include "3rdparty/ffmpeg/FFMpeg.h"
#include <iostream>
#include "Error.h"

int main () {
	FFMpeg::init();
	FFMpegVideoProvider provider("/home/simophin/movie.rmvb");


	if (!provider.startCapture()) {
		std::cerr << provider.getLastError() << std::endl;
		return 1;
	}

	//unsigned char buf[1024000];
	unsigned char *buf = (unsigned char *)::malloc(1024000);

	int i=0;
	while (i++ < 10000){
		size_t data_get = provider.getData(buf,1024000);
		std::cout << "Got " << data_get << " bytes data" << std::endl;
	}

	return 0;
}


