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

	typedef VideoInfo Info;

	void initDevice();
	Info queryInfo() const;

	bool setVideoCodec (const VideoCodec &);
	bool setVideoGeometry (const Geometry &);

	Error getLastError() const;
	bool startCapture();
	bool stopCapture();

	size_t getData(ImageBuffer &);
	size_t getData(unsigned char *, size_t);

protected:
	virtual void doInitDevice();
	virtual VideoProvider::Info doQueryInfo() const = 0;

	virtual bool doSetVideoCodec (const VideoCodec &) = 0;
	virtual bool doSetVideoGeometry (const Geometry &) = 0;

	virtual Error doGetLastError() const = 0;
	virtual bool doStartCapture() = 0;
	virtual bool doStopCapture() = 0;
	virtual size_t doGetData(unsigned char *, size_t) = 0;
};


#endif /* VIDEOPROVIDER_H_ */
