
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

#ifndef gmsec_Status_h
#define gmsec_Status_h


#include <gmsec_defs.h>
#include <gmsec/util/Deprecated.h>
#include <gmsec/util/wdllexp.h>


namespace gmsec
{

namespace internal
{
class BaseStatus;
}


/** @class Status
 * @brief Returned by the API function calls to provide status feedback.  Status provides an error
 * code and error message specific to the error type, but independent of error source.
 *
 *
 * @sa gmsec_errors.h
*/
class GMSEC_API Status
{
public:
	/** @fn Status()
	 * @brief Create a status set to GMSEC_STATUS_NO_ERROR.
	 */
	GMSEC_DEPRECATED Status();

	/** @fn Status(GMSEC_STATUS_CLASS eclass, unsigned int code, const char *text)
	 * @brief Create a status with specific class, code, and string.
	 *
	 * @param eclass - error class
	 * @param code - error code
	 * @param text - error string
	 */
	GMSEC_DEPRECATED Status(GMSEC_STATUS_CLASS eclass, unsigned int code, const char *text);

	/** @fn Status( const Status &st)
	 * @brief Deep copy constructor.
	 */
	GMSEC_DEPRECATED Status(const Status &st);

	/** @fn ~Status()
	 * @brief Destructor.
	 */
	~Status();

	/** @fn operator=( const Status &st )
	* @brief Deep copy asignment operator.
	*/
	Status operator=(const Status &st);

	/** @fn GetString() const
	 * @brief This function will retrieve the string detailed description of this error.
	 *
	 */
	const char * CALL_TYPE GetString() const;

	/** @fn Get() const
	 * @brief This function will return a verbose error string as follows:@n
	 * [ <i>class</i>, <i>code</i> ]: <i>error text</i>
	 *
	 */
	const char * CALL_TYPE Get() const;

	/** @fn GetClass() const
	 * @brief This function will return the error class ID.
	 *
	 * @sa gmsec_errors.h
	 */
	GMSEC_STATUS_CLASS CALL_TYPE GetClass() const;

	/** @fn GetCode() const
	 * @brief This will return the error code number for this status for easy comparison.
	 *
	 * @sa gmsec_errors.h
	 */
	unsigned int CALL_TYPE GetCode() const;

	/** @fn GetCustomCode() const
	 * @brief This will return the custom error code number of this status object.
	 * @sa gmsec_errors.h
	 */
	GMSEC_I32 CALL_TYPE GetCustomCode() const;


	/** @fn isError()
	 * @brief This will return TRUE if the status is indicating an error, FALSE otherwise
	 * @note This method has been deprecated; use IsError() instead.
	 */
	int CALL_TYPE isError();
	int CALL_TYPE isError() const;

	/** @fn IsError()
	 * @brief This will return TRUE if the status is indicating an error, FALSE otherwise
	 */
	int CALL_TYPE IsError();
	int CALL_TYPE IsError() const;

	/** @fn SetString(const char *text)
	 * @brief This will set the error string.
	 */
	void CALL_TYPE SetString(const char *text);

	/** @fn SetClass(GMSEC_STATUS_CLASS eclass)
	 * @brief This will set the erorr class ID.
	 */
	void CALL_TYPE SetClass(GMSEC_STATUS_CLASS eclass);

	/** @fn SetCode(unsigned int code)
	 * @brief This will set the specific error code.
	 */
	void CALL_TYPE SetCode(unsigned int code);

	/** @fn SetCustomCode(GMSEC_I32 code)
	 * @brief This will set the specific error code.
	 */
	void CALL_TYPE SetCustomCode(GMSEC_I32 code);

	/** @fn Set(GMSEC_STATUS_CLASS eclass, unsigned int code, const char *text)
	 * @brief This is a convience function that will set class, code, and error text.
	 */
	void CALL_TYPE Set(GMSEC_STATUS_CLASS eclass, unsigned int code, const char *text);

	/** @fn ReSet()
	 * @brief This clears this Status to GMSEC_STATUS_NO_ERROR.
	 */
	void CALL_TYPE ReSet();

private:
	internal::BaseStatus* ptr;

    // DR 2030: Added to ensure Status remains binary-compatible.
    mutable char*      padding_1;
    GMSEC_STATUS_CLASS padding_2;
    unsigned int       padding_3;
    GMSEC_I32          padding_4;
};

} // namespace gmsec

#endif  /* gmsec_Status_h */
