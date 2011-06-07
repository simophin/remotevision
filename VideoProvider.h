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
#include "FrameRate.h"
#include "Serializable.hpp"
#include "Error.h"
#include <vector>
#include "RString.h"
#include "VideoInfo.h"


class VideoProvider {
public:
	VideoProvider();
	virtual ~VideoProvider();

	typedef VideoInfo Param;

	class Info: public Serializable {
	public:
		std::vector<VideoCodec> supportedCodecs;
		std::vector<Geometry>   supportedGeometries;
		std::vector<FrameRate>  supportedFrameRates;

		virtual String toString() const;
		virtual bool isValid() const { return true; }
		static Info fromString(const String &);
	};


	Error initDevice();
	Error queryInfo(Info &) const;

	Error getParam(Param &) const;
	Error setParam(const Param &);

	Error startCapture();
	Error stopCapture();

	Error getData(ImageBuffer &, size_t *size, int ms = -1);
	Error getData(unsigned char *, size_t , size_t *returned, int ms = -1);

protected:
	virtual Error doInitDevice() = 0;
	virtual Error doQueryInfo(Info &) const = 0;

	virtual Error doGetParam(Param &) const = 0;
	virtual Error doSetParam(const Param &) = 0;

	virtual Error doStartCapture() = 0;
	virtual Error doStopCapture() = 0;
	virtual Error doGetData(unsigned char *, size_t, size_t *returned, int ms) = 0;
};


#endif /* VIDEOPROVIDER_H_ */
