#ifndef VIDEOCODECID_H_
#define VIDEOCODECID_H_

#include <string>

typedef enum  {
	VCODEC_INVALID = -1,
	VCODEC_RAW ,
	VCODEC_FLV,
	VCODEC_MJPEG,
	VCODEC_MPEG4,
	VCODEC_HUFFYUV,
} VideoCodecId;


#endif
