/*
 * Error.cpp
 *
 *  Created on: 2011-5-14
 *      Author: simophin
 */

#include "Error.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "Log.h"

#ifdef OS_WIN32
#include <windows.h>
#endif

syserrno_t syserrno_success = 0;

class Error::Impl {
public:
	syserrno_t errorNumber;
	std::string errorString;

	Impl(syserrno_t e)
	:errorNumber(e) {

	}
};

Error::Error(syserrno_t e)
:d(new Error::Impl(e)){

}

Error::~Error() {
	delete d;
}

syserrno_t Error::getErrno() const {
	return d->errorNumber;
}
void Error::setErrno(syserrno_t e,bool fetchFromSystem) {
	d->errorNumber = e;
	if (fetchFromSystem) {
#ifdef OS_WIN32
		LPSTR buf = NULL;
		::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				NULL,
				e,
				0,
				(LPSTR)&buf,
				0,
				NULL);
		d->errorString = buf;
#endif

#ifdef OS_UNIX
		d->errorString = ::strerrno(e);
#endif
	}
}

std::string Error::
getErrorString () const {
	return d->errorString;
}

void Error::setErrorString (const std::string & str) {
	d->errorString = str;
}

std::ostream& operator <<(std::ostream &os,const Error &obj) {
	os << "errno = " << obj.getErrno() << ", error string = " << obj.getErrorString() << std::endl;
}
