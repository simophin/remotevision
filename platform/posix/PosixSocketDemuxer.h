/*
 * PosixSocketDemuxer.h
 *
 *  Created on: 2011-6-10
 *      Author: simophin
 */

#ifndef POSIXSOCKETDEMUXER_H_
#define POSIXSOCKETDEMUXER_H_

#include <vector>
#include "Error.h"
#include "utils/NonCopyable.hpp"


class PosixSocket;
class PosixSocketDemuxer: public Utils::NonCopyable {
public:
	enum SelectType{
		SELECT_READ = 1,
		SELECT_WRITE = 2,
		SELECT_ERROR = 4
	};

	PosixSocketDemuxer();
	virtual ~PosixSocketDemuxer();

	Error waitEvent ( std::vector<PosixSocket *> & readAvail,
			std::vector<PosixSocket *> & writeAvail,
			std::vector<PosixSocket *> & errorAvail, int ms = -1);

	void addSocket (PosixSocket  * const, int s);
	std::vector<PosixSocket *> getAllSockets(SelectType s) const;
	void clearSockets() ;

private:
	class Impl;
	Impl *d;
};

#endif /* POSIXSOCKETDEMUXER_H_ */
