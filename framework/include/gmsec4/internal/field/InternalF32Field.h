/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_INTERNAL_F32FIELD_H
#define GMSEC_API_INTERNAL_F32FIELD_H

#include <gmsec4/internal/field/InternalField.h>

#include <gmsec4_defs.h>

#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
namespace api
{
namespace internal
{

class GMSEC_API InternalF32Field : public InternalField
{
public:
	InternalF32Field(const char* name, GMSEC_F32 value);

	virtual ~InternalF32Field();

	GMSEC_F32 CALL_TYPE getValue() const;

	virtual const char* CALL_TYPE toXML() const;
	virtual const char* CALL_TYPE toJSON() const;

private:
	InternalF32Field();
	InternalF32Field(const InternalF32Field&);
	InternalF32Field& operator=(const InternalF32Field&);

	GMSEC_F32 m_value;
};

} //namespace internal
} //namespace api
} //namespace gmsec

#endif
