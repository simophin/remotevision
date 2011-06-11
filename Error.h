/*
 * Error.h
 *
 *  Created on: 2011-5-14
 *      Author: simophin
 */

#ifndef ERROR_H_
#define ERROR_H_


#include "RString.h"
#include "utils/SharedPtr.hpp"

#include <iostream>

typedef int syserrno_t;

class Error {
public:
	enum Type {
		ERR_SUCCESS,
		ERR_UNKNOWN,
		ERR_SYS_UNKNOWN,
		ERR_STATE,
		ERR_TIMEOUT,
		ERR_INVALID,
		ERR_ADDRINUSE
	};

	Error (Type t = ERR_SUCCESS);
	explicit Error (Type t, const String &str);
	explicit Error (syserrno_t sys);
	explicit Error (syserrno_t sys, const String &str);

	virtual ~Error ();

	Type getErrorType() const;
	void setErrorType (Type t);
	void setErrorType (Type t, const String &str);
	String getErrorString() const;

	void setErrorString (const String &str);

	syserrno_t getSystemError () const;
	void setSystemError (syserrno_t);
	void setSystemError (syserrno_t, const String &);

	bool isSuccess () const;
	inline bool isError() const {
		return !isSuccess();
	}

	String toString() const;
	static Error fromString(const String &,bool *ok = NULL);

	friend std::ostream& operator <<(std::ostream &os,const Error &obj);

	class SystemMap {
	public:
		syserrno_t system;
		Type type;
		const char * message;
	};

private:
	class Impl;
	Utils::SharedPtr<Impl> d;
};





#endif /* ERROR_H_ */
