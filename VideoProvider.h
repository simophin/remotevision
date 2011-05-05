/*
 * VideoProvider.h
 *
 *  Created on: 2011-5-4
 *      Author: simophin
 */

#ifndef VIDEOPROVIDER_H_
#define VIDEOPROVIDER_H_

class VideoInfo;
class VideoProvider {
public:
	VideoProvider();
	virtual ~VideoProvider();

	virtual void initDevice();
	virtual VideoInfo queryInfo() const = 0;

	static VideoProvider * getInstance();
};

#endif /* VIDEOPROVIDER_H_ */
