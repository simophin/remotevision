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
#include "RString.h"

class VideoInfo: public Serializable {
public:
	VideoCodec currentCodec;
	Geometry   currentGeometry;
	FrameRate  currentFrameRate;

	virtual bool isValid() const{
		return (currentCodec.isValid() && currentGeometry.isValid());
	}

	static VideoInfo fromString(const String &);
	virtual String toString() const;
};

#endif /* VIDEOINFO_H_ */
