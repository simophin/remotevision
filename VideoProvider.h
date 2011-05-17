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
#include "ImageBuffer.h"

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
	virtual bool startCapture() = 0;
	virtual bool stopCapture() = 0;
	virtual size_t getData(unsigned char *, size_t) = 0;
	size_t getData(ImageBuffer &);

	static VideoProvider * getInstance();
};

#endif /* VIDEOPROVIDER_H_ */
