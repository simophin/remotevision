/*
 * VideoCodec.cpp
 *
 *  Created on: 2011-5-14
 *      Author: simophin
 */

#include "VideoCodec.h"

#include <vector>
#include <string>


VideoCodec VideoCodec::
fromString (const std::string &str) {
	VideoCodec ret;
	ret.codecId = VideoCodec::getIdFromString(str);
	return ret;
}

std::string VideoCodec::
toString() const {
	return VideoCodec::getStringFromId(codecId);
}
