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

#include <vector>
#include <string>

class Error;
class VideoInfo;


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

		virtual std::string toString() const;
		virtual bool isValid() const { return true; }
		static Info fromString(const std::string &);
	};


	bool initDevice();
	Info queryInfo() const;

	Param getParam() const;
	bool setParam(const Param &);

	Error getLastError() const;
	bool startCapture();
	bool stopCapture();

	size_t getData(ImageBuffer &);
	size_t getData(unsigned char *, size_t);

protected:
	virtual bool doInitDevice() = 0;
	virtual Info doQueryInfo() const = 0;

	virtual Param doGetParam() const = 0;
	virtual bool doSetParam(const Param &) = 0;

	virtual Error doGetLastError() const = 0;
	virtual bool doStartCapture() = 0;
	virtual bool doStopCapture() = 0;
	virtual size_t doGetData(unsigned char *, size_t) = 0;
};


#endif /* VIDEOPROVIDER_H_ */
