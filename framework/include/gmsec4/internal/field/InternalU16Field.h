/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_INTERNAL_U16FIELD_H
#define GMSEC_API_INTERNAL_U16FIELD_H

#include <gmsec4/internal/field/InternalField.h>

#include <gmsec4_defs.h>

#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
namespace api
{
namespace internal
{

class GMSEC_API InternalU16Field : public InternalField
{
public:
	InternalU16Field(const char* name, GMSEC_U16 value);

	virtual ~InternalU16Field();

	GMSEC_U16 CALL_TYPE getValue() const;

	virtual const char* CALL_TYPE toXML() const;
	virtual const char* CALL_TYPE toJSON() const;

private:
	// defined, but not implemented
	InternalU16Field();
	InternalU16Field(const InternalU16Field&);
	InternalU16Field& operator=(const InternalU16Field&);

	GMSEC_U16 m_value;
};

} //namespace internal
} //namespace api
} //namespace gmsec

#endif
