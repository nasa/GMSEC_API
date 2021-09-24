/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file GarbageCollector.h
 *
 *  @brief This object allocates object needed to manage the C API layer, and
 *  cleans up (or garbage collects) as needed.
 *
 *
 */

#ifndef gmsec_internal_GarbageCollector_h
#define gmsec_internal_GarbageCollector_h

#include <gmsec_defs.h>
#include <gmsec/util/wdllexp.h>

#include <map>
#include <vector>


namespace gmsec
{

class Connection;
class Callback;
class ErrorCallback;
class ReplyCallback;

namespace api
{
class Callback;
class EventCallback;
class ReplyCallback;
}

namespace internal
{

class CAPICallback;
class CAPIErrorCallback;
class CAPIReplyCallback;
class CAPILogHandler;

/**	@class GarbageCollector
 *
 *	@brief This class is responsible for managing the CAPI callbacks.
 *
**/
class GMSEC_API GarbageCollector
{
private:
	// Declared, but not implemented.
	GarbageCollector(const GarbageCollector&);
	GarbageCollector& operator=(const GarbageCollector&);

	typedef std::map<GMSEC_CALLBACK*, CAPICallback*>            cbAPILkp;
	typedef cbAPILkp::const_iterator                            cbAPILkpItr;

	typedef std::map<GMSEC_ERROR_CALLBACK*, CAPIErrorCallback*> cbErrorLkp;
	typedef cbErrorLkp::const_iterator                          cbErrorLkpItr;

	typedef std::map<GMSEC_REPLY_CALLBACK*, CAPIReplyCallback*> cbReplyLkp;
	typedef cbReplyLkp::const_iterator                          cbReplyLkpItr;

	typedef std::map<gmsec::ErrorCallback*, gmsec::api::EventCallback*> EventCallbackLookup;
	typedef std::map<gmsec::Callback*,      gmsec::api::ReplyCallback*> DeprecatedReplyCallbackLookup;
	typedef std::map<gmsec::ReplyCallback*, gmsec::api::ReplyCallback*> ReplyCallbackLookup;


	cbAPILkp                      fCbAPILkps;
	cbErrorLkp                    fCbErrorLkps;
	cbReplyLkp                    fCbReplyLkps;

	EventCallbackLookup           m_eventCallbackLookup;
	DeprecatedReplyCallbackLookup m_depReplyCallbackLookup;
	ReplyCallbackLookup           m_replyCallbackLookup;


public:
	GarbageCollector();


	/**	@fn ~GarbageCollector()
	 *
	 *	@brief Destroys the garbage collector and all associated callbacks.
	 */
	virtual ~GarbageCollector();


	/**	@fn newCAPICallback(GMSEC_CALLBACK* cb)
	 *
	 *	@brief Creates a new CAPI callback if the callback does not already exist
	 */
	CAPICallback* newCAPICallback(GMSEC_CALLBACK* cb);


	/**	@fn deleteCAPICallback(GMSEC_CALLBACK* cb)
	 *
	 *	@brief Deletes the referenced CAPI callback
	 */
	void deleteCAPICallback(GMSEC_CALLBACK* cb);


	/**	@fn getCAPICallback(GMSEC_CALLBACK* cb)
	 *
	 *	@brief Retreives the CAPI callback associated with the referenced GMSEC callback
	 */
	CAPICallback* getCAPICallback(GMSEC_CALLBACK* cb);


	/**	@fn newCAPIErrorCallback(GMSEC_ERROR_CALLBACK* cb)
	 *
	 *	@brief Creates a new CAPI error callback if the callback does not already exist
	 */
	CAPIErrorCallback* newCAPIErrorCallback(GMSEC_ERROR_CALLBACK* cb);


	/**	@fn deleteCAPIErrorCallback(GMSEC_CALLBACK* cb)
	 *
	 *	@brief Deletes the referenced CAPI error callback
	 */
	void deleteCAPIErrorCallback(GMSEC_ERROR_CALLBACK* cb);


	/**	@fn getCAPIErrorCallback(GMSEC_CALLBACK* cb)
	 *
	 *	@brief Retreives the CAPI error callback associated with the referenced
	 *  GMSEC error callback
	 */
	CAPIErrorCallback* getCAPIErrorCallback(GMSEC_ERROR_CALLBACK* cb);


	/**	@fn newCAPIReplyCallback(GMSEC_ERROR_CALLBACK* cb)
	 *
	 *	@brief Creates a new CAPI reply callback if the callback does not already exist
	 */
	CAPIReplyCallback* newCAPIReplyCallback(GMSEC_REPLY_CALLBACK* cb, GMSEC_ERROR_CALLBACK* er);


	/**	@fn deleteCAPIReplyCallback(GMSEC_CALLBACK* cb)
	 *
	 *	@brief Deletes the referenced CAPI reply callback
	 */
	void deleteCAPIReplyCallback(GMSEC_REPLY_CALLBACK* cb);


	/**	@fn getCAPIReplyCallback(GMSEC_CALLBACK* cb)
	 *
	 *	@brief Retreives the CAPI reply callback associated with the referenced
	 *  GMSEC error callback
	 */
	CAPIReplyCallback* getCAPIReplyCallback(GMSEC_REPLY_CALLBACK* cb);


	// Supports 3.x ErrorCallback to 4.x EventCallback
	//
	gmsec::api::EventCallback* newEventCallback(gmsec::Connection* conn, gmsec::ErrorCallback* ecb);
	void                       delEventCallback(gmsec::ErrorCallback* ecb);
	gmsec::api::EventCallback* getEventCallback(gmsec::ErrorCallback* ecb);


	// Supports 3.x Callback to 4.x ReplyCallback (deprecated)
	//
	gmsec::api::ReplyCallback* newReplyCallback(gmsec::Connection* conn, gmsec::Callback* cb);
	void                       delReplyCallback(gmsec::Callback* cb);
	gmsec::api::ReplyCallback* getReplyCallback(gmsec::Callback* cb);


	// Supports 3.x ReplyCallback to 4.x ReplyCallback
	//
	gmsec::api::ReplyCallback* newReplyCallback(gmsec::Connection* conn, gmsec::ReplyCallback* rcb);
	void                       delReplyCallback(gmsec::ReplyCallback* rcb);
	gmsec::api::ReplyCallback* getReplyCallback(gmsec::ReplyCallback* rcb);
};

} //namespace internal
} //namespace gmsec

#endif /* gmsec_internal_GarbageCollector_h */
