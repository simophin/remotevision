/*
 * Geometry.h
 *
 *  Created on: 2011-5-4
 *      Author: simophin
 */

#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include <string>

class Geometry {
public:
	int width, height;

	Geometry(int w = -1, int h = -1)
	:width(w), height(h) {}

	static Geometry fromString (const std::string &);
	std::string toString () const;
	bool isValid() const;
	void invalid();
};

#endif /* GEOMETRY_H_ */
