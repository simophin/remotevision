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
	FFMpegVideoProvider(const String &filename);
	virtual ~FFMpegVideoProvider();

protected:
	virtual Error doInitDevice();
	virtual Error doQueryInfo(Info &) const;
	virtual Error doGetParam(Param &) const;
	virtual Error doSetParam(const Param &);
	virtual Error doStartCapture();
	virtual Error doStopCapture();
	virtual Error doGetData(unsigned char *, size_t, size_t *returned, int ms);

	class Impl;
private:
	void init();
	Impl *d;

	typedef enum {
		STATE_UNINTIALIZED,
		STATE_READY,
		STATE_CAPTURING
	} State;


public:

};

#endif /* FFMPEGVIDEOPROVIDER_H_ */
