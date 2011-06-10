/*
 * PosixSocketDemuxer.cpp
 *
 *  Created on: 2011-6-10
 *      Author: simophin
 */

#include "PosixSocketDemuxer.h"

#include "PosixCompactHeader.h"
#include "PosixSocket.h"
#include "Error.h"

#include <sys/select.h>
#include <errno.h>

class PosixSocketDemuxer::Impl {
public:
	std::vector<PosixSocket *> mSockets [3];
	int maxFd;

	Impl()
	:maxFd(0) {

	}
};

PosixSocketDemuxer::PosixSocketDemuxer()
:d(new Impl){

}

PosixSocketDemuxer::~PosixSocketDemuxer() {
	delete d;
}

Error PosixSocketDemuxer::waitEvent(std::vector<PosixSocket*> &readAvail,
		std::vector<PosixSocket*> &writeAvail, std::vector<PosixSocket*> &errorAvail, int ms)
{
	Error ec;
	fd_set fds[3];
	std::vector<PosixSocket *> *result[] = { &readAvail, &writeAvail, &errorAvail };

	for (int i=0; i<3; i++) {
		FD_ZERO(fds + i);
		for (unsigned j=0; j<d->mSockets[i].size(); j++) {
			FD_SET(d->mSockets[i][j]->getFileDescriptor(), fds + i);
		}
	}

	struct timeval tv, *ptv = 0;

	if (ms > 0) {
		tv.tv_sec = 0;
		tv.tv_usec = ms*1000;
		ptv = &tv;
	}

	int rc = ::select(d->maxFd, fds , fds + 1, fds + 2, ptv);

	if (rc == 0) {
		ec.setErrorType(Error::ERR_TIMEOUT);
		return ec;
	}else if (rc < 0 ){
		ec.setSystemError(errno);
	}else{
		for (int i=0; i< 3; i++) {
			for (unsigned j=0; j<d->mSockets[i].size(); j++) {
				if (FD_ISSET(d->mSockets[i][j]->getFileDescriptor(), fds+i)) {
					result[i]->push_back(d->mSockets[i][j]);
				}
			}
		}
	}

	return ec;
}



void PosixSocketDemuxer::addSocket(PosixSocket  * const psock , int s)
{
	if (s & SELECT_READ) {
		d->mSockets[0].push_back(psock);
	}
	if (s & SELECT_WRITE) {
		d->mSockets[1].push_back(psock);
	}
	if (s & SELECT_ERROR) {
		d->mSockets[2].push_back(psock);
	}

	if (psock->getFileDescriptor() + 1 >= d->maxFd) {
		d->maxFd = psock->getFileDescriptor() + 1;
	}
}




std::vector<PosixSocket*> PosixSocketDemuxer::getAllSockets(SelectType s) const
{
	return d->mSockets[s];
}



