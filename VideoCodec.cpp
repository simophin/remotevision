/*
 * VideoCodec.cpp
 *
 *  Created on: 2011-5-14
 *      Author: simophin
 */

#include "VideoCodec.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/cast.hpp>
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
