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
#include <algorithm>

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

	// For geometry information
	std::vector<std::string> geoargs;
	for (int i=0;i<info.supportedGeometries.size();i++) {
		const Geometry &geo = info.supportedGeometries[i];
		geoargs.push_back(geo.toString());
	}
	builder.appendArgument(boost::algorithm::join(geoargs,";"));

	// For codec information
	std::vector<std::string> codecargs;
	for (int i=0;i<info.supportedCodecs.size();i++) {
		const VideoCodec &codec = info.supportedCodecs[i];
		codecargs.push_back(codec.toString());
	}
	builder.appendArgument(boost::algorithm::join(codecargs,";"));

	Commander parser(ctx->controlDevice);
	Command response;
	builder.build(response);
	parser.writeCommand(response);
}

VideoProvider::Info VideoCommand::QueryInfoCommandHandler::
parseVideoInformationFromCommand(const Command & cmd) {
	assert(cmd.getName() == SUCCESS_STRING && cmd.getArguments().size() == 3);
	VideoProvider::Info ret;

	// Parse geometry(s)
	{
		std::vector<std::string> geoargs;
		std::string geoarg_str = cmd.getArgument(0);
		boost::algorithm::split(geoargs, geoarg_str, boost::is_any_of(";"));
		for (int i=0;i<geoargs.size();i++) {
			ret.supportedGeometries.push_back(Geometry::fromString(geoargs.at(i)));
		}
	}

	// Parse codec information
	{
		std::vector<std::string> codecargs;
		std::string codecarg_str = cmd.getArgument(1);
		boost::algorithm::split(codecargs, codecarg_str, boost::is_any_of(";"));
		for (int i=0;i<codecargs.size();i++) {
			ret.supportedCodecs.push_back(VideoCodec::fromString(codecargs.at(i)));
		}
	}

	// Parse framerates information
	{
		std::vector<std::string> ffargs;
		std::string ffarg_str = cmd.getArgument(2);
		boost::split(ffargs,ffarg_str,boost::is_any_of(";"));
		for (int i=0;i<ffargs.size();i++) {
			ret.supportedFrameRates.push_back(FrameRate::fromString(ffargs.at(i)));
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
}



VideoInfo VideoCommand::GetParameterCommandHandler::
parseInfoFromCommand(const Command & cmd)
{
}




