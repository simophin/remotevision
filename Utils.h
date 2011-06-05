/*
 * Utils.h
 *
 *  Created on: 2011-5-24
 *      Author: simophin
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>

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
		return join(args,';');
	}

	template <class T>
	static T stringToArray (const std::string &str) {
		T ret;
		std::vector<std::string> args;
		args = split< std::vector<std::string> >(str,';');
		for (int i=0;i<args.size();i++) {
			typename T::value_type val = T::value_type::fromString(args.at(i));
			if (val.isValid()) ret.push_back(val);
		}
		return ret;
	}

	template <class T>
	static T split (const std::string & str, char glue) {
		T ret;
		const char *buf = str.c_str();

		int lastIndex = 0;
		for (int i=0; i<str.size(); i++) {
			if (buf[i] == glue || buf[i] == 0) {
				ret.push_back(std::string (buf+lastIndex, i-lastIndex));
				lastIndex = i+1;
			}
		}
		return ret;
	}

	template <class T>
	static std::string join (const T &array, char glue) {
		std::string ret;
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
	static std::string convertToString (T num) {
		std::stringstream stream;
		stream << num;
		return stream.str();
	}

	static int stringToInteger (const std::string &str) {
		return ::atoi(str.c_str());
	}

	class NonCopyable {
	protected:
		NonCopyable() {}
		~NonCopyable() {}
	private:  // emphasize the following members are private
		NonCopyable( const NonCopyable& );
		const NonCopyable& operator=( const NonCopyable& );

	};
};

#endif /* UTILS_H_ */
