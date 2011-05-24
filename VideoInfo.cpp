/*
 * VideoInfo.cpp
 *
 *  Created on: 2011-5-24
 *      Author: simophin
 */

#include "VideoInfo.h"

#include <string>
#include <vector>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/lexical_cast.hpp>
#include <assert.h>
#include <algorithm>
#include <sstream>


VideoInfo VideoInfo::fromString(const std::string &str)
{
	VideoInfo ret;
	std::vector<std::string> args;
	boost::split(args,str,boost::is_any_of("|"));
	if (args.size() != 3) { return ret; }
	ret.currentCodec = VideoCodec::fromString(args.at(0));
	ret.currentGeometry = Geometry::fromString(args.at(1));
	ret.currentFrameRate = FrameRate::fromString(args.at(2));

	return ret;
}



std::string VideoInfo::toString() const
{
	std::vector<std::string> args;
	args.push_back(currentCodec.toString());
	args.push_back(currentGeometry.toString());
	args.push_back(currentFrameRate.toString());
	return boost::join(args,"|");
}

