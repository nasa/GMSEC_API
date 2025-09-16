/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file TicketMutex.h
 *
 * @brief This file contains the interface for FIFO mutex.
 */

#ifndef GMSEC_API5_INTERNAL_TICKETMUTEX_H
#define GMSEC_API5_INTERNAL_TICKETMUTEX_H


#include <gmsec5/util/Condition.h>
#include <gmsec5/util/wdllexp.h>


namespace gmsec
{
namespace api5
{
namespace internal
{

class GMSEC_API TicketMutex
{
public:
	TicketMutex();

	~TicketMutex();

	/**
	 * @fn enter()
	 * @brief Acquires the lock.
	 */
	void CALL_TYPE enter();

	/**
	 * @fn leave()
	 * @brief Releases the lock.
	 */
	void CALL_TYPE leave();

private:
	TicketMutex(const TicketMutex&);
	TicketMutex& operator=(const TicketMutex&);

	gmsec::api5::util::Condition condition;
	GMSEC_U32                    head;
	GMSEC_U32                    tail;
};


/**
 * @class AutoTicket
 * @brief RAII interface to TicketMutex.
 */
class GMSEC_API AutoTicket
{
public:
	AutoTicket(TicketMutex &m, bool acquire = true);

	~AutoTicket();

	/**
	 * @fn enter()
	 * @brief Acquire the mutex.
	 */
	void enter();

	/**
	 * @fn leave()
	 * @brief Release the mutex.
	 */
	void leave();

private:
	// Declared, but not implemented
	AutoTicket();
	AutoTicket& operator=(const AutoTicket& other);

	TicketMutex& ticketer;
	bool         owned;
};


} // namespace internal
} // namespace api5
} // namespace gmsec

#endif
