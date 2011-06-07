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

	bool init (const Option &options = Option(), int ms = -1);
	Info getInformation(int ms = -1) const;
	ImageParam getImageParam(int ms = -1) const;
	bool setImageParam(ImageParam &, int ms = -1);
	bool startCapture(int ms = -1);
	bool stopCapture(int ms = -1);
	void putBuffer(const Buffer &, int ms = -1);
	Buffer getFilledBuffer (int ms = -1);
	Error getLastError() ;

	class Impl;
protected:

	virtual Error doGetLastError();
	void setLastError(const Error &);

	virtual bool doInit (const Option &options, int ms) = 0;
	virtual Info doGetInformation(int ms) const = 0;
	virtual ImageParam doGetImageParam(int ms) const = 0;
	virtual bool doSetImageParam(ImageParam &,int ms) = 0;
	virtual bool doStartCapture(int ms) = 0;
	virtual bool doStopCapture(int ms) = 0;
	virtual void doPutBuffer(const Buffer &, int ms) = 0;
	virtual Buffer doGetFilledBuffer (int ms) = 0;
private:
	Impl *d;
};

#endif /* VIDEOSOURCE_H_ */
