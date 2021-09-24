/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file Exception.h
 *
 *  @brief This file contains the standard class for GMSEC exceptions. It holds
 *  the information previously used by Status objects to convey error states. 
 *  This is not a total replacement of the Status object, the Status object
 *  will still have a purpose for more elaborate error definitions, such as that
 *  pertaining to validation.
 *
 *  Purposely does not extend std::exception to dodge binary compatibility and
 *  cross-compiler issues
 *
 */

#ifndef GMSEC_API_EXCEPTION_H
#define GMSEC_API_EXCEPTION_H

#include <gmsec4/util/wdllexp.h>
#include <gmsec4/Errors.h>

#include <gmsec4_defs.h>  // for GMSEC_I32


namespace gmsec
{
namespace api
{
// Forward declaration(s)
class Status;

namespace internal
{
	class InternalException;
}


/**
 * @class Exception
 * @brief This class defines the standard GMSEC exception.
 */
class GMSEC_API Exception
{
public:
	/**
	 * @fn Exception(StatusClass errorClass, StatusCode errorCode, const char* errorMsg)
	 * @brief Default Constructor.
	 * @param errorClass - enumerated value indicating source of the error
	 * @param errorCode  - enumerated value indicating reason for the error
	 * @param errorMsg   - explanation for the error
	 *
	 * @sa Errors.h
	 */
	Exception(StatusClass errorClass, StatusCode errorCode, const char* errorMsg);


	/**
	 * @fn Exception(StatusClass errorClass, StatusCode errorCode, GMSEC_I32 customCode, const char* errorMsg)
	 * @brief Special constructor that can be used to set a custom error code.
	 * @param errorClass - enumerated value indicating source of the error
	 * @param errorCode  - enumerated value indicating reason for the error
	 * @param customCode - custom error code (typically middleware specific) on the root cause of the error.
	 * @param errorMsg   - explanation for the error
	 *
	 * @sa Errors.h
	 */
	Exception(StatusClass errorClass, StatusCode errorCode, GMSEC_I32 customCode, const char* errorMsg);


	/**
	 * @fn Exception(const Status& status)
	 * @brief Special constructor that uses information from a Status object.
	 * @param status - A Status object.
	 */
	Exception(const Status& status);


	/**
	 * @fn Exception(const Exception& other)
	 * @brief Copy constructor.
	 * @param other - the other %Exception object to copy
	 */
	Exception(const Exception& other);


	/**
	 * @fn ~Exception()
	 * @brief Destructor
	 */
	virtual ~Exception();


	/**
	 * @fn Exception& operator=(const Exception& other)
	 * @brief Overloaded equals operator method to allow for assignment of one %Exception object to another.
	 * @param other - the right-hand side Exception object to use with the assignment.
	 */
	Exception& CALL_TYPE operator=(const Exception& other);


	/**
	 * @fn StatusClass getErrorClass() const
	 * @brief Returns the error class associated with the exception.
	 * @return A StatusClass enumerated value.
	 *
	 * @sa Errors.h
	 */
	StatusClass CALL_TYPE getErrorClass() const;


	/**
	 * @fn StatusCode getErrorCode() const
	 * @brief Returns the error code associated with the exception.
	 * @return A StatusCode enumerated value.
	 *
	 * @sa Errors.h
	 */
	StatusCode CALL_TYPE getErrorCode() const;


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
	internal::InternalException* m_internal;
};


} //namespace api
} //namespace gmsec

#endif
