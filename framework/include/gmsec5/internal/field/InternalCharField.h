/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API5_INTERNAL_CHARFIELD_H
#define GMSEC_API5_INTERNAL_CHARFIELD_H

#include <gmsec5/internal/field/InternalField.h>

#include <gmsec5_defs.h>


namespace gmsec
{
namespace api5
{
namespace internal
{

class InternalCharField : public InternalField
{
public:
	InternalCharField(const char* name, GMSEC_CHAR value, bool isHeader = false);

	virtual ~InternalCharField();

	GMSEC_CHAR getValue() const;

	virtual const char* toXML() const;
	virtual const char* toJSON() const;

private:
	InternalCharField();
	InternalCharField(const InternalCharField&);
	InternalCharField& operator=(const InternalCharField&);

	GMSEC_CHAR m_value;
};

} //namespace internal
} //namespace api5
} //namespace gmsec

#endif
