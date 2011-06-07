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

bool  VideoSource::init (const Option &options, int ms){
	return doInit(options,ms);
}
VideoSource::Info  VideoSource::getInformation(int ms) const{
	return doGetInformation(ms);
}
VideoSource::ImageParam  VideoSource::
getImageParam(int ms) const{
	return doGetImageParam(ms);
}
bool  VideoSource::setImageParam(ImageParam & fmt,int ms){
	return doSetImageParam(fmt,ms);
}
bool  VideoSource::startCapture(int ms){
	return doStartCapture(ms);
}
bool  VideoSource::stopCapture(int ms){
	return doStopCapture(ms);
}
void  VideoSource::putBuffer(const Buffer &buf, int ms){
	return doPutBuffer(buf,ms);
}
VideoSource::Buffer
VideoSource::getFilledBuffer (int ms){
	return doGetFilledBuffer(ms);
}
