/*
 * VideoSource.h
 *
 *  Created on: 2011-5-13
 *      Author: simophin
 */

#ifndef VIDEOSOURCE_H_
#define VIDEOSOURCE_H_

#include <map>
#include "RString.h"

#include "IODevice.h"
#include "VideoInfo.h"
#include "VideoProvider.h"
#include "ImageFormat.h"
#include "Geometry.h"
#include "FrameRate.h"


class ImageBuffer;
class Error;
class VideoSource{
public:
	VideoSource();
	virtual ~VideoSource();

	class Info {
	public:
		VideoInfo info;
		VideoProvider::Info providerInfo;
	};

	class ImageParam {
	public:
		ImageFormat pixFmt;
		Geometry    geo;
		FrameRate   fps;
	};

	class Buffer {
		friend class VideoSource;
	public:
		unsigned char * buf;
		size_t size;

		int index;
		void *priv;

		Buffer()
		:index(-1),priv(0),
		 buf(0),size(0) {}

		bool isValid () {
			return (index >= 0);
		}
	};

	typedef std::map<String,String> Option;

	Error init (const Option &options = Option(), int ms = -1);
	Error getInformation(Info &,int ms = -1) const;
	Error getImageParam(ImageParam &,int ms = -1) const;
	Error setImageParam(ImageParam &, int ms = -1);
	Error startCapture(int ms = -1);
	Error stopCapture(int ms = -1);
	Error putBuffer(const Buffer &, int ms = -1);
	Error getFilledBuffer (Buffer &,int ms = -1);

	class Impl;
protected:
	virtual Error doInit (const Option &options, int ms) = 0;
	virtual Error doGetInformation(Info &,int ms) const = 0;
	virtual Error doGetImageParam(ImageParam &,int ms) const = 0;
	virtual Error doSetImageParam(ImageParam &,int ms) = 0;
	virtual Error doStartCapture(int ms) = 0;
	virtual Error doStopCapture(int ms) = 0;
	virtual Error doPutBuffer(const Buffer &, int ms) = 0;
	virtual Error doGetFilledBuffer (Buffer &,int ms) = 0;
private:
	Impl *d;
};

#endif /* VIDEOSOURCE_H_ */
