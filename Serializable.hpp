/*
 * Serializable.hpp
 *
 *  Created on: 2011-5-24
 *      Author: simophin
 */

#ifndef SERIALIZABLE_HPP_
#define SERIALIZABLE_HPP_

#include <string>

class Serializable {
public:
	virtual std::string toString() const = 0;
	virtual bool isValid() const = 0;
};



#endif /* SERIALIZABLE_HPP_ */
