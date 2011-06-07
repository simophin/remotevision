/*
 * VideoInfo.cpp
 *
 *  Created on: 2011-5-24
 *      Author: simophin
 */

#include "VideoInfo.h"
#include "Utils.h"

#include "RString.h"
#include <vector>
#include <assert.h>
#include <algorithm>
#include <sstream>


VideoInfo VideoInfo::fromString(const String &str)
{
	VideoInfo ret;
	std::vector<String> args;
	args = Utils::split< std::vector<String> >(str,'|');
	if (args.size() != 3) { return ret; }
	ret.currentCodec = VideoCodec::fromString(args.at(0));
	ret.currentGeometry = Geometry::fromString(args.at(1));
	ret.currentFrameRate = FrameRate::fromString(args.at(2));

	return ret;
}



String VideoInfo::toString() const
{
	std::vector<String> args;
	args.push_back(currentCodec.toString());
	args.push_back(currentGeometry.toString());
	args.push_back(currentFrameRate.toString());
	return Utils::join(args,'|');
}

