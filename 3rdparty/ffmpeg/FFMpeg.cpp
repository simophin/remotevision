/*
 * FFMpeg.cpp
 *
 *  Created on: 2011-5-24
 *      Author: simophin
 */

#include "FFMpeg.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
#include <libavdevice/avdevice.h>
}

#include "FFMpegInfo.h"

void FFMpeg::init()
{
	av_register_all();
	avcodec_register_all();
	avdevice_register_all();
	FFMpegInfo::init();
}

