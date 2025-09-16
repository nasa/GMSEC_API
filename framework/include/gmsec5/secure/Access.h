/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Access.h
 * @brief Holds declaration of AbstractAccess interface.
 */

#ifndef GMSEC_API5_SECURE_ACCESS_H
#define GMSEC_API5_SECURE_ACCESS_H


#include <gmsec5/Status.h>
#include <gmsec5/Config.h>

#include <gmsec5/util/wdllexp.h>


namespace gmsec {
namespace api5 {
namespace secure {

using gmsec::api5::Status;
using gmsec::api5::Config;


class AbstractAccess;


class GMSEC_API Access
{
public:

	enum Mode
	{
		OPEN,
		ID_KEY,
		PKI_CERT,
		LIMIT
	};

	Access ();
	~Access ();

	bool CALL_TYPE isValid () const;
	const char * CALL_TYPE getID () const;
	bool CALL_TYPE isMiddleware () const;
	Mode CALL_TYPE getMode () const;

	Status CALL_TYPE initialize (const Config &config);

	/** @method reset ()
		Clear any cached information.
	*/
	Status CALL_TYPE reset ();

	/*
		authenticate/canSubscribe/canSend are only invoked if
		isMiddleware() == false.
	*/
	Status CALL_TYPE authenticate ();
	Status CALL_TYPE canSubscribe (const char *subject);
	Status CALL_TYPE canSend (const char *subject);

private:

	// Declared, but not implemented.
	Access (const Access &other);
	Access &operator= (const Access &other);

	friend class AbstractAccess;

	AbstractAccess *ptr;
};



class GMSEC_API AbstractAccess
{
public:

	virtual ~AbstractAccess ();

	virtual bool CALL_TYPE isValid () const = 0;
	virtual const char * CALL_TYPE getID () const = 0;
	virtual bool CALL_TYPE isMiddleware () const = 0;
	virtual Access::Mode CALL_TYPE getMode () const = 0;

	virtual Status CALL_TYPE initialize (const Config &config) = 0;

	virtual Status CALL_TYPE reset () = 0;

	virtual Status CALL_TYPE authenticate () = 0;
	virtual Status CALL_TYPE canSubscribe (const char *subject) = 0;
	virtual Status CALL_TYPE canSend (const char *subject) = 0;

	void CALL_TYPE setExternal (Access &object);
};


class GMSEC_API OpenAccess : public AbstractAccess
{
public:

	virtual bool CALL_TYPE isValid () const;
	virtual const char * CALL_TYPE getID () const;
	virtual bool CALL_TYPE isMiddleware () const;
	virtual Access::Mode CALL_TYPE getMode () const;

	virtual Status CALL_TYPE initialize (const Config &config);

	virtual Status CALL_TYPE reset ();

	virtual Status CALL_TYPE authenticate ();
	virtual Status CALL_TYPE canSubscribe (const char *subject);
	virtual Status CALL_TYPE canSend (const char *subject);

private:
	Status status;
};


class GMSEC_API MiddlewareAccess : public AbstractAccess
{
public:
	virtual bool CALL_TYPE isValid () const;
	virtual const char * CALL_TYPE getID () const;
	virtual bool CALL_TYPE isMiddleware () const;
	virtual Access::Mode CALL_TYPE getMode () const;

	virtual Status CALL_TYPE initialize (const Config &config);

	virtual Status CALL_TYPE reset ();

	virtual Status CALL_TYPE authenticate ();
	virtual Status CALL_TYPE canSubscribe (const char *subject);
	virtual Status CALL_TYPE canSend (const char *subject);

private:
	Status status;
	Access::Mode fMode;
};


#if 0

// The GMSEC server access implementation.  Move to library.

class SimpleAccess : public AbstractAccess
{
public:

	virtual bool isValid () const;
	virtual const char * getID () const;
	virtual bool isMiddleware () const;
	virtual Access::Mode getMode () const;

	virtual Status initialize (const Config &config);

	virtual Status reset ();

	virtual Status authenticate ();
	virtual Status canSubscribe (const char *subject);
	virtual Status canSend (const char *subject);

private:

	Status status;
	Config config;
	bool middleware;
	Access::Mode mode;

	std::map<std::string, bool> cacheSubscribe;
	std::map<std::string, bool> cacheSend;
};

#endif


} // namespace secure
} // namespace api5
} // namespace gmsec

#endif
