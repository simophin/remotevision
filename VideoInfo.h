/*
 * VideoInfo.h
 *
 *  Created on: 2011-5-4
 *      Author: simophin
 */

#ifndef VIDEOINFO_H_
#define VIDEOINFO_H_

#include "Geometry.h"
#include "VideoCodec.h"

#include <list>

class VideoInfo {
public:
	VideoInfo();
	virtual ~VideoInfo();

	/* Members */
	std::list<Geometry> supportedGeometry;
	std::list<VideoCodec> supportedVideoCodecs;
};

#endif /* VIDEOINFO_H_ */
