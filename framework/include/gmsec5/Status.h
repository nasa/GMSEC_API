/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file Status.h
 *
 *  @brief This file contains the class that encapsulates status information.
 */

#ifndef GMSEC_API5_STATUS_H
#define GMSEC_API5_STATUS_H

#include <gmsec5/Errors.h>

#include <gmsec5/util/wdllexp.h>

#include <gmsec5_defs.h>


namespace gmsec
{
namespace api5
{
	class GmsecException;

// Forward declaration(s)
namespace internal
{
	class InternalStatus;
	class StatusBuddy;
}


/**
 * @class Status
 *
 * @brief Returned by the API function calls to provide status feedback. Status provides an error
 * code and error message specific to the error type, but independent of error source.
 *
 * @sa Errors.h
 */
class GMSEC_API Status
{
public:
	/**
	 * @fn Status()
	 * @brief Create a status set to GMSEC_STATUS_NO_ERROR.
	 */
	Status();


	/**
	 * @fn Status(GMSEC_I32 eclass, GMSEC_I32 ecode, const char* reason = NULL, GMSEC_I32 custom = 0)
	 *
	 * @brief Create a status with specific class, code, and string.
	 *
	 * @param eclass - error class
	 * @param ecode  - error code
	 * @param reason  - error string (optional)
	 * @param custom - custom error code (optional)
	 *
	 * @sa Errors.h
	 */
	Status(GMSEC_I32 eclass, GMSEC_I32 ecode, const char* reason = NULL, GMSEC_I32 custom = 0);


	/**
	 * @fn Status(const GmsecException& e)
	 *
	 * @brief Create a status with information from an GmsecException object.
	 *
	 * @param e - GmsecException object
	 */
	Status(const GmsecException& e);


	/**
	 * @fn Status(const Status& other)
	 * @brief Copy constructor.
	 */
	Status(const Status& other);


	/**
	 * @fn ~Status()
	 * @brief Destructor.
	 */
	~Status();


	/**
	 * @fn operator=(const Status& other)
	 * @brief Deep copy asignment operator.
	 */
	Status& CALL_TYPE operator=(const Status& other);


	/**
	 * @fn hasError() const
	 * @brief Used to determine whether the Status object is reporting an error or not.
	 * @note Only the status class is examined; the status code is not checked.
	 * @return bool - false if the status class indicates no error; true otherwise.
	 */
	bool CALL_TYPE hasError() const;


	/**
	 * @fn get() const
	 * @brief This function will return a verbose error string that contains the
	 * Status class, code, custom code and reason.  The format is as follows:@n
	 * [<i>class</i>,<i>code</i>,<i>custom code<i>] : <i>reason</i>
	 */
	const char* CALL_TYPE get() const;


	/**
	 * @fn set(GMSEC_I32 eclass, GMSEC_I32 ecode, const char* reason = NULL, GMSEC_I32 custom = 0)
	 *
	 * @brief This is a convience function that can be used to set class, code, error text and custom code.
	 *
	 * @param eclass - error class
	 * @param ecode  - error code
	 * @param reason  - error string (optional)
	 * @param custom - custom error code (optional)
	 *
	 * @sa Errors.h
	 */
	void CALL_TYPE set(GMSEC_I32 eclass, GMSEC_I32 ecode, const char* reason = NULL, GMSEC_I32 custom = 0);


	/**
	 * @fn getClass() const
	 * @brief This function will return the error class ID.
	 * @sa Errors.h
	 */
	GMSEC_I32 CALL_TYPE getClass() const;


	/**
	 * @fn setClass(GMSEC_I32 eclass)
	 * @brief This will set the erorr class ID.
	 * @sa Errors.h
	 */
	void CALL_TYPE setClass(GMSEC_I32 eclass);


	/**
	 * @fn getCode() const
	 * @brief This will return the error code number for this status for easy comparison.
	 * @sa Errors.h
	 */
	GMSEC_I32 CALL_TYPE getCode() const;


	/**
	 * @fn setCode(StatusCode ecode)
	 * @brief This will set the specific error code.
	 * @sa Errors.h
	 */
	void CALL_TYPE setCode(GMSEC_I32 ecode);


	/**
	 * @fn getReason() const
	 * @brief This function will retrieve the string detailed description of this error.
	 */
	const char* CALL_TYPE getReason() const;


	/**
	 * @fn setReason(const char* reason)
	 * @brief This will set the error string text.
	 */
	void CALL_TYPE setReason(const char* reason);


	/**
	 * @fn getCustomCode() const
	 * @brief This will return the custom error code number of this status object.
	 */
	GMSEC_I32 CALL_TYPE getCustomCode() const;


	/**
	 * @fn setCustomCode(GMSEC_I32 code)
	 * @brief This will set the specific error code.
	 */
	void CALL_TYPE setCustomCode(GMSEC_I32 code);


	/**
	 * @fn reset()
	 * @brief This clears this Status to indicate no error.
	 */
	void CALL_TYPE reset();


private:
	internal::InternalStatus* m_internal;

	friend class gmsec::api5::internal::StatusBuddy;
};

} // namespace api5
} // namespace gmsec

#endif
