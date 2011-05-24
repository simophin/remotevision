/*
 * VideoProvider.cpp
 *
 *  Created on: 2011-5-4
 *      Author: simophin
 */

#include "VideoProvider.h"
#include "Error.h"
#include "VideoInfo.h"
#include "Utils.h"

#include <sstream>

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



VideoProvider::Param VideoProvider::
getParam() const
{
	return doGetParam();
}

bool VideoProvider::setParam(const Param & param)
{
	return doSetParam(param);
}

void VideoProvider::doInitDevice()
{
}

std::string VideoProvider::Info::
toString() const
{
	std::stringstream stream;
	stream << Utils::arrayToString(supportedCodecs) << "|";
	stream << Utils::arrayToString(supportedGeometries) << "|";
	stream << Utils::arrayToString(supportedFrameRates);
	return stream.str();
}



VideoProvider::Info VideoProvider::Info::
fromString(const std::string & str)
{

}




