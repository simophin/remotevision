/*
 * Misc.hpp
 *
 *  Created on: 2011-6-7
 *      Author: simophin
 */

#ifndef MISC_HPP_
#define MISC_HPP_


#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x)/sizeof((x)[0]))
#endif

template <typename T>
T MIN (T a, T b) {
	return (a>b) ? b: a;
}

#endif /* MISC_HPP_ */
