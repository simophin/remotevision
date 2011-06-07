/*
 * Geometry.cpp
 *
 *  Created on: 2011-5-14
 *      Author: simophin
 */

#include "Geometry.h"


#include "RString.h"
#include <vector>
#include "utils/String.hpp"

Geometry Geometry::
fromString (const String & str) {
	Geometry ret;

	std::vector<String> gargs;
	gargs = Utils::split< std::vector<String> >(str,',');

	if (gargs.size() != 2) return ret;

	ret.width = Utils::stringToInteger(gargs.at(0));
	ret.height = Utils::stringToInteger(gargs.at(1));

	return ret;
}

String Geometry::
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


