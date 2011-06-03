/*
 * IOVideoSource.cpp
 *
 *  Created on: 2011-5-13
 *      Author: simophin
 */

#include "IOVideoSource.h"
#include "IODevice.h"
#include "ImageBuffer.h"
#include "Command.h"
#include "Log.h"
#include "Thread.h"

#include "Commander.h"
#include "commands/CommandBuilder.h"
#include "commands/VideoCommand.h"
#include "3rdparty/ffmpeg/FFMpegInfo.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
#include <libavdevice/avdevice.h>
}

#include "assert.h"
#include <vector>
#include <list>
#include <algorithm>

enum State {
	STATE_UNINTIALIZED,
	STATE_READY,
	STATE_CAPTURING,
};

class IOVideoSource::Impl: public Thread {
public:
	IODevice *ctrlDev, *dataDev;
	Commander cmdParser;

	State state;

	Info info;
	IOVideoSource::ImageParam param;

	struct Config {
		int bufferCount;
		size_t bufferSize;
	} config;


	AVCodecContext *decodeCtx;
	AVCodec * decodeCodec;
	SwsContext *swsCtx;
	std::vector<AVFrame *> convertedBuffers;
	std::list<int> filledBuffers, emptyBuffers;
	Mutex filledBufferMutex, emptyBufferMutex;
	Condition filledBufferCond, emptyBufferCond;
	void updateFFmpeg();


	Impl(IODevice *ctrl, IODevice *data)
	:ctrlDev(ctrl),
	 dataDev(data),
	 cmdParser(ctrl),
	 state(STATE_UNINTIALIZED),
	 decodeCodec(0),
	 decodeCtx(0),
	 swsCtx(0){
		::memset(&config,0,sizeof(config));
		config.bufferCount = 10;
		config.bufferSize = 102400;
	}

	virtual void entry();



};

IOVideoSource::IOVideoSource(IODevice *ctrl, IODevice *data)
:d(new IOVideoSource::Impl(ctrl,data)){
}

IOVideoSource::~IOVideoSource() {
	delete d;
}

IOVideoSource::Info IOVideoSource::
doGetInformation(int ms) const
{
	return d->info;
}



bool IOVideoSource::
doSetImageParam(ImageParam & fmt, int ms)
{
	if (d->state != STATE_READY) {
		setLastError(Error("STATE ERROR"));
		return false;
	}

	d->param = fmt;
	d->updateFFmpeg();
	if (d->decodeCtx == 0 || d->swsCtx == 0) {
		setLastError(Error("Internal error"));
		return false;
	}
	return true;
}



IOVideoSource::Buffer IOVideoSource::
doGetFilledBuffer(int ms)
{
	Buffer ret;

	if (!d->filledBufferMutex.lock(ms)) {
		setLastError(Error(ETIMEDOUT));
		return ret;
	}

	if (d->filledBuffers.size() < 1) {
		if (!d->filledBufferCond.wait(d->filledBufferMutex,ms)) {
			setLastError(Error(ETIMEDOUT));
			return ret;
		}
	}

	int index = d->filledBuffers.front();
	d->filledBuffers.pop_front();
	d->filledBufferMutex.unlock();
	AVFrame *frame = d->convertedBuffers.at(index);
	ret.index = index;
	ret.buf = frame->data[0];
	ret.size = frame->linesize[0] + frame->linesize[1] + frame->linesize[2] + frame->linesize[3];
	return ret;
}



bool IOVideoSource::
doStopCapture(int ms)
{
	if (d->state == STATE_CAPTURING || d->isRunning()) {
		d->stop(ms);
	}

	return true;
}



void IOVideoSource::
doPutBuffer(const Buffer & buf, int ms)
{
	if (buf.index < 0 || buf.index >= d->convertedBuffers.size()) {
		Log::logError("Index %d is not existed", buf.index);
		return;
	}

	if (!d->emptyBufferMutex.lock(ms)){
		return;
	}
	if (!d->filledBufferMutex.lock(ms)) {
		d->emptyBufferMutex.unlock();
		return;
	}

	if (std::find(d->emptyBuffers.begin(), d->emptyBuffers.end(), buf.index) == d->emptyBuffers.end() &&
			std::find(d->filledBuffers.begin(), d->filledBuffers.end(), buf.index) == d->filledBuffers.end()) {
		d->emptyBuffers.push_back(buf.index);
	}
	d->filledBufferMutex.unlock();
	d->emptyBufferMutex.unlock();
}



IOVideoSource::ImageParam IOVideoSource::
doGetImageParam(int ms) const
{
	return d->param;
}



bool IOVideoSource::
doStartCapture(int ms)
{
	if (d->state != STATE_READY) {
		setLastError(Error("STATE ERROR"));
		return false;
	}

	// Require remote to start
	Command request,response;
	CommandBuilder builder;


	builder.clearArguments();
	builder.setRequestCommand(VideoCommand::StartCaptureCommandHandler::REQUEST_STRING);
	builder.build(request);
	if (!d->cmdParser.writeCommand(request)) {
		setLastError(d->cmdParser.getLastError());
		return false;
	}

	if (!d->cmdParser.readCommand(response)){
		setLastError(d->cmdParser.getLastError());
		return false;
	}

	if (response.getName() != VideoCommand::StartCaptureCommandHandler::SUCCESS_STRING) {
		setLastError(Error("Remote error"));
		return false;
	}

	d->run();
	return true;
}



bool IOVideoSource::
doInit(const IOVideoSource::Option & options, int ms)
{
	assert (d->state == STATE_UNINTIALIZED);
	Info info;
	CommandBuilder builder;
	Command cmd;

	// Fetch information
	builder.setRequestCommand(VideoCommand::QueryInfoCommandHandler::REQUEST_STRING);
	builder.build(cmd);

	if (!d->cmdParser.writeCommand(cmd)) {
		const_cast<IOVideoSource *>(this)->setLastError(d->cmdParser.getLastError());
		goto write_error;
	}

	if (!d->cmdParser.readCommand(cmd)) {
		const_cast<IOVideoSource *>(this)->setLastError(d->cmdParser.getLastError());
		goto read_error;
	}

	if (cmd.getName() == VideoCommand::QueryInfoCommandHandler::SUCCESS_STRING ) {
		info.providerInfo = VideoCommand::QueryInfoCommandHandler::parseVideoInformationFromCommand(cmd);
	}

	// Fetch command
	builder.clearArguments();
	builder.setRequestCommand(VideoCommand::GetParameterCommandHandler::REQUEST_STRING);
	builder.build(cmd);

	if (!d->cmdParser.writeCommand(cmd)) {
		const_cast<IOVideoSource *>(this)->setLastError(d->cmdParser.getLastError());
		goto write_error;
	}

	if (!d->cmdParser.readCommand(cmd)) {
		const_cast<IOVideoSource *>(this)->setLastError(d->cmdParser.getLastError());
		goto read_error;
	}

	if (cmd.getName() == VideoCommand::GetParameterCommandHandler::SUCCESS_STRING) {
		info.info = VideoCommand::GetParameterCommandHandler::parseInfoFromCommand(cmd);
	}

	if (info.info.isValid() && info.providerInfo.isValid()) {
		d->info = info;
		d->updateFFmpeg();
		return true;
	}else{
		return false;
	}

	write_error:
	read_error:
	return false;
}

inline void IOVideoSource::Impl::updateFFmpeg()
{

	if (decodeCtx != 0) {
		avcodec_close(decodeCtx);
		av_free(decodeCtx);
	}

	{
		decodeCodec = avcodec_find_decoder(FFMpegInfo::getIdFromRemoteVision(info.info.currentCodec.codecId));
		if (!decodeCodec) {
			Log::logError("Can't find decoder");
			goto find_decoder_error;
		}
	}

	{
		FFMpegCodecInfo finfo = FFMpegInfo::findCodecInfo(decodeCodec->id);
		if (finfo.codecId == CODEC_ID_NONE) {
			Log::logError("Can't find decoder info");
			goto find_decoder_info_error;
		}


		decodeCtx = avcodec_alloc_context();
		decodeCtx->width = info.info.currentGeometry.width;
		decodeCtx->height = info.info.currentGeometry.height;
		decodeCtx->pix_fmt = finfo.supportedPixelFormat.front();
		decodeCtx->time_base = (AVRational){info.info.currentFrameRate.den, info.info.currentFrameRate.num};

		if (avcodec_open(decodeCtx,decodeCodec) != 0) {
			Log::logError("Can't open decoder");
			goto decoder_open_error;
		}
	}

	SwsContext *newSwsCtx;
	// Create swsctx
	{
		newSwsCtx = sws_getCachedContext(NULL, decodeCtx->width, decodeCtx->height, decodeCtx->pix_fmt,
				param.geo.width, param.geo.height, FFMpegInfo::getPixFmtFromRemoteVision(param.pixFmt),
				SWS_FAST_BILINEAR,NULL,NULL,NULL);
		if (!newSwsCtx) {
			Log::logError("Can't create sws context");
			goto sws_create_error;
		}

	}

	// Clean buffers
	{
		filledBuffers.clear();
		emptyBuffers.clear();

		for (std::vector<AVFrame *>::const_iterator iter = convertedBuffers.begin();
				iter != convertedBuffers.end(); ++ iter) {
			avpicture_free((AVPicture *)*iter);
			av_free(*iter);
		}
		convertedBuffers.clear();
	}

	// Create buffers
	{
		AVFrame *frame;
		for (int i=0; i<config.bufferCount;i++) {
			frame = avcodec_alloc_frame();
			if (avpicture_alloc((AVPicture *)frame,FFMpegInfo::getPixFmtFromRemoteVision(param.pixFmt),
					info.info.currentGeometry.width,info.info.currentGeometry.height) != 0) {
				Log::logError("Allocate memory for converting failed...");
				break;
			}
			convertedBuffers.push_back(frame);
		}
		for (int i=0; i<convertedBuffers.size(); i++) {
			emptyBuffers.push_back(i);
		}
	}

	// Clean origin sws
	{
		if (swsCtx != 0) {
			sws_freeContext(swsCtx);
		}
		swsCtx = newSwsCtx;
	}

	return;

	sws_create_error:
	decoder_open_error:
	av_free(decodeCtx);
	find_decoder_info_error:
	decodeCtx = 0;
	decodeCodec = 0;
	find_decoder_error:
	return;
}

void IOVideoSource::Impl::entry() {
	state = STATE_CAPTURING;

	char *buf = (char *)::malloc(config.bufferSize);
	assert (buf != NULL);

	AVFrame *frame = avcodec_alloc_frame();
	int got_frame = 0;
	ssize_t readSize = 0;
	while (!shouldStop()){

		{
			readSize = dataDev->read( buf,config.bufferSize );
			if (readSize < 0) {
				goto read_error;
			}else if (readSize == 0) {
				continue;
			}
		}


		{
			int usedSize = 0;
			do {
				usedSize = avcodec_decode_video(decodeCtx, frame, &got_frame, (uint8_t *)(buf + usedSize), readSize - usedSize);
				if (usedSize < 0) goto decode_error;
				if (got_frame) {
					// Get a buffer to do swscale
					int index = -1;
					do {
						if (!emptyBufferMutex.lock(500)) continue;
						if (emptyBuffers.size() < 1) {
							if (!emptyBufferCond.wait(emptyBufferMutex,500)){
								emptyBufferMutex.unlock();
								continue;
							}
						}
						index = emptyBuffers.front();
						emptyBuffers.pop_front();
						break;
					}while(!shouldStop());

					if (index < 0) {
						goto get_empty_buffer_error;
					}

					AVFrame *convertedBuf = convertedBuffers.at(index);
					if (sws_scale(swsCtx,frame->data,frame->linesize,0,decodeCtx->height,convertedBuf->data,convertedBuf->linesize) < 0) {
						goto sws_error;
					}

					do {
						if (!filledBufferMutex.lock(500)) continue;
						if (filledBuffers.size() < 1) {
							if (!filledBufferCond.wait(filledBufferMutex,500)){
								filledBufferMutex.unlock();
								continue;
							}
						}

						filledBuffers.push_back(index);
					}while(!shouldStop());

					got_frame = 0;
					av_free(frame);
					frame = avcodec_alloc_frame();
				}
			}while ( readSize - usedSize > 0 && !shouldStop());
		}

		continue;

		sws_error:
		get_empty_buffer_error:
		decode_error:
		read_error:
		av_free(frame);
		break;
	}



	state = STATE_READY;
}
