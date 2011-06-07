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
#include "Serializable.hpp"

class VideoCodec: public Serializable {
public:
	VideoCodecId codecId;

	VideoCodec (VideoCodecId id = VCODEC_INVALID)
	:codecId(id) {}

	inline bool isValid() const {
		return codecId != VCODEC_INVALID;
	}

public:
	static String getStringFromId (VideoCodecId);
	static VideoCodecId getIdFromString(const String &);

	static VideoCodec fromString (const String &);
	virtual String toString() const;

	bool operator==(const VideoCodec &rhs) {
		return (rhs.codecId == codecId);
	}
};

#endif /* VIDEOCODEC_H_ */
