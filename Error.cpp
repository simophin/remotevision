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

#include <boost/lexical_cast.hpp>

#include "Log.h"

#ifdef OS_WIN32
#include <windows.h>
#endif

syserrno_t syserrno_success = 0;

class Error::Impl {
public:
	syserrno_t errorNumber;
	errorstring_t errorString;

	Impl(syserrno_t e)
	:errorNumber(e) {

	}
};

Error::Error(syserrno_t e)
:d(new Error::Impl(e)){

}

Error::Error(const errorstring_t &str)
:d(new Error::Impl(syserrno_success)) {
	d->errorString = str;
}

Error::Error(const char *str ,size_t size)
:d(new Error::Impl(syserrno_success)) {
	std::string r;
	if (size > 0) {
		r = std::string(str,size);
	}else{
		r = std::string(str);
	}
	d->errorString = std::wstring(r.begin(), r.end());
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
		LPWSTR buf = NULL;
		::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				NULL,
				e,
				0,
				(LPSTR)&buf,
				0,
				NULL);
		d->errorString = std::wstring(buf,wcslen((LPWSTR)buf));
#endif

#ifdef OS_UNIX
		std::string tmp_str = ::strerror(e);
		d->errorString = std::wstring(tmp_str.begin(), tmp_str.end());
#endif
	}
}

errorstring_t Error::
getErrorString () const {
	return d->errorString;
}

void Error::setErrorString (const errorstring_t & str) {
	d->errorString = str;
}


std::wostream& operator <<(std::wostream &os,const Error &obj) {
	os << "errno = " << obj.getErrno() << ", error string = " << obj.getErrorString() << std::endl;
}
