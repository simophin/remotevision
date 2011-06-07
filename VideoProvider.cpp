/*
 * VideoProvider.cpp
 *
 *  Created on: 2011-5-4
 *      Author: simophin
 */

#include "VideoProvider.h"
#include "Error.h"
#include "VideoInfo.h"
#include "utils/String.hpp"

#include <sstream>

VideoProvider::VideoProvider(){
}

VideoProvider::~VideoProvider() {
}

Error VideoProvider::
initDevice() {
	return doInitDevice();
}


Error VideoProvider::
queryInfo(VideoProvider::Info &info) const
{
	return doQueryInfo(info);
}


Error VideoProvider::
startCapture()
{
	return doStartCapture();
}

Error VideoProvider::
stopCapture()
{
	return doStopCapture();
}

Error VideoProvider::getData(ImageBuffer &buf, size_t *returned, int ms) {
	return getData(buf.getData(),buf.getSize(), returned, ms);
}

Error VideoProvider::
getData(unsigned char * data, size_t size, size_t *returned, int ms )
{
	return doGetData(data,size, returned,ms);
}



Error VideoProvider::
getParam(Param &p) const
{
	return doGetParam(p);
}

Error VideoProvider::setParam(const Param & param)
{
	return doSetParam(param);
}

Error VideoProvider::doInitDevice()
{
	return Error();
}

String VideoProvider::Info::
toString() const
{
	std::stringstream stream;
	stream << Utils::arrayToString(supportedCodecs) << "|";
	stream << Utils::arrayToString(supportedGeometries) << "|";
	stream << Utils::arrayToString(supportedFrameRates);
	return stream.str();
}



VideoProvider::Info VideoProvider::Info::
fromString(const String & str)
{
	std::vector<String> args;
	args = Utils::split< std::vector<String>  >(str,'|');
	if (args.size() != 3) return VideoProvider::Info();

	VideoProvider::Info ret;

	ret.supportedCodecs = Utils::stringToArray< std::vector<VideoCodec> >(args.at(0));
	ret.supportedGeometries = Utils::stringToArray< std::vector<Geometry> >(args.at(1));
	ret.supportedFrameRates = Utils::stringToArray< std::vector<FrameRate> >(args.at(2));

	return ret;
}




