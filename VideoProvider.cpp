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

bool VideoProvider::
initDevice() {
	return doInitDevice();
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
	return doGetData(data,size);
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

bool VideoProvider::doInitDevice()
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
	std::vector<std::string> args;
	boost::split(args,str,boost::is_any_of("|"));
	if (args.size() != 3) return VideoProvider::Info();

	VideoProvider::Info ret;

	ret.supportedCodecs = Utils::stringToArray< std::vector<VideoCodec> >(args.at(0));
	ret.supportedGeometries = Utils::stringToArray< std::vector<Geometry> >(args.at(1));
	ret.supportedFrameRates = Utils::stringToArray< std::vector<FrameRate> >(args.at(2));

	return ret;
}




