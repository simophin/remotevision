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
	FFMpegVideoProvider();
	virtual ~FFMpegVideoProvider();

	virtual VideoInfo queryInfo() const;
	virtual Error getLastError() const;
	virtual bool setParam (const Param &);

	class Impl;
private:
	void init();
	Impl *d;

	typedef enum {
		STATE_READY,
		STATE_CAPTURING,
	} State;


public:
	static VideoCodecId getIdFromFFMpeg(CodecID);
	static CodecID getIdFromRemoteVision(VideoCodecId);
	static PixelFormat getPixFmtFromRemoteVision(ImageFormat);
	static ImageFormat getPixFmtFromFFMpeg(PixelFormat);
};

#endif /* FFMPEGVIDEOPROVIDER_H_ */
