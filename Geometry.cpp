/*
 * Geometry.cpp
 *
 *  Created on: 2011-5-14
 *      Author: simophin
 */

#include "Geometry.h"


#include <string>
#include <vector>
#include "Utils.h"

Geometry Geometry::
fromString (const std::string & str) {
	Geometry ret;

	std::vector<std::string> gargs;
	gargs = Utils::split< std::vector<std::string> >(str,',');

	if (gargs.size() != 2) return ret;

	ret.width = Utils::stringToInteger(gargs.at(0));
	ret.height = Utils::stringToInteger(gargs.at(1));

	return ret;
}

std::string Geometry::
toString () const {
	std::stringstream ret;

	ret << width << "," << height;

	return ret.str();
}

bool Geometry::
isValid() const {
	return (width > 0 && height > 0);
}

void Geometry::
invalid() {
	width = height = -1;
}

bool Geometry::operator ==(const Geometry & rhs) const
{
	return (width == rhs.width && height == rhs.height);
}


