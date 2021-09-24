/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file Status.h
 *
 *  @brief This file contains the class that encapsulates status information.
 *
**/

#ifndef GMSEC_API_STATUS_H
#define GMSEC_API_STATUS_H

#include <gmsec4/util/wdllexp.h>
#include <gmsec4/Errors.h>
#include <gmsec4/Exception.h>

#include <gmsec4_defs.h>


namespace gmsec
{
namespace api
{

// Forward declaration(s)
namespace internal
{
	class InternalStatus;
	class StatusBuddy;
}


/** @class Status
 * @brief Returned by the API function calls to provide status feedback.  Status provides an error
 * code and error message specific to the error type, but independent of error source.
 *
 *
 * @sa Errors.h
*/
class GMSEC_API Status
{
public:
	/** @fn Status()
	 * @brief Create a status set to GMSEC_STATUS_NO_ERROR.
	 */
	Status();


	/** @fn Status(StatusClass eclass, StatusCode code, const char* text, GMSEC_I32 custom = 0)
	 * @brief Create a status with specific class, code, and string.
	 *
	 * @param eclass - error class
	 * @param code - error code
	 * @param text - error string (optional)
	 * @param custom - custom error code (optional)
	 *
	 * @sa Errors.h
	 */
	Status(StatusClass eclass, StatusCode code, const char* text = NULL, GMSEC_I32 custom = 0);


	/** @fn Status(const Exception& e)
	 * @brief Create a status with information from an Exception object.
	 *
	 * @param e - Exception object
	 */
	Status(const Exception& e);


	/** @fn Status(const Status& other)
	 * @brief Deep copy constructor.
	 */
	Status(const Status& other);


	/** @fn ~Status()
	 * @brief Destructor.
	 */
	~Status();


	/** @fn operator=(const Status& other)
	* @brief Deep copy asignment operator.
	*/
	Status& CALL_TYPE operator=(const Status& other);


	/** @fn isError() const
	 * @brief Used to determine whether the Status object is reporting an error or not.
	 * @note Only the status class is examine; the status code is not checked.
	 * @return bool - false if the status class is set to NO_ERROR; true otherwise.
	 */
	bool CALL_TYPE isError() const;


	/** @fn get() const
	 * @desc This function will return a verbose error string that contains the
	 * Status class, code, custom code and reason.  The format is as follows:@n
	 * [<i>class</i>,<i>code</i>,<i>custom code<i>] : <i>reason</i>
	 */
	const char* CALL_TYPE get() const;


	/** @fn set(StatusClass eclass, StatusCode code, const char* text = NULL, GMSEC_I32 custom = 0)
	 * @brief This is a convience function that can be used to set class, code, error text and custom code.
	 *
	 * @param eclass - error class
	 * @param code - error code
	 * @param text - error string (optional)
	 * @param custom - custom error code (optional)
	 *
	 * @sa Errors.h
	 */
	void CALL_TYPE set(StatusClass eclass, StatusCode code, const char* text = NULL, GMSEC_I32 custom = 0);


	/** @fn getClass() const
	 * @brief This function will return the error class ID.
	 *
	 * @sa Errors.h
	 */
	StatusClass CALL_TYPE getClass() const;


	/** @fn setClass(StatusClass eclass)
	 * @brief This will set the erorr class ID.
	 *
	 * @sa Errors.h
	 */
	void CALL_TYPE setClass(StatusClass eclass);


	/** @fn getCode() const
	 * @brief This will return the error code number for this status for easy comparison.
	 *
	 * @sa Errors.h
	 */
	StatusCode CALL_TYPE getCode() const;


	/** @fn setCode(StatusCode code)
	 * @brief This will set the specific error code.
	 *
	 * @sa Errors.h
	 */
	void CALL_TYPE setCode(StatusCode code);


	/** @fn getReason() const
	 * @desc This function will retrieve the string detailed description of this error.
	 */
	const char* CALL_TYPE getReason() const;


	/** @fn setReason(const char* reason)
	 * @desc This will set the error string text.
	 */
	void CALL_TYPE setReason(const char* reason);


	/** @fn getCustomCode() const
	 * @desc This will return the custom error code number of this status object.
	 */
	GMSEC_I32 CALL_TYPE getCustomCode() const;


	/** @fn setCustomCode(GMSEC_I32 code)
	 * @desc This will set the specific error code.
	 */
	void CALL_TYPE setCustomCode(GMSEC_I32 code);


	/** @fn reset()
	 * @desc This clears this Status to GMSEC_STATUS_NO_ERROR.
	 */
	void CALL_TYPE reset();


private:
	internal::InternalStatus* m_iStatus;

	friend class gmsec::api::internal::StatusBuddy;
};

} // namespace api
} // namespace gmsec

#endif
