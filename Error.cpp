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
#include <ostream>
#include <vector>

#include "Log.h"

#ifdef OS_WIN32
#include <windows.h>
#endif


extern std::vector<Error::SystemMap> ERROR_MAPS;
extern syserrno_t SYS_ERR_SUCCESS;
extern syserrno_t SYS_ERR_UNKNOWN;

static bool hasInit = false;
extern void initErrors();

class Error::Impl {
public:
	String mErrorString;
	syserrno_t mSystemErrno;
	Type   mErrorType;

	Impl () {
		init();
	}
	void init () {
		if (!hasInit) {
			initErrors();
			hasInit = true;
		}
	}
};

static syserrno_t findSystemErrno (Error::Type t) {
	for (std::vector<Error::SystemMap>::const_iterator iter = ERROR_MAPS.begin();
			iter != ERROR_MAPS.end(); ++ iter) {
		if (iter->type == t) return iter->system;
	}
	return SYS_ERR_UNKNOWN;
}

static Error::Type findErrorType (syserrno_t e) {
	for (std::vector<Error::SystemMap>::const_iterator iter = ERROR_MAPS.begin();
			iter != ERROR_MAPS.end(); ++ iter) {
		if (iter->system == e) return iter->type;
	}
	return Error::ERR_SYS_UNKNOWN;
}

static const char * findErrorTypeMessage (Error::Type t) {
	for (std::vector<Error::SystemMap>::const_iterator iter = ERROR_MAPS.begin();
			iter != ERROR_MAPS.end(); ++ iter) {
		if (iter->type == t) return iter->message;
	}
	return "Unknown error";
}

static const char * findSystemErrorMessage (syserrno_t e) {
	return findErrorTypeMessage(findErrorType(e));
}

Error::Error(Type t)
:d(new Impl)
{
	setErrorType(t);
}


Error::Error(Type t, const String & str)
:d(new Impl)
{
	setErrorType(t,str);
}



Error::Error(syserrno_t sys)
:d(new Impl)
{
	setSystemError(sys);
}



Error::Error(syserrno_t sys, const String & str)
:d(new Impl)
{
	setSystemError(sys,str);
}

Error::~Error () {

}

Error::Type Error::getErrorType() const
{
	return d->mErrorType;
}



void Error::setErrorType(Type t)
{

	d->mErrorType = t;
	d->mSystemErrno = findSystemErrno(t);
	d->mErrorString = findErrorTypeMessage(t);
}



void Error::setErrorType(Type t, const String & str)
{
	d->mErrorType = t;
	d->mErrorString = str;
	d->mSystemErrno = findSystemErrno(t);
}

void Error::setSystemError(syserrno_t e)
{
	d->mSystemErrno = e;
	d->mErrorString = findSystemErrorMessage(e);
	d->mErrorType = findErrorType(e);
}


void Error::setSystemError(syserrno_t e, const String & str)
{
	d->mSystemErrno = e;
	d->mErrorString = str;
	d->mErrorType = findErrorType(e);
}

String Error::getErrorString() const
{
	return d->mErrorString;
}



syserrno_t Error::getSystemError() const
{
	return d->mSystemErrno;
}


bool Error::isSuccess() const
{
	return (d->mErrorType == ERR_SUCCESS);
}



String Error::toString() const
{
	std::stringstream stream;
	stream << "$ERRNO:" << d->mErrorType << ";$ERRSTR:" << d->mErrorString;
	return stream.str();
}



Error Error::fromString(const String & str, bool *ok)
{
	Type e;
	char buf[1024];
	sscanf(str.c_str(),"$ERRNO:%d;$ERRSTR:%s", (int *)&e, buf);
	buf[sizeof(buf)-1] = '\0';
	Error ret;
	ret.setErrorType(e,buf);
	return ret;
}



void Error::setErrorString(const String & str)
{
	setErrorType(ERR_UNKNOWN,str);
}

std::ostream & operator <<(std::ostream & os, const Error & obj)
{
	os << obj.getErrorString();
	return os;
}
