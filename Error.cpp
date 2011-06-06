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
#include <sstream>

#include "Log.h"

#ifdef OS_WIN32
#include <windows.h>
#endif

syserrno_t syserrno_success = 0;

Error::Error(syserrno_t e)
:errorNumber(e){

}

Error::Error(const errorstring_t &str)
:errorNumber(syserrno_success) {
	errorString = str;
}

Error::Error(const char *str ,size_t size)
:errorNumber(syserrno_success) {
	std::string r;
	if (size > 0) {
		r = std::string(str,size);
	}else{
		r = std::string(str);
	}
	errorString = r;
}


Error::~Error() {
}

syserrno_t Error::getErrno() const {
	return errorNumber;
}
void Error::setErrno(syserrno_t e,bool fetchFromSystem) {
	errorNumber = e;
	if (fetchFromSystem) {
#ifdef OS_WIN32
		LPSTR buf = NULL;
		size_t size = ::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				NULL,
				e,
				LANG_ENGLISH,
				(LPSTR)&buf,
				0,
				NULL);
		errorString = std::string(buf,size);
		::free(buf);
#endif

#ifdef OS_UNIX
		errorString = ::strerror(e);
#endif
	}
}

errorstring_t Error::
getErrorString () const {
	return errorString;
}

void Error::setErrorString (const errorstring_t & str) {
	errorString = str;
}


void Error::setErrorString(const char *buf, size_t size)
{
	if (size > 0) {
		errorString = errorstring_t (buf,size);
	}else{
		errorString = buf;
	}
}

std::ostream& operator <<(std::ostream &os,const Error &obj) {
	os << "errno = " << obj.getErrno() << ", error string = " << obj.getErrorString() << std::endl;
	return os;
}

std::string Error::toString() const {
	std::stringstream stream;
	stream << "$ERRNO:" << errorNumber << ";$ERRSTR:" << errorString;
	return stream.str();

}
Error Error::fromString(const std::string &str,bool *ok) {
	syserrno_t e;
	char buf[1024];
	sscanf(str.c_str(),"$ERRNO:%d;$ERRSTR:%s", &e, buf);
	buf[sizeof(buf)-1] = '\0';
	Error ret;
	ret.setErrno(e,false);
	ret.setErrorString(buf);
	return ret;
}
