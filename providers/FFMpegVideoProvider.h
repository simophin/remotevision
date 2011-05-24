/*
 * FFMpegVideoProvider.h
 *
 *  Created on: 2011-5-4
 *      Author: simophin
 */

#ifndef FFMPEGVIDEOPROVIDER_H_
#define FFMPEGVIDEOPROVIDER_H_

#include "VideoProvider.h"
#include "VideoCodec.h"

extern "C" {
#include <libavcodec/avcodec.h>
}

class FFMpegVideoProvider: public VideoProvider {
public:
	FFMpegVideoProvider(const std::string &filename);
	virtual ~FFMpegVideoProvider();

	/*
	virtual VideoInfo queryInfo() const;
	virtual Error getLastError() const;
	virtual bool setParam (const Param &);
	virtual bool startCapture();
	virtual bool stopCapture();
	virtual size_t getData(unsigned char *, size_t);
	virtual Param getParam() const;\
	*/

protected:
	virtual void doInitDevice();
	virtual VideoProvider::Info doQueryInfo() const;
	virtual Param doGetParam() const;
	virtual bool doSetParam(const Param &);
	virtual Error doGetLastError() const;
	virtual bool doStartCapture();
	virtual bool doStopCapture();
	virtual size_t doGetData(unsigned char *, size_t);

	class Impl;
private:
	void init();
	Impl *d;

	typedef enum {
		STATE_READY,
		STATE_CAPTURING,
	} State;


public:

};

#endif /* FFMPEGVIDEOPROVIDER_H_ */
