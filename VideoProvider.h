/*
 * VideoProvider.h
 *
 *  Created on: 2011-5-4
 *      Author: simophin
 */

#ifndef VIDEOPROVIDER_H_
#define VIDEOPROVIDER_H_

#include "VideoCodec.h"
#include "Geometry.h"

class Error;
class VideoInfo;
class VideoProvider {
public:
	VideoProvider();
	virtual ~VideoProvider();

	class Param {
	public:
		VideoCodec codec;
		Geometry   geo;
	};

	virtual void initDevice();
	virtual VideoInfo queryInfo() const = 0;
	virtual bool setParam (const Param &p) = 0;

	virtual Error getLastError() const = 0;

	static VideoProvider * getInstance();
};

#endif /* VIDEOPROVIDER_H_ */
