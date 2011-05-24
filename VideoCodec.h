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

public:
	static std::string getStringFromId (VideoCodecId);
	static VideoCodecId getIdFromString(const std::string &);

	static VideoCodec fromString (const std::string &);
	std::string toString() const;

	bool operator==(const VideoCodec &rhs) {
		return (rhs.codecId == codecId);
	}
};

#endif /* VIDEOCODEC_H_ */
