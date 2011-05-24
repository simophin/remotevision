/*
 * VideoSource.h
 *
 *  Created on: 2011-5-13
 *      Author: simophin
 */

#ifndef VIDEOSOURCE_H_
#define VIDEOSOURCE_H_

#include <map>
#include <string>

#include "IODevice.h"
#include "VideoInfo.h"
#include "VideoProvider.h"

class ImageBuffer;
class VideoFormat;
class VideoSource: public IODevice {
public:
	VideoSource();
	virtual ~VideoSource();

	class Info {
	public:
		VideoInfo info;
		VideoProvider::Info providerInfo;
	};

	typedef std::map<std::string,std::string> Option;

	bool init (const Option &options = Option(), int ms = -1);
	Info getInformation(int ms = -1) const;
	VideoFormat getFormat(int ms = -1) const;
	bool setFormat(VideoFormat &, int ms = -1);
	bool startCapture(int ms = -1);
	bool stopCapture(int ms = -1);
	void putBuffer(const ImageBuffer &, int ms = -1);
	ImageBuffer getFilledBuffer (int ms = -1);

	class Impl;
protected:
	virtual ssize_t doRead (char *, size_t);
	virtual ssize_t doWrite (const char *, size_t);
	virtual int doPoll(PollType, int);
	virtual void doClose();
	virtual Error doGetLastError();
	void setLastError(const Error &);

	virtual bool doInit (const Option &options, int ms) = 0;
	virtual Info doGetInformation(int ms) const = 0;
	virtual VideoFormat doGetFormat(int ms) const = 0;
	virtual bool doSetFormat(VideoFormat &,int ms) = 0;
	virtual bool doStartCapture(int ms) = 0;
	virtual bool doStopCapture(int ms) = 0;
	virtual void doPutBuffer(const ImageBuffer &, int ms) = 0;
	virtual ImageBuffer doGetFilledBuffer (int ms) = 0;
private:
	Impl *d;
};

#endif /* VIDEOSOURCE_H_ */
