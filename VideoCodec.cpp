/*
 * VideoCodec.cpp
 *
 *  Created on: 2011-5-14
 *      Author: simophin
 */

#include "VideoCodec.h"

#include <vector>
#include "RString.h"


VideoCodec VideoCodec::
fromString (const String &str) {
	VideoCodec ret;
	ret.codecId = VideoCodec::getIdFromString(str);
	return ret;
}

String VideoCodec::
toString() const {
	return VideoCodec::getStringFromId(codecId);
}
