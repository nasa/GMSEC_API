/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file Access.h
	Holds declaration of AbstractAccess interface.
*/

#ifndef GMSEC_API_SECURE_ACCESS_H
#define GMSEC_API_SECURE_ACCESS_H


#include <gmsec4/Status.h>
#include <gmsec4/Config.h>

#include <gmsec4/util/wdllexp.h>


namespace gmsec {
namespace api {
namespace secure {

using gmsec::api::Status;
using gmsec::api::Config;


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

	bool isValid () const;
	const char * getID () const;
	bool isMiddleware () const;
	Mode getMode () const;

	Status initialize (const Config &config);

	/** @method reset ()
		Clear any cached information.
	*/
	Status reset ();

	/*
		authenticate/canSubscribe/canSend are only invoked if
		isMiddleware() == false.
	*/
	Status authenticate ();
	Status canSubscribe (const char *subject);
	Status canSend (const char *subject);

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

	virtual bool isValid () const = 0;
	virtual const char * getID () const = 0;
	virtual bool isMiddleware () const = 0;
	virtual Access::Mode getMode () const = 0;

	virtual Status initialize (const Config &config) = 0;

	virtual Status reset () = 0;

	virtual Status authenticate () = 0;
	virtual Status canSubscribe (const char *subject) = 0;
	virtual Status canSend (const char *subject) = 0;

	void setExternal (Access &object);

};


class GMSEC_API OpenAccess : public AbstractAccess
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
};


class GMSEC_API MiddlewareAccess : public AbstractAccess
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
} // namespace api
} // namespace gmsec

#endif
