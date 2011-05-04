/*
 * VideoCodec.h
 *
 *  Created on: 2011-5-4
 *      Author: simophin
 */

#ifndef VIDEOCODEC_H_
#define VIDEOCODEC_H_

#include "codecs/VideoCodecId.h"
#include "ImageFormat.h"

class VideoCodec {
public:
	VideoCodecId codecId;
	ImageFormat pixelFormat;
};

#endif /* VIDEOCODEC_H_ */
