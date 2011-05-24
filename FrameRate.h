/*
 * FrameRate.h
 *
 *  Created on: 2011-5-24
 *      Author: simophin
 */

#ifndef FRAMERATE_H_
#define FRAMERATE_H_

#include <string>
#include "Serializable.hpp"

class FrameRate: public Serializable {
public:
	int num,den;

	FrameRate()
	:den(0) { }

	inline bool isValid() const { return den != 0;}
	bool operator== (const FrameRate &rhs) const {
		return (num == rhs.num && den == rhs.den);
	}

	virtual std::string toString () const;
	static FrameRate fromString(const std::string &);
};


#endif /* FRAMERATE_H_ */
