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
#include "Utils.h"

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
};

static VideoCodecId SUPPORTED_CODECS[] = {
		VCODEC_FLV,
		VCODEC_MJPEG,
		VCODEC_RAW
};

class FFMpegVideoProvider::Impl: public Thread {
public:
	VideoInfo *videoInfo;
	State state;
	Error lastError;

	std::string mFileName;

	// Related to ffmpeg decoding/encoding
	Param mCurrentParam;

	struct ffmpeg_context{
		AVFormatContext * inputFmtCtx;
		AVCodecContext  * inputCodecCtx, * outputCodecCtx;
		AVCodec * inputCodec, * outputCodec;
		SwsContext *swsCtx;
		AVFrame * convertedFrame;
	} mCtx;

	bool setOutputCodec (CodecID c, int w,int h, const AVRational &, char *errstr = 0, size_t errsize = 0);
	bool updateSws ();

	// Buffer related
	std::list<VBuffer *> mBuffers;
	Condition mBufferCond;
	Mutex mBufferMutex;

	// Thread entry
	virtual void entry();

	virtual ~Impl () {
		if (videoInfo) delete videoInfo;
		if (mCtx.inputFmtCtx) {
			av_close_input_file(mCtx.inputFmtCtx);
		}
		if (mCtx.inputCodecCtx) {
			avcodec_close(mCtx.inputCodecCtx);
		}
		if (mCtx.outputCodecCtx) {
			avcodec_close(mCtx.outputCodecCtx);
		}
		if (mCtx.swsCtx) {
			sws_freeContext(mCtx.swsCtx);
		}
		if (mCtx.convertedFrame) {
			avpicture_free((AVPicture *)mCtx.convertedFrame);
			av_free(mCtx.convertedFrame);
		}
	}

	Impl(const std::string &f)
	:videoInfo(0),state(STATE_UNINTIALIZED),
	 mFileName(f){
		::memset(&mCtx,0,sizeof(mCtx));
	}
};

FFMpegVideoProvider::FFMpegVideoProvider(const std::string &filename)
:d(new FFMpegVideoProvider::Impl(filename)){
	init();
}

FFMpegVideoProvider::~FFMpegVideoProvider() {
	delete d;
}

bool FFMpegVideoProvider::doInitDevice()
{
	assert (d->state == STATE_UNINTIALIZED);
	int rc = 0;
	char errbuf[1024];

	AVFormatContext * inputFmtCtx = 0;
	AVCodec *decodeCodec = 0, *encodeCodec = 0 ;
	AVCodecContext *decodeCtx = 0, *encodeCtx = 0;
	SwsContext *swsCtx = 0;
	AVFrame * convertedFrame = 0;

	FFMpegCodecInfo info = FFMpegInfo::findCodecInfo(d->mCurrentParam.currentCodec);

	// Setup decoder
	{
		AVInputFormat *fmt = av_find_input_format("video4linux2");
		assert (fmt != NULL);

		if ( (rc=av_open_input_file(&inputFmtCtx, d->mFileName.c_str(),fmt,0,NULL)) != 0){
			av_strerror(rc,errbuf,sizeof(errbuf));
			goto open_input_file_error;
		}

		for (int i=0;i<inputFmtCtx->nb_streams; i++) {
			if (inputFmtCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
				decodeCtx = inputFmtCtx->streams[i]->codec;
				decodeCodec = decodeCtx->codec;
				break;
			}
		}

		if (decodeCtx == NULL) {
			snprintf(errbuf,sizeof(errbuf),"Can't find video track");
			goto find_decoder_error;
		}

		decodeCodec = avcodec_find_decoder(decodeCtx->codec_id);
		if (!decodeCodec) {
			snprintf(errbuf,sizeof(errbuf),"Can't find decoder");
			goto find_decoder_error;
		}


		if (decodeCtx->pix_fmt == PIX_FMT_NONE) {
			decodeCtx->pix_fmt = PIX_FMT_YUVJ420P;
		}
		decodeCtx->height = 320;
		decodeCtx->width  = 240;
		decodeCtx->time_base = (AVRational) {1,30};

		if ((rc = avcodec_open(decodeCtx,decodeCodec)) != 0) {
			av_strerror(rc,errbuf,sizeof(errbuf));
			goto open_decoder_error;
		}

		d->mCtx.inputCodec = decodeCodec;
		d->mCtx.inputCodecCtx = decodeCtx;
		d->mCtx.inputFmtCtx = inputFmtCtx;
	}

	{
		AVRational ral = (AVRational){d->mCurrentParam.currentFrameRate.den,d->mCurrentParam.currentFrameRate.num};
		if(!d->setOutputCodec(info.codecId, d->mCurrentParam.currentGeometry.width,
				d->mCurrentParam.currentGeometry.height, ral,errbuf,sizeof(errbuf))) {
			assert(false);
		}
	}

	{
		if (!d->updateSws()) {
			assert(false);
		}
	}

	d->state = STATE_READY;
	return true;

	open_decoder_error:
	find_decoder_error:
	av_close_input_file(inputFmtCtx);
	open_input_file_error:
	errbuf[sizeof(errbuf)-1] = 0;
	d->lastError.setErrorString(errbuf);
	d->state = STATE_UNINTIALIZED;
	return false;
}

VideoProvider::Info FFMpegVideoProvider::doQueryInfo() const
{
	Info ret;
	FFMpegCodecInfo cf = FFMpegInfo::findCodecInfo(d->mCurrentParam.currentCodec);
	if (cf.codecId == CODEC_ID_NONE) {
		d->lastError = Error("No such codec");
		return ret;
	}


	for (int i=0;i<ARRAY_SIZE(SUPPORTED_CODECS);i++) {
		ret.supportedCodecs.push_back(VideoCodec(SUPPORTED_CODECS[i]));
	}
	ret.supportedGeometries = cf.supportedGeometries;
	//ret.supportedFrameRates = cf.supportedRationals;
	return ret;
}


Error FFMpegVideoProvider::doGetLastError() const
{
	return d->lastError;
}

bool FFMpegVideoProvider::doStartCapture()
{
	if (d->state != STATE_READY || d->isRunning()) {
		d->lastError.setErrorString("State error");
		return false;
	}

	// Some init work
	{
		d->mBufferMutex.unlock();
		d->mBuffers.clear();
	}

	d->run();
	return true;
}

bool FFMpegVideoProvider::doStopCapture()
{
	d->stop(2000);
	return true;
}

size_t FFMpegVideoProvider::doGetData(unsigned char *data, size_t size)
{
	VBuffer buf;
	buf.buf.setData(data,size);
	buf.mutex.lock();

	if (!d->mBufferMutex.lock(2000)) return 0;
	d->mBuffers.push_back(&buf);
	d->mBufferMutex.unlock();

	if (!buf.cond.wait(buf.mutex,2000)) {
		return 0;
	}

	return buf.size;
}

FFMpegVideoProvider::Param FFMpegVideoProvider::doGetParam() const
{
	return d->mCurrentParam;
}

bool FFMpegVideoProvider::doSetParam(const Param & param)
{
	char errbuf[1024];
	if (d->state == STATE_CAPTURING) {
		d->lastError = Error("STATE ERROR");
		return false;
	}

	CodecID c = FFMpegInfo::getIdFromRemoteVision(param.currentCodec.codecId);
	if (!d->setOutputCodec(c,param.currentGeometry.width,
			param.currentGeometry.height, (AVRational){param.currentFrameRate.den,param.currentFrameRate.num},
			errbuf,sizeof(errbuf))) {
		errbuf[sizeof(errbuf)-1] = 0;
		d->lastError.setErrorString(errbuf);
		return false;
	}

	if (!d->updateSws()) {
		d->lastError.setErrorString("Failed to update sws info");
		return false;
	}

	return true;
}

void FFMpegVideoProvider::init() {
	d->mCurrentParam.currentCodec = SUPPORTED_CODECS[0];
	FFMpegCodecInfo cf = FFMpegInfo::findCodecInfo(d->mCurrentParam.currentCodec);
	assert (cf.codecId != CODEC_ID_NONE);
	d->mCurrentParam.currentGeometry = cf.supportedGeometries[0];
	d->mCurrentParam.currentFrameRate.num = cf.supportedRationals[0].num;
	d->mCurrentParam.currentFrameRate.den = cf.supportedRationals[0].den;
}



inline bool FFMpegVideoProvider::Impl::
setOutputCodec(CodecID c, int w, int h, const AVRational & r, char *errstr , size_t errsize )
{
	AVCodec * codec = 0;
	AVCodecContext *codecCtx = 0;
	int rc = 0;

	FFMpegCodecInfo info = FFMpegInfo::findCodecInfo(c);

	{
		codec = avcodec_find_encoder(c);
		if (codec == NULL) goto find_encoder_error;

		codecCtx = avcodec_alloc_context();
		codecCtx->width = w;
		codecCtx->height = h;
		codecCtx->pix_fmt = info.supportedPixelFormat.front();
		codecCtx->time_base = r;

		if ((rc = avcodec_open(codecCtx,codec)) != 0) {
			if (errstr && errsize) av_strerror(rc,errstr,errsize);
			goto open_codec_error;
		}
	}

	// Update the internal config
	{
		if (mCtx.outputCodec != NULL && mCtx.outputCodecCtx != NULL) {
			avcodec_close(mCtx.outputCodecCtx);
		}
		mCtx.outputCodec = codec;
		mCtx.outputCodecCtx = codecCtx;
	}

	return true;
	open_codec_error:
	av_free(codecCtx);
	find_encoder_error:
	if (errstr && errsize) snprintf (errstr,errsize, "can't find encoder");

	return false;
}

inline bool FFMpegVideoProvider::Impl::updateSws()
{
	assert ( (mCtx.outputCodecCtx != NULL) && (mCtx.inputCodecCtx != NULL));

	SwsContext *swsCtx = sws_getCachedContext(NULL,
			mCtx.inputCodecCtx->width,mCtx.inputCodecCtx->height,mCtx.inputCodecCtx->pix_fmt,
			mCtx.outputCodecCtx->width,mCtx.outputCodecCtx->height,mCtx.outputCodecCtx->pix_fmt,
			SWS_FAST_BILINEAR,NULL,NULL,NULL
	);

	if (swsCtx == NULL) {
		return false;
	}

	if (mCtx.swsCtx) sws_freeContext(mCtx.swsCtx);
	mCtx.swsCtx = swsCtx;

	if (mCtx.convertedFrame) {
		avpicture_free( (AVPicture *)mCtx.convertedFrame);
		av_free(mCtx.convertedFrame);
	}

	mCtx.convertedFrame = avcodec_alloc_frame();
	if (avpicture_alloc((AVPicture *)mCtx.convertedFrame,mCtx.outputCodecCtx->pix_fmt, mCtx.outputCodecCtx->width, mCtx.outputCodecCtx->height) != 0){
		sws_freeContext(mCtx.swsCtx);
		mCtx.swsCtx = NULL;
		av_free(mCtx.convertedFrame);
		mCtx.convertedFrame = NULL;
		return false;
	}

	return true;
}


void FFMpegVideoProvider::Impl::entry() {
	state = STATE_CAPTURING;

	AVPacket pkt;

	int got_frame = 0, rc = 0 ;

	while (!shouldStop()) {
		VBuffer *vbuf;
		// try to get a buffer
		{
			if (!mBufferMutex.lock(500)) continue;
			if (mBuffers.size() < 1) {
				mBufferMutex.unlock();
				continue;
			}
			vbuf = mBuffers.front();
			mBuffers.pop_front();
			mBufferMutex.unlock();
		}

		AVFrame *decodedFrame = avcodec_alloc_frame();
		got_frame = 0;

		// Decoding
		while (!got_frame && !shouldStop()) {
			av_init_packet(&pkt);

			// Read packet
			if ((rc = av_read_frame(mCtx.inputFmtCtx,&pkt)) != 0) {
				av_free_packet(&pkt);
				goto read_packet_error;
			}

			// Check packet type
			if (mCtx.inputFmtCtx->streams[pkt.stream_index]->codec->codec != mCtx.inputCodec) {
				av_free_packet(&pkt);
				continue;
			}

			// Decode video
			if ((rc = avcodec_decode_video2(mCtx.inputCodecCtx,decodedFrame,&got_frame,&pkt)) < 0){
				av_free_packet(&pkt);
				av_free(decodedFrame);
				goto decode_error;
			}

			av_free_packet(&pkt);
		}

		if (!got_frame) {
			av_free(decodedFrame);
			break;
		}

		// Do swscale
		{
			if (sws_scale(mCtx.swsCtx,decodedFrame->data,decodedFrame->linesize,0,mCtx.inputCodecCtx->height,
					mCtx.convertedFrame->data,mCtx.convertedFrame->linesize) < 0) {
				av_free(decodedFrame);
				goto sws_scale_error;
			}
			av_free(decodedFrame);
		}

		// Do encode
		{
			if ((rc = avcodec_encode_video(mCtx.outputCodecCtx,vbuf->buf.getData(), vbuf->buf.getSize(),mCtx.convertedFrame)) < 0) {
				goto encode_error;
			}
			vbuf->size = rc;
			vbuf->cond.signal();
		}
	}

	encode_error:
	sws_scale_error:
	decode_error:
	read_packet_error:

	state = STATE_READY;
	return;
}
