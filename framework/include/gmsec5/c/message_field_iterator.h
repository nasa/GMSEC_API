/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file message_field_iterator.h
 *
 *  @brief This file contains functions for the management of a MessageFieldIterator object.
 *
 * Example creation and use:
 * @code
 * GMSEC_Status               status = statusCreate();
 * GMSEC_Message              msg    = messageFactoryCreateMessage(factory, "MSG.HB", status);
 * GMSEC_MessageFieldIterator iter   = messageGetFieldIterator(msg, status);
 *
 * while (messageFieldIteratorHasNext(iter, status) == GMSEC_TRUE)
 * {
 *     const GMSEC_Field field = messageFieldIteratorNext(iter, status);
 *
 *     // do something with field
 * }
 * @endcode
 */


#ifndef GMSEC_API5_C_MESSAGE_FIELD_ITERATOR_H
#define GMSEC_API5_C_MESSAGE_FIELD_ITERATOR_H

#include <gmsec5_defs.h>

#include <gmsec5/util/wdllexp.h>


#ifdef __cplusplus
extern "C"
{
#endif


	/**
	 * @fn GMSEC_BOOL messageFieldIteratorHasNext(GMSEC_MessageFieldIterator iter, GMSEC_Status status)
	 *
	 * @brief Provides information as to whether there are additional fields that can be referenced using next().
	 *
	 * @param[in]  iter   - the handle to the MessageFieldIterator object.
	 * @param[out] status - out parameter operation result status
	 *
	 * @return GMSEC_TRUE if additional fields are available, GMSEC_FALSE otherwise.
	 *
	 * @sa messageGetMessageFieldIterator()
	 */
	GMSEC_API GMSEC_BOOL messageFieldIteratorHasNext(GMSEC_MessageFieldIterator iter, GMSEC_Status status);


	/**
	 * @fn const GMSEC_Field messageFieldIteratorNext(GMSEC_MessageFieldIterator iter, GMSEC_Status status)
	 *
	 * @brief Returns a handle to the next available Field object.
	 *
	 * @param[in]  iter   - the handle to the MessageFieldIterator object.
	 * @param[out] status - out parameter operation result status
	 *
	 * @return A handle to a field object, or NULL if a field can be referenced.  For the latter case, examine the status.
	 *
	 * @note Do NOT destroy the returned field object.
	 *
	 * @sa messageGetMessageFieldIterator()
	 * @sa messageFieldIteratorHasNext()
	 */
	GMSEC_API const GMSEC_Field messageFieldIteratorNext(GMSEC_MessageFieldIterator iter, GMSEC_Status status);


	/**
	 * @fn void messageFieldIteratorReset(GMSEC_MessageFieldIterator iter, GMSEC_Status status)
	 *
	 * @brief Resets the interator to the beginning of the Field list that is maintained by the Message.
	 *
	 * @param[in]  iter   - the handle to the MessageFieldIterator object.
	 * @param[out] status - out parameter operation result status
	 *
	 * @sa messageGetMessageFieldIterator()
	 * @sa messageFieldIteratorHasNext()
	 * @sa messageFieldIteratorNext()
	 */
	GMSEC_API void messageFieldIteratorReset(GMSEC_MessageFieldIterator iter, GMSEC_Status status);


#ifdef __cplusplus
}	// extern "C"
#endif

#endif
