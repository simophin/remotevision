/*
 * IOVideoSource.h
 *
 *  Created on: 2011-5-13
 *      Author: simophin
 */

#ifndef IOVIDEOSOURCE_H_
#define IOVIDEOSOURCE_H_

#include "VideoSource.h"

class IODevice;
class IOVideoSource: public VideoSource {
public:
	IOVideoSource(IODevice *ctrl, IODevice *data);
	virtual ~IOVideoSource();

	class Impl;

	IODevice * getControlDevice () const;
	IODevice * getDataDevice() const;

protected:
	virtual Error doInit (const Option &options, int ms);
	virtual Error doGetInformation(Info &,int ms) const;
	virtual Error doGetImageParam(ImageParam &,int ms) const;
	virtual Error doSetImageParam(ImageParam &,int ms);
	virtual Error doStartCapture(int ms);
	virtual Error doStopCapture(int ms);
	virtual Error doPutBuffer(const Buffer &, int ms);
	virtual Error doGetFilledBuffer (Buffer &,int ms);

private:
	Impl *d;
};

#endif /* IOVIDEOSOURCE_H_ */
