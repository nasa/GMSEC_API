/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file InternalStatus.h
 *
 *  @brief This file contains the class that encapsulates status information.
 */

#ifndef GMSEC_API5_INTERNALSTATUS_H
#define GMSEC_API5_INTERNALSTATUS_H

#include <gmsec5/Errors.h>

#include <gmsec5_defs.h>

#include <gmsec5/util/wdllexp.h>

#include <string>


namespace gmsec
{
namespace api5
{
	class GmsecException;

// Forward declaration(s)
namespace internal
{


class GMSEC_API InternalStatus
{
public:
	InternalStatus();


	InternalStatus(GMSEC_I32 eclass, GMSEC_I32 ecode, const char* reason = NULL, GMSEC_I32 custom = 0);


	InternalStatus(const GmsecException& e);


	InternalStatus(const InternalStatus& other);


	~InternalStatus();


	InternalStatus& operator=(const InternalStatus& other);


	bool CALL_TYPE hasError() const;

	const char* CALL_TYPE get() const;

	void CALL_TYPE set(GMSEC_I32 eclass, GMSEC_I32 ecode, const char* reason = NULL, GMSEC_I32 custom = 0);

	GMSEC_I32 CALL_TYPE getClass() const;

	void CALL_TYPE setClass(GMSEC_I32 eclass);

	GMSEC_I32 CALL_TYPE getCode() const;

	void CALL_TYPE setCode(GMSEC_I32 ecode);

	const char* CALL_TYPE getReason() const;

	void CALL_TYPE setReason(const char* reason);

	GMSEC_I32 CALL_TYPE getCustomCode() const;

	void CALL_TYPE setCustomCode(GMSEC_I32 code);

	void CALL_TYPE reset();

private:
	GMSEC_I32   m_eclass;
	GMSEC_I32   m_ecode;
	std::string m_reason;
	GMSEC_I32   m_custom;

	mutable std::string m_status;
};

} // namespace internal
} // namespace api5
} // namespace gmsec

#endif
