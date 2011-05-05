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

	virtual VideoInfo queryInfo() const = 0;

	class Impl;
private:
	void init();
	Impl *d;


public:
	static VideoCodecId getIdFromFFMpeg(CodecID);
	static CodecID getIdFromRemoteVision(VideoCodecId);
	static PixelFormat getPixFmtFromRemoteVision(ImageFormat);
	static ImageFormat getPixFmtFromFFMpeg(PixelFormat);
};

#endif /* FFMPEGVIDEOPROVIDER_H_ */
