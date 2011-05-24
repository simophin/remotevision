/*
 * VideoProvider.cpp
 *
 *  Created on: 2011-5-4
 *      Author: simophin
 */

#include "VideoProvider.h"
#include "Error.h"
#include "VideoInfo.h"

VideoProvider::VideoProvider(){
}

VideoProvider::~VideoProvider() {
}

void VideoProvider::
initDevice() {

}


VideoProvider::Info VideoProvider::
queryInfo() const
{
	return doQueryInfo();
}

bool VideoProvider::
setVideoCodec(const VideoCodec & codec)
{
	return doSetVideoCodec(codec);
}

bool VideoProvider::
setVideoGeometry(const Geometry & geo)
{
	return doSetVideoGeometry(geo);
}

Error VideoProvider::
getLastError() const
{
	return doGetLastError();
}

bool VideoProvider::
startCapture()
{
	return doStartCapture();
}

bool VideoProvider::
stopCapture()
{
	return doStopCapture();
}

size_t VideoProvider::getData(ImageBuffer &buf) {
	return getData(buf.getData(),buf.getSize());
}

size_t VideoProvider::
getData(unsigned char *data, size_t size)
{
	return getData(data,size);
}



void VideoProvider::doInitDevice()
{
}


