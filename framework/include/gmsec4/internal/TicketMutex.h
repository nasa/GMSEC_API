/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file TicketMutex.h
 *
 *  @brief This file contains the interface for FIFO mutex.
 */

#ifndef GMSEC_API_UTIL_TICKETMUTEX_H
#define GMSEC_API_UTIL_TICKETMUTEX_H


#include <gmsec4/util/Condition.h>
#include <gmsec4/util/wdllexp.h>


namespace gmsec {
namespace api {
namespace util {


class GMSEC_API TicketMutex
{
public:
	TicketMutex();
	~TicketMutex();

	/**	@fn enter()
	 *	@brief Acquires the lock.
	**/
	void CALL_TYPE enter();

	/**	@fn leave()
	 *	@brief Releases the lock.
	**/
	void CALL_TYPE leave();

private:
	TicketMutex(const TicketMutex &) /* = delete */ ;
	TicketMutex &operator=(const TicketMutex &) /* = delete */ ;

	Condition condition;
	GMSEC_U32 head;
	GMSEC_U32 tail;
};


/*
 * class AutoTicket
 * @brief RAII interface to TicketMutex.
**/
class GMSEC_API AutoTicket
{
public:
	AutoTicket(TicketMutex &m, bool acquire = true);
	~AutoTicket();

	/**	@fn enter()
	 *	@brief Acquire the mutex.
	 *
	**/
	void enter();

	/**	@fn leave()
	 *	@brief Release the mutex.
	**/
	void leave();

private:
	AutoTicket() /* = delete */;
	AutoTicket& operator= (const AutoTicket &other) /* = delete */;

	TicketMutex &ticketer;
	bool owned;
};


} // namespace util
} // namespace api
} // namespace gmsec

#endif
