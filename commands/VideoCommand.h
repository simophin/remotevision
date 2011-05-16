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
	VideoCommand();
	virtual ~VideoCommand();

	typedef enum {
		STATE_READY,
		STATE_CAPTURING
	} State;

	State getState() const;
	void setState(State);

	class QueryInfoCommandHandler: public CommandHandler {
	public:
		QueryInfoCommandHandler();
		virtual ~QueryInfoCommandHandler();
		virtual void onHandle(const Command &, const CommandContext *);

		static VideoInfo parseVideoInformationFromCommand(const Command &);
	};

	class SetParameterCommandHandler: public CommandHandler {
	public:
		SetParameterCommandHandler ();
		virtual ~SetParameterCommandHandler();
		virtual void onHandle(const Command &, const CommandContext *);

		static void buildRequestCommand (Command &,const Geometry &, const VideoCodec &);
	};

	friend class QueryInfoCommandHandler;
	friend class SetParameterCommandHandler;

	class Impl;
protected:
	static VideoCommand *getInstance();
private:
	Impl *d;
};

#endif /* VIDEOCOMMAND_H_ */
