/*
 * Geometry.cpp
 *
 *  Created on: 2011-5-14
 *      Author: simophin
 */

#include "Geometry.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/cast.hpp>
#include <vector>
#include <string>

Geometry Geometry::
fromString (const std::string & str) {
	Geometry ret;

	std::vector<std::string> gargs;
	boost::algorithm::split(gargs,str,boost::is_any_of(","));

	if (gargs.size() != 2) return ret;

	try {
		ret.width = boost::lexical_cast<int>(gargs.at(0));
		ret.height = boost::lexical_cast<int>(gargs.at(1));
	}catch(boost::bad_lexical_cast &e) {
		return Geometry ();
	}

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
