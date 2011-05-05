/*
 * RemoteVision.h
 *
 *  Created on: 2011-5-5
 *      Author: launliu
 */

#ifndef REMOTEVISION_H_
#define REMOTEVISION_H_

class RemoteVision {
public:
	RemoteVision();
	virtual ~RemoteVision();

	class Impl;
private:
	Impl *d;
};

#endif /* REMOTEVISION_H_ */
