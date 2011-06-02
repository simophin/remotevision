/*
 * RemoteVision.h
 *
 *  Created on: 2011-5-5
 *      Author: launliu
 */

#ifndef REMOTEVISION_H_
#define REMOTEVISION_H_

#include <string>

class CommandMgr;
class VideoProvider;
class RemoteVisionApp {
public:
	RemoteVisionApp(const std::string &filename);
	virtual ~RemoteVisionApp();

	VideoProvider *getProvider() const;
	CommandMgr *   getCommandMgr() const;

	static RemoteVisionApp * getInstance();

	class Impl;
private:
	Impl *d;
};

#endif /* REMOTEVISION_H_ */
