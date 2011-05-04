/*
 * FFMpegVideoProvider.h
 *
 *  Created on: 2011-5-4
 *      Author: simophin
 */

#ifndef FFMPEGVIDEOPROVIDER_H_
#define FFMPEGVIDEOPROVIDER_H_

#include "VideoProvider.h"

class FFMpegVideoProvider: public VideoProvider {
public:
	FFMpegVideoProvider();
	virtual ~FFMpegVideoProvider();

	virtual VideoInfo queryInfo() const = 0;
};

#endif /* FFMPEGVIDEOPROVIDER_H_ */
