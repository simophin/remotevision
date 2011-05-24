/*
 * FrameRate.cpp
 *
 *  Created on: 2011-5-24
 *      Author: simophin
 */

#include "FrameRate.h"

#include <string>
#include <vector>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/lexical_cast.hpp>
#include <assert.h>
#include <algorithm>
#include <sstream>

std::string FrameRate::toString() const
{
	std::stringstream stream;
	stream << num << "/" << den;
	return stream.str();
}



FrameRate FrameRate::fromString(const std::string & str)
{
	std::vector<std::string> args;
	boost::split(args,str,boost::is_any_of("/"));
	if (args.size() != 2) return FrameRate();
	FrameRate ret;
	ret.num = boost::lexical_cast<int>(args.at(0));
	ret.den = boost::lexical_cast<int>(args.at(1));
	return ret;
}

