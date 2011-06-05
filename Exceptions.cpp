/*
 * Exceptions.cpp
 *
 *  Created on: 2011-6-5
 *      Author: simophin
 */

#include <stdlib.h>
#include <boost/throw_exception.hpp>

#ifdef BOOST_NO_EXCEPTIONS
namespace boost {
void throw_exception(std::exception const &) {
	exit(1);
}
}

#endif
