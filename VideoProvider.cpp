/*
 * VideoProvider.cpp
 *
 *  Created on: 2011-5-4
 *      Author: simophin
 */

#include "VideoProvider.h"

VideoProvider * INSTANCE = 0;

VideoProvider::VideoProvider() {
	INSTANCE = this;
}

VideoProvider::~VideoProvider() {
	INSTANCE = 0;
}

void VideoProvider::
initDevice() {

}

VideoProvider * VideoProvider::
getInstance() {
	return INSTANCE;
}

size_t VideoProvider::getData(ImageBuffer &buf) {
	return getData(buf.getData(),buf.getSize());
}
