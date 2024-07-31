/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API5_INTERNAL_BOOLEANFIELD_H
#define GMSEC_API5_INTERNAL_BOOLEANFIELD_H

#include <gmsec5/internal/field/InternalField.h>


namespace gmsec
{
namespace api5
{
namespace internal
{

class InternalBooleanField : public InternalField
{
public:
	InternalBooleanField(const char* name, bool value, bool isHeader = false);

	virtual ~InternalBooleanField();

	bool getValue() const;

	virtual const char* toXML() const;
	virtual const char* toJSON() const;

private:
	// defined, but not implemented
	InternalBooleanField();
	InternalBooleanField(const InternalBooleanField&);
	InternalBooleanField& operator=(const InternalBooleanField&);

	bool m_value;
};

} //namespace internal
} //namespace api5
} //namespace gmsec

#endif
