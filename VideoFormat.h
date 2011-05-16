/*
 * VideoFormat.h
 *
 *  Created on: 2011-5-13
 *      Author: simophin
 */

#ifndef VIDEOFORMAT_H_
#define VIDEOFORMAT_H_

#include "VideoCodec.h"
#include "Geometry.h"

class VideoFormat {
public:
	VideoFormat(const VideoCodec &id = VideoCodec(), const Geometry &geo = Geometry())
	:mVideoCodec(id), mGeometry(geo) {}
	virtual ~VideoFormat() {}

	inline VideoCodec getVideoCodec() const { return mVideoCodec; }
	inline Geometry   getGeometry()   const { return mGeometry; }

private:
	VideoCodec mVideoCodec;
	Geometry   mGeometry;

};

#endif /* VIDEOFORMAT_H_ */
