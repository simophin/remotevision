/*
 * VideoCodec.h
 *
 *  Created on: 2011-5-4
 *      Author: simophin
 */

#ifndef VIDEOCODEC_H_
#define VIDEOCODEC_H_

#include "codecs/VideoCodecId.h"
#include "PixelFormat.h"

class VideoCodec {
public:
	VideoCodecId codecId;
	PixelFormat pixelFormat;
};

#endif /* VIDEOCODEC_H_ */
