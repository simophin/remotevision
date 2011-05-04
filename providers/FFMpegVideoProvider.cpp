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

struct Geometry SUPPORTTED_GEOMETRIES[] = {
		Geometry(320,240)
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
			info->supportedGeometry.push_back(SUPPORTTED_GEOMETRIES[i]);
		}
	}
}

void FFMpegVideoProvider::init() {
	av_register_all();
	avdevice_register_all();
}
