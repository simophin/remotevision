/*
 * VideoCommand.cpp
 *
 *  Created on: 2011-5-16
 *      Author: launliu
 */

#include "VideoCommand.h"
#include "CommandMgr.h"
#include "CommandHandler.h"
#include "Command.h"
#include "CommandContext.h"
#include "VideoInfo.h"
#include "VideoProvider.h"
#include "CommandBuilder.h"
#include "IODevice.h"
#include "Commander.h"
#include "Geometry.h"
#include "Log.h"
#include "Thread.h"

#include <string>
#include <vector>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/lexical_cast.hpp>
#include <assert.h>
#include <algorithm>

#define MAX_VIDEO_DATA_BUFFER_SIZE 1024000

static VideoCommand *VCMD_INSTANCE = 0;
const char * VideoCommand::QueryInfoCommandHandler::ERROR_STRING = "QUERY_INFO_FAILED";
const char * VideoCommand::QueryInfoCommandHandler::SUCCESS_STRING = "QUERY_INFO_OK";
const char * VideoCommand::QueryInfoCommandHandler::REQUEST_STRING = "QUERY_INFO";

const char * VideoCommand::SetParameterCommandHandler::REQUEST_STRING = "SET_PARAM";
const char * VideoCommand::SetParameterCommandHandler::SUCCESS_STRING = "SET_PARAM_OK";
const char * VideoCommand::SetParameterCommandHandler::ERROR_STRING   = "SET_PARAM_FAILED";

const char * VideoCommand::GetParameterCommandHandler::REQUEST_STRING = "GET_PARAM";
const char * VideoCommand::GetParameterCommandHandler::SUCCESS_STRING = "GET_PARAM_OK";
const char * VideoCommand::GetParameterCommandHandler::ERROR_STRING   = "GET_PARAM_FAILED";

const char * VideoCommand::StartCaptureCommandHandler::REQUEST_STRING = "START_CAPTURE";
const char * VideoCommand::StartCaptureCommandHandler::SUCCESS_STRING = "START_CAPTURE_OK";
const char * VideoCommand::StartCaptureCommandHandler::ERROR_STRING   = "START_CAPTURE_FAILED";

const char * VideoCommand::StopCaptureCommandHandler::REQUEST_STRING = "STOP_CAPTURE";
const char * VideoCommand::StopCaptureCommandHandler::SUCCESS_STRING = "STOP_CAPTURE_OK";
const char * VideoCommand::StopCaptureCommandHandler::ERROR_STRING   = "STOP_CAPTURE_FAILED";


VideoCommand::QueryInfoCommandHandler::QueryInfoCommandHandler()
:CommandHandler(REQUEST_STRING){
}

VideoCommand::QueryInfoCommandHandler::~QueryInfoCommandHandler() {
}

void VideoCommand::QueryInfoCommandHandler::
onHandle(const Command &cmd, const CommandContext * ctx) {
	VideoProvider::Info info = ctx->videoProvider->queryInfo();
	CommandBuilder builder;
	builder.setResponseCommand(SUCCESS_STRING);
	builder.appendArgument(info.toString());

	Commander parser(ctx->controlDevice);
	Command response;
	builder.build(response);
	parser.writeCommand(response);
}

VideoProvider::Info VideoCommand::QueryInfoCommandHandler::
parseVideoInformationFromCommand(const Command & cmd) {
	assert(cmd.getName() == SUCCESS_STRING && cmd.getArguments().size() == 1);
	return VideoProvider::Info::fromString(cmd.getArgument(0));
}

VideoCommand::SetParameterCommandHandler::
~SetParameterCommandHandler()
{
}



VideoCommand::SetParameterCommandHandler::
SetParameterCommandHandler()
:CommandHandler(REQUEST_STRING)
{
}



void VideoCommand::SetParameterCommandHandler::
onHandle(const Command & cmd, const CommandContext *ctx)
{
	CommandBuilder builder;
	if (cmd.getArguments().size() != 2) {
		builder.setResponseCommand(ERROR_STRING, "invalid argument count");
	}else{
		const char *err = 0;
		Geometry requestGeo;
		VideoCodec requestCodec;

		// Process the geometry requested
		{
			requestGeo = Geometry::fromString(cmd.getArgument(0));
			if (!requestGeo.isValid()) {
				err = "Invalid geometry";
				goto error_out;
			}
		}

		// Process the video codec requested
		{
			requestCodec = VideoCodec::fromString(cmd.getArgument(1));
			if (requestCodec.codecId == VCODEC_INVALID) {
				err = "Invalid/Unsupported video codec";
				goto error_out;
			}

			VideoProvider::Info info = ctx->videoProvider->queryInfo();
			if (std::find(info.supportedCodecs.begin(), info.supportedCodecs.end(),
					requestCodec) == info.supportedCodecs.end()) {
				err = "Invalid/Unsupported video codec";
				goto error_out;
			}
		}

		{
			VideoProvider::Param p = ctx->videoProvider->getParam();
			p.currentGeometry = requestGeo;
			p.currentCodec = requestCodec;
			if (!ctx->videoProvider->setParam(p)) {

			}
		}

		builder.setResponseCommand(SUCCESS_STRING);
		goto out;

		error_out:
		builder.setResponseCommand(ERROR_STRING, err);
	}

	out:
	Command response;
	Commander parser(ctx->controlDevice);
	builder.build(response);
	if (!parser.writeCommand(response)) {
		Log::logError("Write response to %s failed: %s",
				REQUEST_STRING,
				parser.getLastError().getErrorString().c_str());
	}
}

void VideoCommand::SetParameterCommandHandler::
buildRequestCommand (Command &result, const Geometry &geo, const VideoCodec &codec) {
	CommandBuilder builder;

	assert(geo.isValid());
	assert(codec.codecId != VCODEC_INVALID);

	builder.setRequestCommand(REQUEST_STRING);
	builder.appendArgument(geo.toString());
	builder.appendArgument(codec.toString());
	builder.build(result);
}

void VideoCommand::GetParameterCommandHandler::
onHandle(const Command & cmd, const CommandContext *ctx)
{
	VideoProvider::Param param = ctx->videoProvider->getParam();
	CommandBuilder builder;
	Command response;
	builder.setResponseCommand(SUCCESS_STRING);
	builder.appendArgument(param.toString());
	builder.build(response);

	Commander cmder (ctx->controlDevice);
	cmder.writeCommand(response);
}



VideoCommand::GetParameterCommandHandler::GetParameterCommandHandler()
:CommandHandler (REQUEST_STRING)
{
}

VideoInfo VideoCommand::GetParameterCommandHandler::
parseInfoFromCommand(const Command & cmd)
{
	return VideoInfo::fromString(cmd.getArgument(0));
}


class DataThread: public Thread {
public:
	DataThread(const CommandContext &ctx)
	:mCtx(ctx) {}

	virtual void entry();

	const CommandContext &mCtx;
};

VideoCommand::StartCaptureCommandHandler::StartCaptureCommandHandler()
:CommandHandler(REQUEST_STRING)
{
}



VideoCommand::StartCaptureCommandHandler::~StartCaptureCommandHandler()
{
}



void VideoCommand::StartCaptureCommandHandler::
onHandle(const Command & cmd, const CommandContext *ctx)
{
	CommandBuilder response;
	if (!ctx->videoProvider->startCapture()) {
		response.setResponseCommand(ERROR_STRING,ctx->videoProvider->getLastError().getErrorString());
	}else{
		if (ctx->dataThread == NULL) {
			const_cast<CommandContext *>(ctx)->dataThread = new DataThread(*ctx);
		}
		if (!ctx->dataThread->isRunning()) {
			ctx->dataThread->run();
		}

		response.setResponseCommand(SUCCESS_STRING);
	}

	Command rcmd;
	response.build(rcmd);

	Commander writer (ctx->controlDevice);
	if (!writer.writeCommand(rcmd)){
		Log::logError("While writing to control device: %s", writer.getLastError().getErrorString().c_str());
	}
}

void DataThread::entry() {
	unsigned char *vbuf = (unsigned char *)::malloc(MAX_VIDEO_DATA_BUFFER_SIZE);

	while (!shouldStop()) {
		size_t size = mCtx.videoProvider->getData(vbuf,MAX_VIDEO_DATA_BUFFER_SIZE);
		if (size > 0) {
			if (mCtx.dataDevice->write((char *)vbuf, size) < 0) {
				Log::logError("Can't write to data device: %s", mCtx.dataDevice->getLastError().getErrorString().c_str());
				break;
			}
		}
	}
	mCtx.videoProvider->stopCapture();
	::free(vbuf);
}

VideoCommand::StopCaptureCommandHandler::StopCaptureCommandHandler()
:CommandHandler(REQUEST_STRING)
{
}



VideoCommand::StopCaptureCommandHandler::~StopCaptureCommandHandler()
{
}



void VideoCommand::StopCaptureCommandHandler::
onHandle(const Command & cmd, const CommandContext *ctx)
{
	Thread * t = ctx->dataThread;
	if (t){
		const_cast<CommandContext *>(ctx)->dataThread = NULL;
		t->stop();
		delete t;
	}
	ctx->videoProvider->stopCapture();

	CommandBuilder builder;
	Command rcmd;
	Commander writer (ctx->controlDevice);
	builder.setResponseCommand(SUCCESS_STRING);
	builder.build(rcmd);
	if (!writer.writeCommand(rcmd)) {
		Log::logError("Can't write to data device: %s",
				writer.getLastError().getErrorString().c_str());
	}
}






