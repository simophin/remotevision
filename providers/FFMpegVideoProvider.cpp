/*
 * FFMpegVideoProvider.cpp
 *
 *  Created on: 2011-5-4
 *      Author: simophin
 */

#include "FFMpegVideoProvider.h"
#include "VideoInfo.h"
#include "VideoCodec.h"
#include "Geometry.h"
#include "Error.h"
#include "Thread.h"
#include "ImageBuffer.h"
#include "3rdparty/ffmpeg/FFMpegInfo.h"
#include "utils/String.hpp"
#include "utils/Misc.hpp"

#include <algorithm>
#include <list>
#include <assert.h>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
#include <libavdevice/avdevice.h>
}

class VBuffer {
public:
	ImageBuffer buf;
	Condition   cond;
	Mutex       mutex;
	size_t      size;
	Error       returned;
};

static VideoCodecId SUPPORTED_CODECS[] = {
		VCODEC_HUFFYUV,
		VCODEC_MPEG4,
		VCODEC_MJPEG,
		VCODEC_RAW
};

static const char *TRY_INPUT_FORMATS[] = {
		"video4linux2",
		"video4linux",
		"vfwcap",
		NULL // indicate that it's a file
};

class FFMpegVideoProvider::Impl: public Thread {
public:
	VideoInfo *mVideoInfo;
	State mState;

	String mFileName;

	Param mCurrentParam;

	struct ffmpeg_context {

	} mCtx;

	/*
	struct ffmpeg_context{
		AVFormatContext * inputFmtCtx;
		AVCodecContext  * inputCodecCtx, * outputCodecCtx;
		AVCodec * inputCodec, * outputCodec;
		SwsContext *swsCtx;
		AVFrame * convertedFrame;
	} mCtx;

	Error setOutputCodec (CodecID c, int w,int h, const AVRational &);
	Error updateSws ();
	*/

	// Buffer related
	std::list<VBuffer *> mBuffers;
	Condition mBufferCond;
	Mutex mBufferMutex;

	// Preview buffer
	std::list<VBuffer *> mPreviewBuffers;
	Condition mPreviewBufferCond;
	Mutex mPreviewBufferMutex;

	// Thread entry
	virtual Error entry();

	virtual ~Impl () {
		if (mVideoInfo) delete mVideoInfo;
	}

	Impl(const String &f)
	:mVideoInfo(0),mState(STATE_UNINTIALIZED),
	 mFileName(f){

	}
};

FFMpegVideoProvider::FFMpegVideoProvider(const String &filename)
:d(new FFMpegVideoProvider::Impl(filename)){
	init();
}

FFMpegVideoProvider::~FFMpegVideoProvider() {
	delete d;
}

Error FFMpegVideoProvider::doInitDevice()
{
	Error rc;
	if (d->mState != STATE_UNINTIALIZED) {
		rc.setErrorType(Error::ERR_STATE);
		return rc;
	}

	// Try to open file
	{
		AVInputFormat *fmt = 0;
		AVFormatContext *fmtCtx = 0;

		AVFormatParameters ap;
		::memset(&ap,0,sizeof(ap));


		for (int i=0;i<ARRAY_SIZE(TRY_INPUT_FORMATS); i++) {
			// Find input format
			if (TRY_INPUT_FORMATS[i] != NULL) {
				fmt = av_find_input_format(TRY_INPUT_FORMATS[i]);
				if (fmt == NULL) continue;
			}

			if (av_open_input_file(&fmtCtx, d->mFileName.c_str(),fmt,0,&ap) != 0){
				fmtCtx = NULL;
				continue;
			}else break;
		}

		if (fmtCtx == NULL) {
			rc.setErrorString("Can't open the specified file");
			return rc;
		}

		//::memcpy(&d->mCtx.ap, &ap, sizeof(ap));
		av_close_input_file(fmtCtx);
		return rc;
	}

	return rc;
}

Error FFMpegVideoProvider::doQueryInfo(Info &info) const
{
	Error rc;

	FFMpegCodecInfo cf = FFMpegInfo::findCodecInfo(d->mCurrentParam.currentCodec);
	if (cf.codecId == CODEC_ID_NONE) {
		rc.setErrorString("Can't find current encoder");
		return rc;
	}


	for (unsigned i=0;i<ARRAY_SIZE(SUPPORTED_CODECS);i++) {
		VideoCodec c(SUPPORTED_CODECS[i]);
		info.supportedCodecs.push_back(c);
	}
	info.supportedGeometries = cf.supportedGeometries;

	return rc;
}


Error FFMpegVideoProvider::doStartCapture()
{
	Error rc;
	if (d->mState != STATE_READY || d->isRunning()) {
		rc.setErrorType(Error::ERR_STATE);
		return rc;
	}

	// Some init work
	{
		d->mBufferMutex.unlock();
		d->mBuffers.clear();
	}

	d->run();
	return rc;
}

Error FFMpegVideoProvider::doStopCapture()
{
	d->stop(2000);
	return Error();
}

Error FFMpegVideoProvider::doGetData(unsigned char *data, size_t size, size_t *returned, int ms)
{
	Error rc;

	if (!d->isRunning()) {
		rc.setErrorType(Error::ERR_STATE);
		return rc;
	}

	VBuffer buf;
	buf.buf.setData(data,size);

	rc = d->mBufferMutex.lock(ms);
	if (!rc.isSuccess()) return rc;

	d->mBuffers.push_back(&buf);
	d->mBufferMutex.unlock();

	buf.mutex.lock();

	rc = buf.cond.wait(buf.mutex, ms);

	if (!rc.isSuccess()) {
		return rc;
	}
	buf.mutex.unlock();

	*returned = buf.size;

	return buf.returned;
}

Error FFMpegVideoProvider::doGetParam(Param &p) const
{
	p = d->mCurrentParam;
	return Error();
}

Error FFMpegVideoProvider::doSetParam(const Param & param)
{
	Error ret;
	if (d->mState == STATE_CAPTURING) {
		ret.setErrorType(Error::ERR_STATE);
		return ret;
	}

	/*

	CodecID c = FFMpegInfo::getIdFromRemoteVision(param.currentCodec.codecId);
	ret = d->setOutputCodec(c,param.currentGeometry.width,
			param.currentGeometry.height, (AVRational){param.currentFrameRate.den,param.currentFrameRate.num});

	if (!ret.isSuccess()) {
		return ret;
	}

	ret = d->updateSws();
	if (!ret.isSuccess()) {
		return ret;
	}

	return ret;
	*/
}

Error FFMpegVideoProvider::doGetPreviewImage(PreviewImageBuffer & buf, int ms)
{
	Error rc;

	if (d->mState != STATE_CAPTURING) {
		rc.setErrorType(Error::ERR_STATE);
		return rc;
	}

	/*

	VBuffer vbuf;
	vbuf.buf = buf.buf;

	rc = d->mPreviewBufferMutex.lock(ms);
	if (rc.isError()) return rc;

	d->mPreviewBuffers.push_back(&vbuf);
	d->mPreviewBufferMutex.unlock();

	vbuf.mutex.lock();
	vbuf.cond.wait(vbuf.mutex);

	buf.geo.height = d->mCtx.inputCodecCtx->height;
	buf.geo.width  = d->mCtx.inputCodecCtx->width;
	buf.usedSize = vbuf.size;
	buf.pixFmt = FFMpegInfo::getPixFmtFromFFMpeg(d->mCtx.inputCodecCtx->pix_fmt);
	*/


	return rc;
}

void FFMpegVideoProvider::init() {
	d->mCurrentParam.currentCodec = VCODEC_MPEG4;
	FFMpegCodecInfo cf = FFMpegInfo::findCodecInfo(d->mCurrentParam.currentCodec);
	assert (cf.codecId != CODEC_ID_NONE);
	d->mCurrentParam.currentGeometry = cf.supportedGeometries[0];
	d->mCurrentParam.currentFrameRate.num = cf.supportedRationals[2].num;
	d->mCurrentParam.currentFrameRate.den = cf.supportedRationals[2].den;
}




Error FFMpegVideoProvider::Impl::entry() {
	mState = STATE_CAPTURING;
	Error rc;


	mState = STATE_READY;
	return rc;
}
