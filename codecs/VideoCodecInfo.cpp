/*
 * VideoCodecInfo.cpp
 *
 *  Created on: 2011-5-5
 *      Author: launliu
 */

#include "VideoCodec.h"
#include "VideoCodecId.h"

struct IdStrMap {
	VideoCodecId id;
	const char *str;
};

static struct IdStrMap ID_STR_MAPS[] = {
		{ VCODEC_INVALID,		"INVALID" },
		{ VCODEC_FLV,			"FLV" },
		{ VCODEC_MJPEG,		"MJPEG" },
		{ VCODEC_RAW,			"RAW" },
		{ VCODEC_MPEG4,		"MPEG4" },
		{ VCODEC_HUFFYUV,		"HUFFYUV" },
};

const static size_t ID_STR_MAPS_SIZE = sizeof(ID_STR_MAPS)/sizeof(struct IdStrMap);

std::string VideoCodec::
getStringFromId (VideoCodecId id) {
	int found_index = 0;
	for (int i=0; i<ID_STR_MAPS_SIZE; i++) {
		if (id == ID_STR_MAPS[i].id) {
			found_index = i;
			break;
		}
	}
	return std::string(ID_STR_MAPS[found_index].str);
}

VideoCodecId VideoCodec::
getIdFromString(const std::string &str) {
	int found_index = 0;
	for (int i=0; i<ID_STR_MAPS_SIZE; i++) {
		if (str == ID_STR_MAPS[i].str) {
			found_index = i;
			break;
		}
	}

	return ID_STR_MAPS[found_index].id;
}
