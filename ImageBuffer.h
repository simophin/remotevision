/*
 * ImageBuffer.h
 *
 *  Created on: 2011-5-13
 *      Author: simophin
 */

#ifndef IMAGEBUFFER_H_
#define IMAGEBUFFER_H_

#include <sys/types.h>

class ImageBuffer {
public:
	ImageBuffer(unsigned char * data = 0, size_t size = 0)
	:mData(data), mSize(size) {}
	virtual ~ImageBuffer() {}

	inline unsigned char *
	getData() { return mData; }

	inline size_t
	getSize() { return mSize; }

	inline void setData(unsigned char *d, size_t s) { mData = d; mSize = s; }


private:
	unsigned char *mData;
	size_t mSize;
};

#endif /* IMAGEBUFFER_H_ */
