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
	virtual bool doInit (const Option &options, int ms);
	virtual Info doGetInformation(int ms) const;
	virtual ImageParam doGetImageParam(int ms) const;
	virtual bool doSetImageParam(ImageParam &,int ms);
	virtual bool doStartCapture(int ms);
	virtual bool doStopCapture(int ms);
	virtual void doPutBuffer(const Buffer &, int ms);
	virtual Buffer doGetFilledBuffer (int ms);

private:
	Impl *d;
};

#endif /* IOVIDEOSOURCE_H_ */
