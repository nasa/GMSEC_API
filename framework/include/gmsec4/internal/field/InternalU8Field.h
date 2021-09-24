/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_INTERNAL_U8FIELD_H
#define GMSEC_API_INTERNAL_U8FIELD_H

#include <gmsec4/internal/field/InternalField.h>

#include <gmsec4_defs.h>

#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
namespace api
{
namespace internal
{

class GMSEC_API InternalU8Field : public InternalField
{
public:
	InternalU8Field(const char* name, GMSEC_U8 value);

	virtual ~InternalU8Field();

	GMSEC_U8 CALL_TYPE getValue() const;

	virtual const char* CALL_TYPE toXML() const;
	virtual const char* CALL_TYPE toJSON() const;

private:
	// defined, but not implemented
	InternalU8Field();
	InternalU8Field(const InternalU8Field&);
	InternalU8Field& operator=(const InternalU8Field&);

	GMSEC_U8 m_value;
};

} //namespace internal
} //namespace api
} //namespace gmsec

#endif
