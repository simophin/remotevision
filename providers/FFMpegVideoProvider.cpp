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

	VBuffer (const VBuffer &rhs)
	:buf (rhs.buf),
	 size(rhs.size),
	 returned (rhs.returned) {

	}

	VBuffer ()
	:size(0){

	}
};

typedef Utils::SharedPtr<VBuffer> VBufferPtr;

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
		const char * inputFmt;
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
	std::list<VBufferPtr> mBuffers;
	Condition mBufferCond;
	Mutex mBufferMutex;

	// Preview buffer
	std::list<VBufferPtr> mPreviewBuffers;
	Condition mPreviewBufferCond;
	Mutex mPreviewBufferMutex;


	// Thread related
	Error mThreadError;
	Mutex mThreadMutex;
	Condition mThreadCond;

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
		int i = 0;

		for (i=0;i<ARRAY_SIZE(TRY_INPUT_FORMATS); i++) {
			// Find input format
			if (TRY_INPUT_FORMATS[i] != NULL) {
				fmt = av_find_input_format(TRY_INPUT_FORMATS[i]);
				if (fmt == NULL) continue;
			}

			if (av_open_input_file(&fmtCtx, d->mFileName.c_str(),fmt,0,NULL) != 0){
				fmtCtx = NULL;
				continue;
			}else break;
		}

		if (fmtCtx == NULL) {
			rc.setErrorString("Can't open the specified file");
			return rc;
		}

		d->mCtx.inputFmt = TRY_INPUT_FORMATS[i];
		av_close_input_file(fmtCtx);
		d->mState = STATE_READY;
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

	// Wait for signal from the capture thread
	{
		d->mThreadMutex.lock();
		rc = d->mThreadCond.wait(d->mThreadMutex,5000);
		if (rc.isError()) {
			d->stopNoWait();
			d->mThreadMutex.unlock();
			return rc;
		}

		rc = d->mThreadError;
		d->mThreadMutex.unlock();
	}

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



	rc = d->mBufferMutex.lock(ms);
	if (!rc.isSuccess()) return rc;

	VBufferPtr buf(new VBuffer());
	buf->buf.setData(data,size);
	d->mBuffers.push_back(buf);
	d->mBufferCond.signal();
	d->mBufferMutex.unlock();

	buf->mutex.lock();
	rc = buf->cond.wait(buf->mutex, ms);

	if (!rc.isSuccess()) {
		return rc;
	}
	buf->mutex.unlock();

	*returned = buf->size;
	rc = buf->returned;
	return rc;
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

	d->mCurrentParam = param;
	return ret;
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

	AVFormatContext *fmtCtx = NULL;
	AVCodecContext  *decodeCtx = NULL, *encodeCtx = NULL;
	AVCodec         *decoder= NULL, *encoder = NULL;
	SwsContext      *swsCtx;
	AVFrame         *convertedFrame = avcodec_alloc_frame();
	AVPacket pkt;
	VBufferPtr buf;

	FFMpegCodecInfo encodeInfo = FFMpegInfo::findCodecInfo(mCurrentParam.currentCodec);

	// Open file
	{
		AVInputFormat *inputFmt = av_find_input_format(mCtx.inputFmt);
		AVFormatParameters ap;
		::memset(&ap,0,sizeof(ap));

		ap.width = 640;
		ap.height = 480;
		ap.time_base = (AVRational) {1,25};
		if (av_open_input_file(&fmtCtx,mFileName.c_str(),inputFmt,0,&ap) != 0
				|| fmtCtx == NULL) {
			rc.setErrorString("Open input error");
			goto open_input_file_failed;
		}
	}

	// Find video track
	{
		for (int i=0; i<fmtCtx->nb_streams; i++) {
			if (fmtCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
				decodeCtx = fmtCtx->streams[i]->codec;
				break;
			}
		}
		if (decodeCtx == NULL) {
			rc.setErrorString("No video track found");
			goto find_video_track_failed;
		}
	}

	// Find decoder
	{
		decoder = avcodec_find_decoder(decodeCtx->codec_id);
		if (decoder == NULL) {
			rc.setErrorString("Can't find decoder");
			goto find_decoder_failed;
		}

		if (decodeCtx->pix_fmt == PIX_FMT_NONE ) {
			decodeCtx->pix_fmt = PIX_FMT_YUVJ420P;
		}
	}

	// Open decoder
	{
		if (avcodec_open(decodeCtx,decoder) != 0) {
			rc.setErrorString("Can't open decoder");
			goto open_decoder_failed;
		}
	}

	// Find encoder
	{
		encoder = avcodec_find_encoder(encodeInfo.codecId);
		if (encoder == NULL) {
			rc.setErrorString("Can't find encoder");
			goto find_encoder_failed;
		}

		encodeCtx = avcodec_alloc_context();
		encodeCtx->width = mCurrentParam.currentGeometry.width;
		encodeCtx->height = mCurrentParam.currentGeometry.height;
		encodeCtx->time_base = (AVRational){mCurrentParam.currentFrameRate.num, mCurrentParam.currentFrameRate.den};
		encodeCtx->pix_fmt = encodeInfo.supportedPixelFormat.front();
		encodeCtx->bit_rate = 480000;
		encodeCtx->max_b_frames = 1;
	}

	// Open encoder
	{
		if (avcodec_open(encodeCtx, encoder) != 0) {
			rc.setErrorString("Open encoder failed");
			goto open_encoder_failed;
		}
	}

	// Init swscale
	{
		swsCtx = sws_getCachedContext(NULL, decodeCtx->width, decodeCtx->height,decodeCtx->pix_fmt,
				encodeCtx->width,encodeCtx->height,encodeCtx->pix_fmt, SWS_FAST_BILINEAR,
				NULL,NULL,NULL);
		if (swsCtx == NULL) {
			rc.setErrorString("Init sws context failed");
			goto init_sws_context_failed;
		}
	}

	// Allocate space for sws conversion
	{
		if (avpicture_alloc((AVPicture *)convertedFrame,encodeCtx->pix_fmt,encodeCtx->width,encodeCtx->height) != 0) {
			rc.setErrorString("Allocate space for conversion failed");
			goto allocate_space_failed;
		}
	}

	// All init finish successfully. Notify it
	{
		mThreadError = rc;
		mThreadMutex.lock();
		mThreadCond.signal();
		mThreadMutex.unlock();
	}

	// The mainloop
	while (!shouldStop()) {

		int got_frame = 0;
		AVFrame * decodedFrame = avcodec_alloc_frame();
		// Get a frame of decoded picture
		while (!got_frame && !shouldStop()){
			av_init_packet(&pkt);
			if (av_read_frame(fmtCtx, &pkt) != 0){
				rc.setErrorString("Read frame error");
				av_free_packet (&pkt);
				goto read_frame_error;
			}

			if ( fmtCtx->streams[pkt.stream_index]->codec != decodeCtx) {
				av_free_packet(&pkt);
				continue;
			}

			if (avcodec_decode_video2(decodeCtx,decodedFrame,&got_frame,&pkt) < 0){
				rc.setErrorString("Decode video frame error");
				av_free_packet (&pkt);
				goto decode_frame_error;
			}
			av_free_packet (&pkt);
		}

		if (!got_frame) {
			rc.setErrorString("Decode thread is requested to stop");
			goto decode_stop;
		}

		// Do sws scale
		{
			if (sws_scale(swsCtx,decodedFrame->data,decodedFrame->linesize,
					0,decodeCtx->height, convertedFrame->data, convertedFrame->linesize) < 1) {
				rc.setErrorString("sws scale failed");
				goto sws_scale_failed;
			}
		}

		// Wait a buffer to encode to
		{
			mBufferMutex.lock();
			while (!shouldStop() && (mBuffers.size() < 1)) {
				rc = mBufferCond.wait(mBufferMutex,500);
				if (rc.isError()) {
					if (rc.getErrorType() == Error::ERR_TIMEOUT) continue;
					else {
						mBufferMutex.unlock();
						goto wait_buffer_error;
					}
				}else{
					break;
				}
			}
			if (mBuffers.size() < 1) {
				mBufferMutex.unlock();
				goto wait_buffer_error;
			}
			buf = mBuffers.front();
			mBuffers.pop_front();
			mBufferMutex.unlock();
		}

		// Encode the video
		{
			int encoded = avcodec_encode_video(encodeCtx, buf->buf.getData(),buf->buf.getSize(),convertedFrame);
			if (encoded < 0) {
				rc.setErrorString("Encode video error");
				goto encode_video_error;
			}
			buf->size = encoded;
		}


		encode_video_error:
		buf->returned = rc;
		buf->cond.signal();
		wait_buffer_error:
		sws_scale_failed:
		decode_stop:
		decode_frame_error:
		read_frame_error:

		av_free(decodedFrame);
		if (rc.isError()) break;
	}


	// Notify all waiter(if exists)
	{
		mBufferMutex.lock();
		for (std::list<VBufferPtr>::const_iterator iter = mBuffers.begin();
				iter != mBuffers.end(); ++ iter) {
			(*iter)->mutex.lock();
			(*iter)->returned = rc;
			(*iter)->cond.signal();
			(*iter)->size = 0;
			(*iter)->mutex.unlock();
		}
		mBuffers.clear();
		mBufferMutex.unlock();
	}

	allocate_space_failed:
	sws_freeContext(swsCtx);
	init_sws_context_failed:
	avcodec_close(encodeCtx);
	open_encoder_failed:
	av_free(encodeCtx);
	find_encoder_failed:
	avcodec_close(decodeCtx);
	open_decoder_failed:
	find_decoder_failed:
	find_video_track_failed:
	av_close_input_file(fmtCtx);
	open_input_file_failed:
	av_free(convertedFrame);

	mState = STATE_READY;
	mThreadMutex.lock();
	mThreadError = rc;
	mThreadCond.signal();
	mThreadMutex.unlock();
	return rc;
}
