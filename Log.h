/*
 * Log.h
 *
 *  Created on: 2011-5-14
 *      Author: simophin
 */

#ifndef LOG_H_
#define LOG_H_

#include <stdarg.h>
#include "RString.h"

class Log {
public:
	typedef enum {
		LOG_ERROR = 0,
		LOG_WARNING,
		LOG_DEBUG
	} Level;


	static void logError (const char *, ...);
	static void vLogError (const char *, va_list);
	static void logWarning (const char *, ...);
	static void vLogWarning (const char *, va_list);
	static void logDebug (const char *, ...);
	static void vLogDebug (const char *, va_list);

	static void log (Level, const char *, ...);
	static void log (Level, const String &);
	static void vLog (Level, const char *, va_list);
};

#endif /* LOG_H_ */
