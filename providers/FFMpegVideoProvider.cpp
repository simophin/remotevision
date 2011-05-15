/*
 * FFMpegVideoProvider.cpp
 *
 *  Created on: 2011-5-4
 *      Author: simophin
 */

#include "FFMpegVideoProvider.h"
#include "VideoInfo.h"
#include "VideoCodec.h"
#include "Geometry.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
#include <libavdevice/avdevice.h>
}

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof((x))/sizeof((x[0])))
#endif

struct ID_CONV_TABLE_T {
	VideoCodecId rid;
	CodecID fid;
};

struct PIX_CONV_TABLE_T {
	ImageFormat rfmt;
	PixelFormat ffmt;
};

struct s_geometry {
	int width,height;
};

static ID_CONV_TABLE_T ID_CONV_TABLE[] = {
		{ VCODEC_INVALID, CODEC_ID_NONE },
		{ VCODEC_FLV, 	CODEC_ID_FLV1 },
		{ VCODEC_MJPEG, CODEC_ID_MJPEG },
		{ VCODEC_RAW,	CODEC_ID_RAWVIDEO },
};

static PIX_CONV_TABLE_T PIX_CONV_TABLE[] = {
		{ IF_INVALID, PIX_FMT_NONE },
		{ IF_YUV420P, PIX_FMT_YUV420P },
};

struct s_geometry SUPPORTTED_GEOMETRIES[] = {
		{320,240},
};

class FFMpegVideoProvider::Impl {
public:
	VideoInfo *videoInfo;
	~Impl () {
		if (videoInfo) delete videoInfo;
	}

	Impl()
	:videoInfo(0) {}
};

FFMpegVideoProvider::FFMpegVideoProvider()
:d(new FFMpegVideoProvider::Impl){
	init();
}

FFMpegVideoProvider::~FFMpegVideoProvider() {
	delete d;
}

VideoInfo FFMpegVideoProvider::
queryInfo() const {
	if (d->videoInfo == 0) {
		VideoInfo *info = new VideoInfo;
		for (int i=0;i<ARRAY_SIZE(SUPPORTTED_GEOMETRIES);i++) {
			info->supportedGeometry.push_back(Geometry(SUPPORTTED_GEOMETRIES[i].width,
					SUPPORTTED_GEOMETRIES[i].height ));
		}

		AVCodec *codec = NULL;
		while ((codec = av_codec_next(codec)) != NULL) {
			VideoCodec vcodec;
			vcodec.codecId = getIdFromFFMpeg(codec->id);
			if (vcodec.codecId == VCODEC_INVALID) continue;
			vcodec.pixelFormat = getPixFmtFromFFMpeg(PIX_FMT_YUV420P);
			info->supportedVideoCodecs.push_back(vcodec);
		}

		d->videoInfo = info;
	}
	return *d->videoInfo;
}

void FFMpegVideoProvider::init() {
	av_register_all();
	avdevice_register_all();
}

VideoCodecId FFMpegVideoProvider::
getIdFromFFMpeg(CodecID id) {
	for (int i=0; i<ARRAY_SIZE(ID_CONV_TABLE); i++) {
		if (ID_CONV_TABLE[i].fid == id) {
			return ID_CONV_TABLE[i].rid;
		}
	}
	return VCODEC_INVALID;
}

CodecID FFMpegVideoProvider::
getIdFromRemoteVision(VideoCodecId id) {
	for (int i=0; i<ARRAY_SIZE(ID_CONV_TABLE); i++) {
		if (ID_CONV_TABLE[i].rid == id) {
			return ID_CONV_TABLE[i].fid;
		}
	}
	return CODEC_ID_NONE;
}

PixelFormat FFMpegVideoProvider::getPixFmtFromRemoteVision(ImageFormat fmt)
{
	for (int i=0; i<ARRAY_SIZE(PIX_CONV_TABLE); i++) {
		if (PIX_CONV_TABLE[i].rfmt == fmt) {
			return PIX_CONV_TABLE[i].ffmt;
		}
	}
	return PIX_FMT_NONE;
}



ImageFormat FFMpegVideoProvider::
getPixFmtFromFFMpeg(PixelFormat fmt)
{
	for (int i=0; i<ARRAY_SIZE(PIX_CONV_TABLE); i++) {
		if (PIX_CONV_TABLE[i].ffmt == fmt) {
			return PIX_CONV_TABLE[i].rfmt;
		}
	}
	return IF_INVALID;
}


