/*
 * Utils.h
 *
 *  Created on: 2011-5-24
 *      Author: simophin
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <vector>
#include <string>

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x)/sizeof((x)[0]))
#endif

class Utils {
public:
	template <class T>
	static std::string arrayToString(const T &arr) {
		std::vector<std::string> args;
		typename T::const_iterator i;
		for (i=arr.begin(); i!=arr.end(); ++i) {
			args.push_back(i->toString());
		}
		return boost::join(args,";");
	}

	template <class T>
	static T stringToArray (const std::string &str) {
		T ret;
		std::vector<std::string> args;
		boost::split(args,str,boost::is_any_of(";"));
		for (int i=0;i<args.size();i++) {
			typename T::value_type val = T::value_type::fromString(args.at(i));
			if (val.isValid()) ret.push_back(val);
		}
		return ret;
	}
};

#endif /* UTILS_H_ */
