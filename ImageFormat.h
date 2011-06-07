/*
 * ImageFormat.h
 *
 *  Created on: 2011-5-4
 *      Author: simophin
 */

#ifndef IMAGEFORMAT_H_
#define IMAGEFORMAT_H_

typedef enum {
	IF_INVALID = -1,
	IF_YUV420P,
	IF_YUV422P,
	IF_RGB565,
	IF_RGB888
} ImageFormat;

#endif /* IMAGEFORMAT_H_ */
