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

class FFMpegVideoProvider::Impl: public Thread {
public:
	VideoInfo *videoInfo;
	State state;
	Error lastError;

	String mFileName;

	// Related to ffmpeg decoding/encoding
	Param mCurrentParam;

	struct ffmpeg_context{
		AVFormatContext * inputFmtCtx;
		AVCodecContext  * inputCodecCtx, * outputCodecCtx;
		AVCodec * inputCodec, * outputCodec;
		SwsContext *swsCtx;
		AVFrame * convertedFrame;
	} mCtx;

	Error setOutputCodec (CodecID c, int w,int h, const AVRational &);
	Error updateSws ();

	// Buffer related
	std::list<VBuffer *> mBuffers;
	Condition mBufferCond;
	Mutex mBufferMutex;

	// Thread entry
	virtual Error entry();

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

	Impl(const String &f)
	:videoInfo(0),state(STATE_UNINTIALIZED),
	 mFileName(f){
		::memset(&mCtx,0,sizeof(mCtx));
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
	assert (d->state == STATE_UNINTIALIZED);
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
		AVFormatParameters ap;
		memset(&ap,0,sizeof(ap));
		assert (fmt != NULL);

		ap.width = 320;
		ap.height = 240;
		ap.time_base = (AVRational){1,25};
		ap.pix_fmt = info.supportedPixelFormat.front();
		if ( (av_open_input_file(&inputFmtCtx, d->mFileName.c_str(),fmt,0,&ap)) != 0){
			rc.setErrorType(Error::ERR_UNKNOWN, "Open camera device error");
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

		if (decodeCodec->capabilities & CODEC_CAP_TRUNCATED) {
			decodeCtx->flags |= CODEC_FLAG_TRUNCATED;
		}

		if ((avcodec_open(decodeCtx,decodeCodec)) != 0) {
			rc.setErrorType(Error::ERR_UNKNOWN, "Open camera decoder failed");
			goto open_decoder_error;
		}

		d->mCtx.inputCodec = decodeCodec;
		d->mCtx.inputCodecCtx = decodeCtx;
		d->mCtx.inputFmtCtx = inputFmtCtx;
	}

	{
		AVRational ral = (AVRational){d->mCurrentParam.currentFrameRate.num,d->mCurrentParam.currentFrameRate.den};
		rc = d->setOutputCodec(info.codecId, d->mCurrentParam.currentGeometry.width,
				d->mCurrentParam.currentGeometry.height, ral);
		if(!rc.isSuccess()) {
			goto set_output_codec_failed;
		}
	}

	{
		rc = d->updateSws();
		if (!rc.isSuccess()) {
			goto update_sws_failed;
		}
	}

	d->state = STATE_READY;
	return rc;

	update_sws_failed:
	avcodec_close(d->mCtx.outputCodecCtx);
	d->mCtx.outputCodecCtx = 0;
	set_output_codec_failed:
	avcodec_close(d->mCtx.inputCodecCtx);
	d->mCtx.inputCodecCtx = 0;
	open_decoder_error:
	find_decoder_error:
	av_close_input_file(inputFmtCtx);
	open_input_file_error:
	errbuf[sizeof(errbuf)-1] = 0;
	d->state = STATE_UNINTIALIZED;
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


	for (int i=0;i<ARRAY_SIZE(SUPPORTED_CODECS);i++) {
		info.supportedCodecs.push_back(VideoCodec(SUPPORTED_CODECS[i]));
	}
	info.supportedGeometries = cf.supportedGeometries;

	return rc;
}


Error FFMpegVideoProvider::doStartCapture()
{
	Error rc;
	if (d->state != STATE_READY || d->isRunning()) {
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
	if (d->state == STATE_CAPTURING) {
		ret.setErrorType(Error::ERR_STATE);
		return ret;
	}

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
}

void FFMpegVideoProvider::init() {
	d->mCurrentParam.currentCodec = VCODEC_MPEG4;
	FFMpegCodecInfo cf = FFMpegInfo::findCodecInfo(d->mCurrentParam.currentCodec);
	assert (cf.codecId != CODEC_ID_NONE);
	d->mCurrentParam.currentGeometry = cf.supportedGeometries[0];
	d->mCurrentParam.currentFrameRate.num = cf.supportedRationals[2].num;
	d->mCurrentParam.currentFrameRate.den = cf.supportedRationals[2].den;
}



inline Error FFMpegVideoProvider::Impl::
setOutputCodec(CodecID c, int w, int h, const AVRational & r )
{
	AVCodec * codec = 0;
	AVCodecContext *codecCtx = 0;
	Error rc;

	FFMpegCodecInfo info = FFMpegInfo::findCodecInfo(c);

	{
		codec = avcodec_find_encoder(c);
		if (codec == NULL) goto find_encoder_error;

		codecCtx = avcodec_alloc_context();
		codecCtx->width = w;
		codecCtx->height = h;


		codecCtx->pix_fmt = info.supportedPixelFormat.front();
		codecCtx->time_base = r;



		codecCtx->bit_rate = 240000;
		codecCtx->max_b_frames = 1;
		codecCtx->gop_size = 10;


		//codecCtx->bit_rate = 64000;
		//codecCtx->sample_rate = 44100;

		if ((avcodec_open(codecCtx,codec)) != 0) {
			rc.setErrorString("Can't open encoder codec");
			goto open_codec_error;
		}

		mCurrentParam.currentGeometry.height = codecCtx->height;
		mCurrentParam.currentGeometry.width = codecCtx->width;
	}

	// Update the internal config
	{
		if (mCtx.outputCodec != NULL && mCtx.outputCodecCtx != NULL) {
			avcodec_close(mCtx.outputCodecCtx);
		}
		mCtx.outputCodec = codec;
		mCtx.outputCodecCtx = codecCtx;
	}

	return rc;
	open_codec_error:
	av_free(codecCtx);
	find_encoder_error:
	return rc;
}

inline Error FFMpegVideoProvider::Impl::updateSws()
{
	Error rc;
	assert ( (mCtx.outputCodecCtx != NULL) && (mCtx.inputCodecCtx != NULL));


	SwsContext *swsCtx = sws_getCachedContext(NULL,
			mCtx.inputCodecCtx->width,mCtx.inputCodecCtx->height,mCtx.inputCodecCtx->pix_fmt,
			mCtx.outputCodecCtx->width,mCtx.outputCodecCtx->height,mCtx.outputCodecCtx->pix_fmt,
			SWS_FAST_BILINEAR,NULL,NULL,NULL
	);


	if (swsCtx == NULL) {
		rc.setErrorString("Can't create swscontext!");
		return rc;
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
		rc.setErrorString("Can't allocate space for conversion");
		return rc;
	}

	return rc;
}


Error FFMpegVideoProvider::Impl::entry() {
	state = STATE_CAPTURING;

	AVPacket pkt;

	int got_frame = 0;
	Error rc;

	while (!shouldStop()) {
		VBuffer *vbuf;
		// try to get a buffer
		{
			rc = mBufferMutex.lock(500);
			if (rc.getErrorType() == Error::ERR_TIMEOUT) continue;
			else break;

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
			if ((av_read_frame(mCtx.inputFmtCtx,&pkt)) != 0) {
				rc.setErrorString("read frame from camera failed");
				av_free_packet(&pkt);
				goto read_packet_error;
			}

			// Check packet type
			if (mCtx.inputFmtCtx->streams[pkt.stream_index]->codec->codec != mCtx.inputCodec) {
				av_free_packet(&pkt);
				continue;
			}

			// Decode video
			if ((avcodec_decode_video2(mCtx.inputCodecCtx,decodedFrame,&got_frame,&pkt)) < 0){
				av_free_packet(&pkt);
				av_free(decodedFrame);
				rc.setErrorString("decode video from camera failed");
				goto decode_error;
			}

			av_free_packet(&pkt);
		}

		if (!got_frame) {
			rc.setErrorType(Error::ERR_TIMEOUT);
			av_free(decodedFrame);
			break;
		}

		// Do swscale
		{
			if (sws_scale(mCtx.swsCtx,decodedFrame->data,decodedFrame->linesize,0,mCtx.inputCodecCtx->height,
					mCtx.convertedFrame->data,mCtx.convertedFrame->linesize) < 0) {
				av_free(decodedFrame);
				rc.setErrorString("Do sws scaling failed");
				goto sws_scale_error;
			}
			av_free(decodedFrame);
		}

		// Do encode
		{
			int encoded_size;
			if ((encoded_size = avcodec_encode_video(mCtx.outputCodecCtx,vbuf->buf.getData(), vbuf->buf.getSize(),mCtx.convertedFrame)) < 0) {
				rc.setErrorString("Encode video failed");
				goto encode_error;
			}
			vbuf->size = encoded_size;
			vbuf->cond.signal();
		}
	}

	encode_error:
	sws_scale_error:
	decode_error:
	read_packet_error:

	state = STATE_READY;
	return rc;
}
