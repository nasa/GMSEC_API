/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API5_INTERNAL_STRINGFIELD_H
#define GMSEC_API5_INTERNAL_STRINGFIELD_H

#include <gmsec5/internal/field/InternalField.h>

#include <string>


namespace gmsec
{
namespace api5
{
namespace internal
{

class InternalStringField : public InternalField
{
public:
	InternalStringField(const char* name, const char* value, bool isHeader = false);

	virtual ~InternalStringField();

	const char* getValue() const;

	virtual const char* toXML() const;
	virtual const char* toJSON() const;

private:
	// defined, but not implemented
	InternalStringField();
	InternalStringField(const InternalStringField&);
	InternalStringField& operator=(const InternalStringField&);

	std::string m_value;
};

} //namespace internal
} //namespace api5
} //namespace gmsec

#endif
