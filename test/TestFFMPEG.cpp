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
	FFMpegVideoProvider provider("/dev/video0");

	Error ec = provider.initDevice();

	if (ec.isError()) {
		std::cerr << ec << std::endl;
		return 1;
	}

	return 0;
}


