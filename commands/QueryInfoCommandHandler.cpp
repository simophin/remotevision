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

#include <string>
#include <vector>
#include <boost/algorithm/string/join.hpp>
#include <boost/lexical_cast.hpp>

QueryInfoCommandHandler::QueryInfoCommandHandler() {
}

QueryInfoCommandHandler::~QueryInfoCommandHandler() {
}

void QueryInfoCommandHandler::
onHandle(const Command &cmd, const CommandContext * ctx) {
	VideoInfo info = ctx->videoProvider->queryInfo();
	CommandBuilder builder;
	builder.setResponseCommand("OK");

	// For geometry information
	std::vector<std::string> geoargs;
	for (int i=0;i<info.supportedGeometry.size();i++) {
		const Geometry &geo = info.supportedGeometry[i];
		std::string g;
		g += ( boost::lexical_cast<std::string>(geo.height) + "," + boost::lexical_cast<std::string>(geo.width));
		geoargs.push_back(g);
	}
	builder.appendArgument(boost::algorithm::join(geoargs,";"));

	// For codec information
	std::vector<std::string> codecargs;
	for (int i=0;i<info.supportedVideoCodecs.size();i++) {
		const VideoCodec &codec = info.supportedVideoCodecs[i];
		std::string c;
		c += VideoCodec::getStringFromId(codec.codecId);
		codecargs.push_back(c);
	}
	builder.appendArgument(boost::algorithm::join(codecargs,";"));

	Commander parser(ctx->controlDevice);
	Command response;
	builder.build(response);
	parser.writeCommand(response);
}
