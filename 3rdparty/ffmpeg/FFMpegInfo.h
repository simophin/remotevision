/*
 * FFMpegInfo.h
 *
 *  Created on: 2011-5-24
 *      Author: simophin
 */

#ifndef FFMPEG_INFO_H_
#define FFMPEG_INFO_H_

#include <vector>
#include "VideoCodec.h"
#include "Geometry.h"
#include "ImageFormat.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
#include <libavdevice/avdevice.h>
}

class FFMpegCodecInfo {
public:
	CodecID codecId;
	PixelFormat pixelFormat;
	std::vector<Geometry> supportedGeometries;
	std::vector<AVRational> supportedRationals;
	std::vector<PixelFormat> supportedPixelFormat;

	FFMpegCodecInfo()
	:codecId(CODEC_ID_NONE),
	 pixelFormat(PIX_FMT_NONE){}
};

class FFMpeg;
class FFMpegInfo {
	friend class FFMpeg;
public:
public:
	static FFMpegCodecInfo findCodecInfo(CodecID);
	static FFMpegCodecInfo findCodecInfo (const VideoCodec &);
	static VideoCodecId getIdFromFFMpeg(CodecID);
	static CodecID getIdFromRemoteVision(VideoCodecId);
	static PixelFormat getPixFmtFromRemoteVision(ImageFormat);
	static ImageFormat getPixFmtFromFFMpeg(PixelFormat);

private:
	static void init();
};


#endif
