/*
 * VideoCommand.h
 *
 *  Created on: 2011-5-16
 *      Author: launliu
 */

#ifndef VIDEOCOMMAND_H_
#define VIDEOCOMMAND_H_

#include "CommandHandler.h"

class VideoInfo;
class Command;
class Geometry;
class VideoCodec;
class VideoCommand {
public:
	class QueryInfoCommandHandler: public CommandHandler {
	public:
		QueryInfoCommandHandler();
		virtual ~QueryInfoCommandHandler();
		virtual void onHandle(const Command &, const CommandContext *);

		static VideoInfo parseVideoInformationFromCommand(const Command &);
		static const char * SUCCESS_STRING;
		static const char * ERROR_STRING;
		static const char * REQUEST_STRING;
	};

	class SetParameterCommandHandler: public CommandHandler {
	public:
		SetParameterCommandHandler ();
		virtual ~SetParameterCommandHandler();
		virtual void onHandle(const Command &, const CommandContext *);

		static void buildRequestCommand (Command &,const Geometry &, const VideoCodec &);
		static const char * SUCCESS_STRING;
		static const char * ERROR_STRING;
		static const char * REQUEST_STRING;
	};
};

#endif /* VIDEOCOMMAND_H_ */
