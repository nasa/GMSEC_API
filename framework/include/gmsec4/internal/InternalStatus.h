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

#ifndef GMSEC_API_INTERNAL_STATUS_H
#define GMSEC_API_INTERNAL_STATUS_H

#include <gmsec4_defs.h>

#include <gmsec4/Errors.h>
#include <gmsec4/Exception.h>
//#include <gmsec4/Message.h>     // Remove when API 3.x is deprecated
#include <gmsec4/Status.h>      // Remove when API 3.x is deprecated

#include <gmsec4/util/wdllexp.h>

#include <string>


namespace gmsec
{
namespace api
{
class Message;

namespace internal
{

class GMSEC_API InternalStatus
{
public:
	InternalStatus();


	/** @fn InternalStatus(StatusClass eclass, StatusCode code, const char* text)
	 * @brief Create a status with specific class, code, and string.
	 *
	 * @param eclass - error class
	 * @param code - error code
	 * @param text - error string (optional)
	 * @param custom - custom error code (optional)
	 *
	 * @sa Errors.h
	 */
	InternalStatus(StatusClass eclass, StatusCode code, const char* text = NULL, GMSEC_I32 custom = 0);


	InternalStatus(const Exception& e);


	/** @fn InternalStatus(const InternalStatus& other)
	 * @brief Deep copy constructor.
	 */
	InternalStatus(const InternalStatus& other);


	/** @fn ~InternalStatus()
	 * @brief Destructor.
	 */
	~InternalStatus();


	/** @fn operator=(const InternalStatus& other)
	* @brief Deep copy asignment operator.
	*/
	InternalStatus& operator=(const InternalStatus& other);


	/** @fn isError()
	 * @brief This will return TRUE if the status is indicating an error, FALSE otherwise
	 */
	bool CALL_TYPE isError() const;


	/** @fn get() const
	 * @brief This function will return a verbose error string as follows:@n
	 * [ <i>class</i>, <i>code</i>, <i>custom code<i> ]: <i>error text</i>
	 */
	const char* CALL_TYPE get() const;


	/** @fn set(StatusClass eclass, StatusCode code, const char* text)
	 * @brief This is a convience function that will set class, code, and error text.
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
	 * @brief This function will retrieve the string detailed description of this error.
	 */
	const char* CALL_TYPE getReason() const;


	/** @fn setReason(const char* reason)
	 * @brief This will set the error string text.
	 */
	void CALL_TYPE setReason(const char* reason);


	/** @fn getCustomCode() const
	 * @brief This will return the custom error code number of this status object.
	 */
	GMSEC_I32 CALL_TYPE getCustomCode() const;


	/** @fn setCustomCode(GMSEC_I32 code)
	 * @brief This will set the specific error code.
	 */
	void CALL_TYPE setCustomCode(GMSEC_I32 code);


	/** @fn reset()
	 * @brief This clears this Status to GMSEC_STATUS_NO_ERROR.
	 */
	void CALL_TYPE reset();


	// Remove the following method when the API 3.x interface is deprecated
	Message* CALL_TYPE getRequestMessage();

	// Remove the following method when the API 3.x interface is deprecated
	void CALL_TYPE setRequestMessage(const Message& request);

private:
	StatusClass m_class;
	StatusCode  m_code;
	std::string m_reason;
	GMSEC_I32   m_customCode;

	mutable std::string m_getString;

	// Remove the following declaration when the API 3.x interface is deprecated
	Message* m_reqMessage;
};


// Remove the following class when the API 3.x interface is deprecated
//
class StatusBuddy
{
public:
	static InternalStatus& getInternal(const Status& status)
	{
		return *status.m_iStatus;
	}
};

} // namespace internal
} // namespace api
} // namespace gmsec

#endif
