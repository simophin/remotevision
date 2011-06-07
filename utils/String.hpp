/*
 * String.hpp
 *
 *  Created on: 2011-6-7
 *      Author: simophin
 */

#ifndef UTIL_STRING_HPP_
#define UTIL_STRING_HPP_

#include "RString.h"
#include <stdlib.h>
#include <vector>
#include <sstream>

namespace Utils {


template <class T>
static T split (const String & str, char glue) {
	T ret;
	const char *buf = str.c_str();
	int total_length = str.size() + 1;
	int lastIndex = 0;


	for (int i=0; i< total_length; i++) {
		if ( (buf[i] == glue) || (buf[i] == 0) ) {
			int length = i - lastIndex;
			String ps(buf+lastIndex, length);
			ret.push_back(ps);
			lastIndex = i+1;
		}
	}
	return ret;
}

template <class T>
static String join (const T &array, char glue) {
	String ret;
	typename T::const_iterator iter;
	for (iter = array.begin() ; iter != array.end(); ++iter) {
		ret += *iter;
		if (iter != array.end() - 1) {
			ret += glue;
		}
	}
	return ret;
}

template <class T>
static String convertToString (T num) {
	std::stringstream stream;
	stream << num;
	return stream.str();
}

static int stringToInteger (const String &str) {
	return ::atoi(str.c_str());
}


template <class T>
static String arrayToString(const T &arr) {
	std::vector<String> args;
	typename T::const_iterator i;
	for (i=arr.begin(); i!=arr.end(); ++i) {
		args.push_back(i->toString());
	}
	return join(args,';');
}

template <class T>
static T stringToArray (const String &str) {
	T ret;
	std::vector<String> args;
	args = split< std::vector<String> >(str,';');
	for (int i=0;i<args.size();i++) {
		typename T::value_type val = T::value_type::fromString(args.at(i));
		if (val.isValid()) ret.push_back(val);
	}
	return ret;
}

}

#endif /* UTIL_STRING_HPP_ */
