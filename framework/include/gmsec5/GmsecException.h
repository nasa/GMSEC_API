/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file GmsecException.h
 *
 *  @brief This file contains the standard class for GMSEC exceptions. It holds
 *  the information previously used by Status objects to convey error states. 
 *  This is not a total replacement of the Status object, the Status object
 *  will still have a purpose for more elaborate error definitions, such as that
 *  pertaining to validation.
 *
 *  Purposely does not extend std::exception to dodge binary compatibility and
 *  cross-compiler issues
 */

#ifndef GMSEC_API5_GMSECEXCEPTION_H
#define GMSEC_API5_GMSECEXCEPTION_H

#include <gmsec5/Errors.h>

#include <gmsec5/util/wdllexp.h>

#include <gmsec5_defs.h>  // for GMSEC_I32


namespace gmsec
{
namespace api5
{
	// Forward declaration(s)
	class Status;

namespace internal
{
	class InternalGmsecException;
}


/**
 * @class GmsecException
 * @brief This class defines the standard GMSEC exception.
 */
class GMSEC_API GmsecException
{
public:
	/**
	 * @fn GmsecException(GMSEC_I32 errorClass, GMSEC_I32 errorCode, const char* errorMsg)
	 * @brief Default Constructor.
	 * @param errorClass - enumerated value indicating source of the error
	 * @param errorCode  - enumerated value indicating reason for the error
	 * @param errorMsg   - explanation for the error
	 *
	 * @sa Errors.h
	 */
	GmsecException(GMSEC_I32 errorClass, GMSEC_I32 errorCode, const char* errorMsg);


	/**
	 * @fn GmsecException(GMSEC_I32 errorClass, GMSEC_I32 errorCode, GMSEC_I32 customCode, const char* errorMsg)
	 * @brief Special constructor that can be used to set a custom error code.
	 * @param errorClass - enumerated value indicating source of the error
	 * @param errorCode  - enumerated value indicating reason for the error
	 * @param customCode - custom error code (typically middleware specific) on the root cause of the error.
	 * @param errorMsg   - explanation for the error
	 *
	 * @sa Errors.h
	 */
	GmsecException(GMSEC_I32 errorClass, GMSEC_I32 errorCode, GMSEC_I32 customCode, const char* errorMsg);


	/**
	 * @fn GmsecException(const Status& status)
	 * @brief Special constructor that uses information from a Status object.
	 * @param status - A Status object.
	 */
	GmsecException(const Status& status);


	/**
	 * @fn GmsecException(const GmsecException& other)
	 * @brief Copy constructor.
	 * @param other - the other %GmsecException object to copy
	 */
	GmsecException(const GmsecException& other);


	/**
	 * @fn ~GmsecException()
	 * @brief Destructor
	 */
	virtual ~GmsecException();


	/**
	 * @fn GmsecException& operator=(const GmsecException& other)
	 * @brief Overloaded equals operator method to allow for assignment of one %GmsecException object to another.
	 * @param other - the right-hand side %GmsecException object to use with the assignment.
	 */
	GmsecException& CALL_TYPE operator=(const GmsecException& other);


	/**
	 * @fn GMSEC_I32 getErrorClass() const
	 * @brief Returns the error class associated with the exception.
	 * @return A StatusClass enumerated value.
	 *
	 * @sa Errors.h
	 */
	GMSEC_I32 CALL_TYPE getErrorClass() const;


	/**
	 * @fn GMSEC_I32 getErrorCode() const
	 * @brief Returns the error code associated with the exception.
	 * @return A StatusCode enumerated value.
	 *
	 * @sa Errors.h
	 */
	GMSEC_I32 CALL_TYPE getErrorCode() const;


	/**
	 * @fn GMSEC_I32 getCustomCode() const
	 * @brief An error code originating from a third party library (middleware or otherwise) related to the error that occurred.
	 * @return A StatusCode enumerated value.
	 */
	GMSEC_I32 CALL_TYPE getCustomCode() const;


	/**
	 * @fn const char* getErrorMessage() const
	 * @brief Returns the error message associated with the exception.
	 * @return A string containing the error message.
	 */
	const char* CALL_TYPE getErrorMessage() const;


	/**
	 * @fn const char* what() const
	 * @brief Returns string with the format of [errorClass,errorCode,customCode] : message
	 * @return A string containing the error class, code, custom code and message.
	 *
	 * @sa Errors.h
	 */
	const char* CALL_TYPE what() const;

private:
	internal::InternalGmsecException* m_internal;
};


} //namespace api5
} //namespace gmsec

#endif
