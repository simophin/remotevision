/*
 * Geometry.h
 *
 *  Created on: 2011-5-4
 *      Author: simophin
 */

#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include <string>
#include "Serializable.hpp"

class Geometry: public Serializable {
public:
	int width, height;

	Geometry(int w = -1, int h = -1)
	:width(w), height(h) {}

	static Geometry fromString (const std::string &);
	virtual std::string toString () const;
	bool isValid() const;
	void invalid();

	bool operator== (const Geometry &rhs) const;
};

#endif /* GEOMETRY_H_ */
