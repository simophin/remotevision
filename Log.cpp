/*
 * Log.cpp
 *
 *  Created on: 2011-5-14
 *      Author: simophin
 */

#include "Log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <iostream>

void Log::logWarning(const char *fmt, ...)
{
	va_list va;
	va_start(va,fmt);
	vLogWarning(fmt,va);
	va_end(va);
}



void Log::vLogError(const char *fmt, va_list va)
{
	vLog(LOG_ERROR,fmt,va);
}



void Log::vLogDebug(const char *fmt, va_list list)
{
	vLog(LOG_DEBUG,fmt,list);
}



void Log::logError(const char *fmt, ...)
{
	va_list va;
	va_start(va,fmt);
	vLogError(fmt,va);
	va_end(va);
}



void Log::log(Log::Level level, const char *fmt, ...)
{
	va_list va;
	va_start(va,fmt);
	vLog(level,fmt,va);
	va_end(va);
}



void Log::vLogWarning(const char *fmt, va_list list)
{
	vLog(LOG_WARNING,fmt,list);
}

void Log::vLog(Level level, const char *fmt, va_list va)
{
	const char * levelFmt = 0;
	static char buf[1024];

	switch(level){
	case LOG_WARNING: levelFmt = "WARNING:" ; break;
	case LOG_ERROR:   levelFmt = "ERROR:" ; break;
	default:
	case LOG_DEBUG:   levelFmt = "DEBUG:"; break;
	}

	vsnprintf (buf, sizeof(buf), fmt, va);
	buf[sizeof(buf)-1] = '\0';

	std::cerr << levelFmt << buf;
}

void Log::log(Level level, const std::string &str) {
	log(level, str.c_str());
}



void Log::logDebug(const char *fmt, ...)
{
	va_list va;
	va_start(va,fmt);
	vLogDebug(fmt,va);
	va_end(va);
}

