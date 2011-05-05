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

public:
	static std::string getStringFromId (VideoCodecId);
	static VideoCodecId getIdFromString(const std::string &);
};

#endif /* VIDEOCODEC_H_ */
