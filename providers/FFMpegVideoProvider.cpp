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

class FFMpegVideoProvider::Impl: public Thread {
public:
	VideoInfo *videoInfo;
	State state;
	Error lastError;

	std::string mFileName;

	// Related to ffmpeg decoding/encoding
	Geometry mGeometry;
	VideoCodec mVideoCodec;

	AVInputFormat * mInputFmt;
	AVFormatContext * mInputFmtCtx;
	AVCodecContext  * mInputCodecCtx, * mOutputCodecCtx;
	AVCodec * mInputCodec, * mOutputCodec;
	SwsContext *mSwsCtx;

	// Buffer related
	std::list<VBuffer *> mBuffers;
	Condition mBufferCond;
	Mutex mBufferMutex;

	// Thread entry
	virtual void entry();

	virtual ~Impl () {
		if (videoInfo) delete videoInfo;
	}

	Impl(const std::string &f)
	:videoInfo(0),state(STATE_READY),
	 mFileName(f){}
};

FFMpegVideoProvider::FFMpegVideoProvider(const std::string &filename)
:d(new FFMpegVideoProvider::Impl(filename)){
	init();
}

FFMpegVideoProvider::~FFMpegVideoProvider() {
	delete d;
}

VideoInfo FFMpegVideoProvider::
queryInfo() const {
	if (d->videoInfo == 0) {
		VideoInfo *info = new VideoInfo;
		/*
		for (int i=0;i<ARRAY_SIZE(SUPPORTTED_GEOMETRIES);i++) {
			info->supportedGeometry.push_back(Geometry(SUPPORTTED_GEOMETRIES[i].width,
					SUPPORTTED_GEOMETRIES[i].height ));
		}

		AVCodec *codec = NULL;
		while ((codec = av_codec_next(codec)) != NULL) {
			VideoCodec vcodec;
			vcodec.codecId = getIdFromFFMpeg(codec->id);
			if (vcodec.codecId == VCODEC_INVALID) continue;
			std::vector<VideoCodec>::iterator found = std::find(info->supportedVideoCodecs.begin(),
					info->supportedVideoCodecs.end(), vcodec);
			if (found != info->supportedVideoCodecs.end()) continue;
			vcodec.pixelFormat = getPixFmtFromFFMpeg(PIX_FMT_YUV420P);
			info->supportedVideoCodecs.push_back(vcodec);
		}
		*/


		d->videoInfo = info;
	}

	d->videoInfo->currentGeometry = d->mGeometry;
	d->videoInfo->currentVideoCodec = d->mVideoCodec;
	return *d->videoInfo;
}

void FFMpegVideoProvider::init() {
	/*
	d->mGeometry.width = SUPPORTTED_GEOMETRIES[0].width;
	d->mGeometry.height = SUPPORTTED_GEOMETRIES[0].height;
	d->mVideoCodec.codecId = VCODEC_FLV;
	*/
}





bool FFMpegVideoProvider::startCapture()
{
	if (d->state != STATE_READY || d->isRunning()) {
		d->lastError = Error("Video is not ready or is being capturing");
		return false;
	}

	d->run();

	return true;
}

bool FFMpegVideoProvider::stopCapture()
{
	return true;
}

size_t FFMpegVideoProvider::getData(unsigned char *buf, size_t size)
{
	if (!d->mBufferMutex.lock(2000)) {
		return 0;
	}
	VBuffer vbuf;
	vbuf.buf.setData(buf,size);
	d->mBuffers.push_back(&vbuf);
	d->mBufferMutex.unlock();
	d->mBufferCond.signal();

	if (!vbuf.mutex.lock(2000)) {
		return 0;
	}
	if (!vbuf.cond.wait(vbuf.mutex) ){
		return 0;
	}
	vbuf.mutex.unlock();
	return vbuf.size;
}



Error FFMpegVideoProvider::
getLastError() const {
	return d->lastError;
}
bool FFMpegVideoProvider::
setParam (const Param & param) {
	assert(param.codec.codecId != VCODEC_INVALID);
	assert(param.geo.isValid());

	if (d->state == STATE_CAPTURING) {
		d->lastError = Error("Video device is capturing");
		return false;
	}

	d->mGeometry = param.geo;
	d->mVideoCodec = param.codec;
	return true;
}

void FFMpegVideoProvider::Impl::entry() {
	state = STATE_CAPTURING;

	char error[2048];
	int rc;
	AVFrame *convertedFrame = avcodec_alloc_frame();
	AVFormatParameters inputParam;

	// Initialize ffmpeg
	{
		memset(&inputParam,0,sizeof(inputParam));
		inputParam.width = 320;
		inputParam.height = 240;

		mInputFmtCtx = NULL;
		if ( (rc = av_open_input_file(&mInputFmtCtx,mFileName.c_str(),NULL,0,&inputParam)) != 0) {
			av_strerror(rc,error,sizeof(error));
			goto open_input_file_error;
		}

		mInputFmt = mInputFmtCtx->iformat;
	}

	// Find decoder
	{
		mInputCodecCtx = NULL;
		for (unsigned i=0;i<mInputFmtCtx->nb_streams; i++) {
			if (mInputFmtCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
				mInputCodecCtx = mInputFmtCtx->streams[i]->codec;
				break;
			}
		}
		if (mInputCodecCtx == NULL) {
			strncpy(error, "Can't find codec for video", sizeof(error));
			goto find_input_codec_error;
		}
	}

	// Find and open decoder
	{
		mInputCodec = avcodec_find_decoder(mInputCodecCtx->codec_id);
		if (mInputCodec == NULL) {
			strncpy(error, "Can't find decoder", sizeof(error));
			goto find_codec_error;
		}

		if (mInputCodec->capabilities & CODEC_CAP_TRUNCATED) {
			mInputCodecCtx->flags |= CODEC_FLAG_TRUNCATED;
		}

		if ( (rc = avcodec_open(mInputCodecCtx, mInputCodec)) != 0) {
			av_strerror(rc,error,sizeof(error));
			goto open_codec_error;
		}
	}

	// The encoder
	{
		mOutputCodec = avcodec_find_encoder(FFMpegInfo::getIdFromRemoteVision(mVideoCodec.codecId));
		if (mOutputCodec == NULL) {
			strncpy(error, "Can't find encoder", sizeof(error));
			goto find_encoder_error;
		}

		mOutputCodecCtx = avcodec_alloc_context3(mOutputCodec);
		if (mOutputCodecCtx == NULL) {
			goto alloc_context_error;
		}
		mOutputCodecCtx->height = mGeometry.height;
		mOutputCodecCtx->width  = mGeometry.width;
		mOutputCodecCtx->pix_fmt = PIX_FMT_YUV420P;
		mOutputCodecCtx->time_base = (AVRational){1,30};

		if ((rc = avcodec_open(mOutputCodecCtx, mOutputCodec)) != 0) {
			av_strerror(rc,error,sizeof(error));
			goto open_encoder_error;
		}
	}

	// The sws scaler
	{
		mSwsCtx = sws_getCachedContext(mSwsCtx, mInputCodecCtx->width,
				mInputCodecCtx->height,mInputCodecCtx->pix_fmt,mGeometry.width,
				mGeometry.height,mOutputCodecCtx->pix_fmt,SWS_FAST_BILINEAR,NULL,NULL,NULL);
		if (mSwsCtx == NULL) {
			av_strerror(rc,error,sizeof(error));
			goto sws_init_error;
		}


		if (avpicture_alloc((AVPicture *)convertedFrame,mOutputCodecCtx->pix_fmt,mGeometry.width,mGeometry.height) != 0) {
			goto sws_alloc_error;
		}

	}

	// The main loop
	{
		VBuffer *vbuf;

		int rc = 0;
		while (!shouldStop()) {
			if (!mBufferMutex.lock(500)) continue;
			if (mBuffers.size() < 1) {
				mBufferMutex.unlock();
				continue;
			}
			vbuf = mBuffers.front();
			mBuffers.pop_front();
			assert (vbuf != 0);
			mBufferMutex.unlock();

			// Got a buffer. Use ffmpeg to encode data.
			AVPacket pkt;
			AVFrame *frame = avcodec_alloc_frame();
			assert (frame != NULL);
			int got_picture = 0;
			while (!got_picture && !shouldStop()) {
				// DECODE packet
				rc = av_read_packet(mInputFmtCtx, &pkt);
				if (rc != 0) {
					av_strerror(rc,error,sizeof(error));
					break;
				}
				if ( mInputFmtCtx->streams[pkt.stream_index]->codec != mInputCodecCtx) {
					av_free_packet(&pkt);
					continue;
				}
				if ((rc = avcodec_decode_video2(mInputCodecCtx,frame,&got_picture,&pkt)) < 0 ){
					av_strerror(rc,error,sizeof(error));
					av_free_packet(&pkt);
					break;
				}
			}

			if (!got_picture) {
				av_free(frame);
				break;
			}


			// Do SWSCALE and ENCODE
			{
				rc = sws_scale(mSwsCtx,frame->data,frame->linesize,0,mOutputCodecCtx->height,
						convertedFrame->data, convertedFrame->linesize);
				if (rc < 0) {
					av_strerror(rc,error,sizeof(error));
				}else{
					rc = avcodec_encode_video(mOutputCodecCtx, (uint8_t *)vbuf->buf.getData(),
							vbuf->buf.getSize()/10,convertedFrame);
					if (rc < 0) {
						av_strerror(rc,error,sizeof(error));
					}else{
						vbuf->size = rc;
						vbuf->cond.signal();
					}
				}
			}

			av_free(frame);
			av_free_packet(&pkt);
		}
	}


	avpicture_free((AVPicture *)convertedFrame);
sws_alloc_error:
	av_free(convertedFrame);
sws_init_error:
	avcodec_close(mOutputCodecCtx);
open_encoder_error:
	mOutputCodecCtx = NULL;
alloc_context_error:
find_encoder_error:
	mOutputCodec = NULL;
open_codec_error:
	mInputCodec = NULL;
	mInputCodecCtx = NULL;
find_codec_error:
find_input_codec_error:
	av_close_input_file(mInputFmtCtx);
	mInputFmtCtx = NULL;
	mInputFmt = NULL;
open_input_file_error:
	error[sizeof(error)-1] = '\0';
	lastError = Error(error);
	state = STATE_READY;
	return;
}
