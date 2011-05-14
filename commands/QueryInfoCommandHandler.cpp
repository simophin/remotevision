/*
 * QueryInfoCommandHandler.cpp
 *
 *  Created on: 2011-5-4
 *      Author: simophin
 */

#include "QueryInfoCommandHandler.h"
#include "Command.h"
#include "CommandContext.h"
#include "VideoInfo.h"
#include "VideoProvider.h"
#include "CommandBuilder.h"
#include "IODevice.h"
#include "Commander.h"
#include "Geometry.h"

#include <string>
#include <vector>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/lexical_cast.hpp>

#include <assert.h>

QueryInfoCommandHandler::QueryInfoCommandHandler() {
}

QueryInfoCommandHandler::~QueryInfoCommandHandler() {
}

void QueryInfoCommandHandler::
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

VideoInfo QueryInfoCommandHandler::
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
		std::string codecarg_str = cmd.getArgument(0);
		boost::algorithm::split(codecargs, codecarg_str, boost::is_any_of(";"));
		for (int i=0;i<codecargs.size();i++) {
			ret.supportedVideoCodecs.push_back(VideoCodec::fromString(codecargs.at(i)));
		}
	}

	return ret;
}
