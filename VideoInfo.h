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

#include <vector>

class VideoInfo {
public:
	/* Members */
	std::vector<Geometry> supportedGeometry;
	std::vector<VideoCodec> supportedVideoCodecs;
};

#endif /* VIDEOINFO_H_ */
