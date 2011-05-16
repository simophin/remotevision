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

#include <string>
#include <vector>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/lexical_cast.hpp>
#include <assert.h>

static VideoCommand *VCMD_INSTANCE = 0;

class VideoCommand::Impl {
public:
	std::vector<CommandHandler *> mCommandHandlers;
	State mState;

	// Variable related to encoding/decoding
	Geometry mVideoGeometry;
	VideoCodec mVideoCodec;

	Impl() {
		mCommandHandlers.push_back(new QueryInfoCommandHandler());
		mCommandHandlers.push_back(new SetParameterCommandHandler());
	}

	~Impl () {
		for (int i=0;i<mCommandHandlers.size();i++)
			delete mCommandHandlers[i];
		mCommandHandlers.clear();
	}
};

VideoCommand::VideoCommand()
:d(new VideoCommand::Impl()){
	VCMD_INSTANCE = this;
	d->mState = STATE_READY;
	for (int i=0;i<d->mCommandHandlers.size();i++)
		CommandMgr::getInstance()->registerCommandHandler(d->mCommandHandlers[i]);
}

VideoCommand::~VideoCommand() {
	delete d;
}

VideoCommand * VideoCommand::
getInstance() {
	return VCMD_INSTANCE;
}

VideoCommand::State VideoCommand::
getState() const {
	return d->mState;
}

void VideoCommand::setState(VideoCommand::State s) {
	d->mState = s;
}

VideoCommand::QueryInfoCommandHandler::QueryInfoCommandHandler()
:CommandHandler("QUERY_INFO"){
}

VideoCommand::QueryInfoCommandHandler::~QueryInfoCommandHandler() {
}

void VideoCommand::QueryInfoCommandHandler::
onHandle(const Command &cmd, const CommandContext * ctx) {
	VideoInfo info = ctx->videoProvider->queryInfo();
	CommandBuilder builder;
	builder.setResponseCommand("QUERY_INFO_OK");

	// For geometry information
	std::vector<std::string> geoargs;
	for (int i=0;i<info.supportedGeometry.size();i++) {
		const Geometry &geo = info.supportedGeometry[i];
		geoargs.push_back(geo.toString());
	}
	builder.appendArgument(boost::algorithm::join(geoargs,";"));

	// For codec information
	std::vector<std::string> codecargs;
	for (int i=0;i<info.supportedVideoCodecs.size();i++) {
		const VideoCodec &codec = info.supportedVideoCodecs[i];
		codecargs.push_back(codec.toString());
	}
	builder.appendArgument(boost::algorithm::join(codecargs,";"));

	Commander parser(ctx->controlDevice);
	Command response;
	builder.build(response);
	parser.writeCommand(response);
}

VideoInfo VideoCommand::QueryInfoCommandHandler::
parseVideoInformationFromCommand(const Command & cmd) {
	assert(cmd.getName() == "QUERY_INFO_OK" && cmd.getArguments().size() == 2);
	VideoInfo ret;

	// Parse geometry(s)
	{
		std::vector<std::string> geoargs;
		std::string geoarg_str = cmd.getArgument(0);
		boost::algorithm::split(geoargs, geoarg_str, boost::is_any_of(";"));
		for (int i=0;i<geoargs.size();i++) {
			ret.supportedGeometry.push_back(Geometry::fromString(geoargs.at(i)));
		}
	}

	// Parse codec information
	{
		std::vector<std::string> codecargs;
		std::string codecarg_str = cmd.getArgument(1);
		boost::algorithm::split(codecargs, codecarg_str, boost::is_any_of(";"));
		for (int i=0;i<codecargs.size();i++) {
			ret.supportedVideoCodecs.push_back(VideoCodec::fromString(codecargs.at(i)));
		}
	}

	return ret;
}

VideoCommand::SetParameterCommandHandler::
~SetParameterCommandHandler()
{
}



VideoCommand::SetParameterCommandHandler::
SetParameterCommandHandler()
:CommandHandler("SET_PARAM")
{
}



void VideoCommand::SetParameterCommandHandler::
onHandle(const Command & cmd, const CommandContext *ctx)
{
	CommandBuilder builder;
	static const char * FAILED_STR = "SET_PARAM_FAILED", * SUCCESS_STR = "SET_PARAM_OK";
	if (getInstance()->getState() == STATE_CAPTURING) {
		builder.setResponseCommand(FAILED_STR, "the video device now is capturing");
	}else if (cmd.getArguments().size() != 2) {
		builder.setResponseCommand(FAILED_STR, "invalid argument count");
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
		}

		{
			VideoCommand::Impl *d = getInstance()->d;
			d->mVideoCodec = requestCodec;
			d->mVideoGeometry = requestGeo;
		}

		builder.setResponseCommand(SUCCESS_STR);
		goto out;

		error_out:
		builder.setResponseCommand(FAILED_STR, err);
	}

	out:
	Command response;
	Commander parser(ctx->controlDevice);
	builder.build(response);
	if (!parser.writeCommand(response)) {
		Log::logError("Write response to SET_PARAM failed: %s", parser.getLastError().getErrorString().c_str());
	}
}


