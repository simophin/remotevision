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
#include "Serializable.hpp"

#include <vector>
#include <string>

class VideoInfo: public Serializable {
public:
	VideoCodec currentCodec;
	Geometry   currentGeometry;
	FrameRate  currentFrameRate;

	virtual bool isValid() const{
		return (currentCodec.isValid() && currentGeometry.isValid());
	}

	static VideoInfo fromString(const std::string &);
	virtual std::string toString() const;
};

#endif /* VIDEOINFO_H_ */
