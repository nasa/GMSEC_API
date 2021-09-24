/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_INTERNAL_CHARFIELD_H
#define GMSEC_API_INTERNAL_CHARFIELD_H

#include <gmsec4/internal/field/InternalField.h>

#include <gmsec4_defs.h>

#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
namespace api
{
namespace internal
{

class GMSEC_API InternalCharField : public InternalField
{
public:
	InternalCharField(const char* name, GMSEC_CHAR value);

	virtual ~InternalCharField();

	GMSEC_CHAR CALL_TYPE getValue() const;

	virtual const char* CALL_TYPE toXML() const;
	virtual const char* CALL_TYPE toJSON() const;

private:
	InternalCharField();
	InternalCharField(const InternalCharField&);
	InternalCharField& operator=(const InternalCharField&);

	GMSEC_CHAR m_value;
};

} //namespace internal
} //namespace api
} //namespace gmsec

#endif
