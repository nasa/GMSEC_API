/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_INTERNAL_BINARYFIELD_H
#define GMSEC_API_INTERNAL_BINARYFIELD_H

#include <gmsec4/internal/field/InternalField.h>

#include <gmsec4_defs.h>

#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
namespace api
{
namespace internal
{

class GMSEC_API InternalBinaryField : public InternalField
{
public:
	InternalBinaryField(const char* name, const GMSEC_BIN blob, size_t length);

	virtual ~InternalBinaryField();

	GMSEC_BIN CALL_TYPE getValue() const;

	size_t CALL_TYPE getLength() const;

	std::string getStringValue() const;

	virtual const char* CALL_TYPE toXML() const;
	virtual const char* CALL_TYPE toJSON() const;

private:
	// defined, but not implemented
	InternalBinaryField();
	InternalBinaryField(const InternalBinaryField&);
	InternalBinaryField& operator=(const InternalBinaryField&);

	GMSEC_BIN m_blob;
	size_t    m_length;
};

} //namespace internal
} //namespace api
} //namespace gmsec

#endif
