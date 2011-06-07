/*
 * VideoSource.cpp
 *
 *  Created on: 2011-5-13
 *      Author: simophin
 */

#include "VideoSource.h"
#include "ImageBuffer.h"

class VideoSource::Impl {
public:
	Error lastError;
};

VideoSource::VideoSource()
:d(new VideoSource::Impl){

}

VideoSource::~VideoSource() {
	delete d;
}

Error VideoSource::
doGetLastError(){
	return d->lastError;
}

Error VideoSource::getLastError()
{
	return doGetLastError();
}

void VideoSource::
setLastError(const Error &err) {
	d->lastError = err;
}

Error  VideoSource::init (const Option &options, int ms){
	return doInit(options,ms);
}
Error VideoSource::getInformation(Info &info,int ms) const{
	return doGetInformation(info,ms);
}
Error VideoSource::
getImageParam(Param &p,int ms) const{
	return doGetImageParam(p,ms);
}
Error VideoSource::setImageParam(ImageParam & fmt,int ms){
	return doSetImageParam(fmt,ms);
}
Error VideoSource::startCapture(int ms){
	return doStartCapture(ms);
}
Error VideoSource::stopCapture(int ms){
	return doStopCapture(ms);
}
Error VideoSource::putBuffer(const Buffer &buf, int ms){
	return doPutBuffer(buf,ms);
}
Error
VideoSource::getFilledBuffer (Buffer &buf,int ms){
	return doGetFilledBuffer(buf,ms);
}
