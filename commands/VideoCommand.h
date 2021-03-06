/*
 * VideoCommand.h
 *
 *  Created on: 2011-5-16
 *      Author: launliu
 */

#ifndef VIDEOCOMMAND_H_
#define VIDEOCOMMAND_H_

#include "CommandHandler.h"
#include "VideoProvider.h"

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

		static VideoProvider::Info parseVideoInformationFromCommand(const Command &);
		static const char * SUCCESS_STRING;
		static const char * ERROR_STRING;
		static const char * REQUEST_STRING;
	};

	class GetParameterCommandHandler: public CommandHandler {
	public:
		GetParameterCommandHandler();
		virtual ~GetParameterCommandHandler() {}
		virtual void onHandle(const Command &, const CommandContext *);

		static VideoInfo parseInfoFromCommand (const Command &);
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

	class StartCaptureCommandHandler: public CommandHandler {
	public:
		StartCaptureCommandHandler();
		virtual ~StartCaptureCommandHandler();
		virtual void onHandle(const Command &, const CommandContext *);

		static const char * SUCCESS_STRING;
		static const char * ERROR_STRING;
		static const char * REQUEST_STRING;
	};

	class StopCaptureCommandHandler: public CommandHandler {
	public:
		StopCaptureCommandHandler();
		virtual ~StopCaptureCommandHandler();
		virtual void onHandle(const Command &, const CommandContext *);

		static const char * SUCCESS_STRING;
		static const char * ERROR_STRING;
		static const char * REQUEST_STRING;
	};
};

#endif /* VIDEOCOMMAND_H_ */
