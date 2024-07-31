/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file InternalGmsecException.h
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

#ifndef GMSEC_API5_INTERNAL_GMSECEXCEPTION_H
#define GMSEC_API5_INTERNAL_GMSECEXCEPTION_H

#include <gmsec5/Errors.h>

#include <gmsec5_defs.h>  // for GMSEC_I32

#include <gmsec5/util/wdllexp.h>

#include <string>


namespace gmsec
{
namespace api5
{
namespace internal
{


class GMSEC_API InternalGmsecException
{
public:
	InternalGmsecException(GMSEC_I32 errorClass, GMSEC_I32 errorCode, const char* errorMsg);


	InternalGmsecException(GMSEC_I32 errorClass, GMSEC_I32 errorCode, GMSEC_I32 customCode, const char* errorMsg);


	InternalGmsecException(const InternalGmsecException& other);


	virtual ~InternalGmsecException();


	InternalGmsecException& operator=(const InternalGmsecException& other);


	GMSEC_I32 CALL_TYPE getErrorClass() const;


	GMSEC_I32 CALL_TYPE getErrorCode() const;


	GMSEC_I32 CALL_TYPE getCustomCode() const;


	const char* CALL_TYPE getErrorMessage() const;


	const char* CALL_TYPE what() const;

private:
	GMSEC_I32   m_errorClass;
	GMSEC_I32   m_errorCode;
	GMSEC_I32   m_customCode;
	std::string m_errorMsg;

	mutable std::string m_what;
};


} //end namespace internal
} //end namespace api5
} //end namespace gmsec

#endif
