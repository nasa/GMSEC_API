/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_INTERNAL_STRINGFIELD_H
#define GMSEC_API_INTERNAL_STRINGFIELD_H

#include <gmsec4/internal/field/InternalField.h>

#include <gmsec4/util/wdllexp.h>

#include <string>


namespace gmsec
{
namespace api
{
namespace internal
{

class GMSEC_API InternalStringField : public InternalField
{
public:
	InternalStringField(const char* name, const char* value);

	virtual ~InternalStringField();

	const char* CALL_TYPE getValue() const;

	virtual const char* CALL_TYPE toXML() const;
	virtual const char* CALL_TYPE toJSON() const;

private:
	// defined, but not implemented
	InternalStringField();
	InternalStringField(const InternalStringField&);
	InternalStringField& operator=(const InternalStringField&);

	std::string m_value;
};

} //namespace internal
} //namespace api
} //namespace gmsec

#endif
