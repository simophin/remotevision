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
    Impl()
    {
        init();
    }

    void init()
    {
        if(!hasInit){
            initErrors();
            hasInit = true;
        }
    }

    inline String getErrorString() const
    {
        return mErrorString;
    }

    inline void setErrorString(const String & str)
    {
        mErrorString = str;
    }

    inline Type getErrorType() const
    {
        return mErrorType;
    }

    inline syserrno_t getSystemErrno() const
    {
        return mSystemErrno;
    }

    inline void setErrorType(Type mErrorType)
    {
        this->mErrorType = mErrorType;
    }

    inline void setSystemErrno(syserrno_t mSystemErrno)
    {
        this->mSystemErrno = mSystemErrno;
    }


private:
    String mErrorString;
    syserrno_t mSystemErrno;
    Type mErrorType;

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
	return d->getErrorType();
}



void Error::setErrorType(Type t)
{
	d->setErrorString(findErrorTypeMessage(t));
	d->setErrorType(t);
	d->setSystemErrno(findSystemErrno(t));
}



void Error::setErrorType(Type t, const String & str)
{
	d->setErrorString(str);
	d->setErrorType(t);
	d->setSystemErrno(findSystemErrno(t));
}

void Error::setSystemError(syserrno_t e)
{

	d->setErrorString(findSystemErrorMessage(e));
	d->setErrorType(findErrorType(e));
	d->setSystemErrno(e);

}


void Error::setSystemError(syserrno_t e, const String & str)
{
	d->setErrorString(str);
	d->setErrorType(findErrorType(e));
	d->setSystemErrno(e);
}

String Error::getErrorString() const
{
	return d->getErrorString();
}



syserrno_t Error::getSystemError() const
{
	return d->getSystemErrno();
}


bool Error::isSuccess() const
{
	return (d->getErrorType() == ERR_SUCCESS);
}



String Error::toString() const
{
	std::stringstream stream;
	stream << "$ERRNO:" << d->getErrorType() << ";$ERRSTR:" << d->getErrorString();
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
