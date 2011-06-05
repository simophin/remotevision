/*
 * FrameRate.cpp
 *
 *  Created on: 2011-5-24
 *      Author: simophin
 */

#include "FrameRate.h"

#include <string>
#include <vector>
#include <assert.h>
#include <algorithm>
#include <sstream>
#include "Utils.h"

std::string FrameRate::toString() const
{
	std::stringstream stream;
	stream << num << "/" << den;
	return stream.str();
}



FrameRate FrameRate::fromString(const std::string & str)
{
	std::vector<std::string> args;
	args = Utils::split< std::vector<std::string>  >(str,'/');
	if (args.size() != 2) return FrameRate();
	FrameRate ret;
	ret.num = Utils::stringToInteger(args.at(0));
	ret.den = Utils::stringToInteger(args.at(1));
	return ret;
}

