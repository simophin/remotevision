/*
 * FFMpegInfo.cpp
 *
 *  Created on: 2011-5-24
 *      Author: simophin
 */

#include "FFMpegInfo.h"
#include "Utils.h"

#include <vector>

static std::vector<FFMpegCodecInfo> FFMPEG_CODEC_INFO;


struct ID_CONV_TABLE_T {
	VideoCodecId rid;
	CodecID fid;
};

struct PIX_CONV_TABLE_T {
	ImageFormat rfmt;
	PixelFormat ffmt;
};


static ID_CONV_TABLE_T ID_CONV_TABLE[] = {
		{ VCODEC_INVALID, CODEC_ID_NONE },
		{ VCODEC_FLV, 	CODEC_ID_FLV1 },
		{ VCODEC_MJPEG, CODEC_ID_MJPEG },
		{ VCODEC_MPEG4, CODEC_ID_MPEG4 },
		{ VCODEC_RAW,	CODEC_ID_RAWVIDEO },
		{ VCODEC_HUFFYUV, CODEC_ID_HUFFYUV },
};

static PIX_CONV_TABLE_T PIX_CONV_TABLE[] = {
		{ IF_INVALID, PIX_FMT_NONE },
		{ IF_YUV420P, PIX_FMT_YUV420P },
		{ IF_RGB565,  PIX_FMT_RGB565},
		{ IF_RGB888,	PIX_FMT_RGB24 },
		{ IF_YUV422P, PIX_FMT_YUV422P },
};

VideoCodecId FFMpegInfo::
getIdFromFFMpeg(CodecID id) {
	for (int i=0; i<ARRAY_SIZE(ID_CONV_TABLE); i++) {
		if (ID_CONV_TABLE[i].fid == id) {
			return ID_CONV_TABLE[i].rid;
		}
	}
	return VCODEC_INVALID;
}

CodecID FFMpegInfo::
getIdFromRemoteVision(VideoCodecId id) {
	for (int i=0; i<ARRAY_SIZE(ID_CONV_TABLE); i++) {
		if (ID_CONV_TABLE[i].rid == id) {
			return ID_CONV_TABLE[i].fid;
		}
	}
	return CODEC_ID_NONE;
}

PixelFormat FFMpegInfo::getPixFmtFromRemoteVision(ImageFormat fmt)
{
	for (int i=0; i<ARRAY_SIZE(PIX_CONV_TABLE); i++) {
		if (PIX_CONV_TABLE[i].rfmt == fmt) {
			return PIX_CONV_TABLE[i].ffmt;
		}
	}
	return PIX_FMT_NONE;
}


FFMpegCodecInfo FFMpegInfo::findCodecInfo(CodecID id)
{
	std::vector<FFMpegCodecInfo>::iterator i;
	for (i=FFMPEG_CODEC_INFO.begin();i != FFMPEG_CODEC_INFO.end(); ++i) {
		if (i->codecId == id) {
			return *i;
		}
	}
	return FFMpegCodecInfo();
}

FFMpegCodecInfo FFMpegInfo::findCodecInfo(const VideoCodec & codec)
{
	return findCodecInfo(getIdFromRemoteVision(codec.codecId));
}

ImageFormat FFMpegInfo::
getPixFmtFromFFMpeg(PixelFormat fmt)
{
	for (int i=0; i<ARRAY_SIZE(PIX_CONV_TABLE); i++) {
		if (PIX_CONV_TABLE[i].ffmt == fmt) {
			return PIX_CONV_TABLE[i].rfmt;
		}
	}
	return IF_INVALID;
}

void FFMpegInfo::init()
{
	// Default geometries
	std::vector<Geometry> defaultGeometries;
	defaultGeometries.push_back(Geometry(320,240));
	defaultGeometries.push_back(Geometry(640,480));
	defaultGeometries.push_back(Geometry(1024,768));

	// Default rational
	std::vector<AVRational> defaultRationals;
	defaultRationals.push_back((AVRational){1,15});
	defaultRationals.push_back((AVRational){1,25});
	defaultRationals.push_back((AVRational){1,30});

	// For huffyuv
	{
		FFMpegCodecInfo info;
		info.codecId = CODEC_ID_HUFFYUV;
		info.pixelFormat = PIX_FMT_YUV422P;
		info.supportedGeometries = defaultGeometries;
		info.supportedPixelFormat.push_back(info.pixelFormat);
		info.supportedRationals = defaultRationals;
		FFMPEG_CODEC_INFO.push_back(info);
	}

	// For mpeg4
	{
		FFMpegCodecInfo info;
		info.codecId = CODEC_ID_MPEG4;
		info.pixelFormat = PIX_FMT_YUV420P;
		info.supportedGeometries = defaultGeometries;
		info.supportedPixelFormat.push_back(info.pixelFormat);
		info.supportedRationals = defaultRationals;
		FFMPEG_CODEC_INFO.push_back(info);
	}
}



