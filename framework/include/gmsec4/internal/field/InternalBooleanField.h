/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_INTERNAL_BOOLEANFIELD_H
#define GMSEC_API_INTERNAL_BOOLEANFIELD_H

#include <gmsec4/internal/field/InternalField.h>

#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
namespace api
{
namespace internal
{

class GMSEC_API InternalBooleanField : public InternalField
{
public:
	InternalBooleanField(const char* name, bool value);

	virtual ~InternalBooleanField();

	bool CALL_TYPE getValue() const;

	virtual const char* CALL_TYPE toXML() const;
	virtual const char* CALL_TYPE toJSON() const;

private:
	// defined, but not implemented
	InternalBooleanField();
	InternalBooleanField(const InternalBooleanField&);
	InternalBooleanField& operator=(const InternalBooleanField&);

	bool m_value;
};

} //namespace internal
} //namespace api
} //namespace gmsec

#endif
