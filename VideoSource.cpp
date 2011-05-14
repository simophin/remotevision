/*
 * VideoSource.cpp
 *
 *  Created on: 2011-5-13
 *      Author: simophin
 */

#include "VideoSource.h"
#include "ImageBuffer.h"
#include "VideoFormat.h"

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

ssize_t  VideoSource::
doRead (char *, size_t){
	//TODO: implement read
}
ssize_t VideoSource::
doWrite (const char *, size_t){
	//TODO: implement write
}

int VideoSource::
doPoll(PollType, int){
	//TODO: implement poll
}

void VideoSource::doClose(){
	//TODO: implement close
}


Error VideoSource::
doGetLastError(){
	return d->lastError;
}

void VideoSource::
setLastError(const Error &err) {
	d->lastError = err;
}

bool  VideoSource::init (const Option &options, int ms){
	return doInit(options,ms);
}
VideoInfo  VideoSource::getInformation(int ms) const{
	return doGetInformation(ms);
}
VideoFormat  VideoSource::getFormat(int ms) const{
	return doGetFormat(ms);
}
bool  VideoSource::setFormat(VideoFormat & fmt,int ms){
	return doSetFormat(fmt,ms);
}
bool  VideoSource::startCapture(int ms){
	return doStartCapture(ms);
}
bool  VideoSource::stopCapture(int ms){
	return doStopCapture(ms);
}
void  VideoSource::putBuffer(const ImageBuffer &buf, int ms){
	return doPutBuffer(buf,ms);
}
ImageBuffer  VideoSource::getFilledBuffer (int ms){
	return doGetFilledBuffer(ms);
}
