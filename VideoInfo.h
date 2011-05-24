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
#include "FrameRate.h"

#include <vector>

class VideoInfo {
public:
	VideoCodec currentCodec;
	Geometry   currentGeometry;
	FrameRate  currentFrameRate;
};

#endif /* VIDEOINFO_H_ */
