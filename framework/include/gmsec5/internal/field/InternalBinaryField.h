/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API5_INTERNAL_BINARYFIELD_H
#define GMSEC_API5_INTERNAL_BINARYFIELD_H

#include <gmsec5/internal/field/InternalField.h>

#include <gmsec5_defs.h>


namespace gmsec
{
namespace api5
{
namespace internal
{

class InternalBinaryField : public InternalField
{
public:
	InternalBinaryField(const char* name, const GMSEC_U8* blob, size_t length, bool isHeader = false);

	virtual ~InternalBinaryField();

	GMSEC_U8 const* getValue() const;

	size_t getLength() const;

	std::string getStringValue() const;

	virtual const char* toXML() const;
	virtual const char* toJSON() const;

private:
	// defined, but not implemented
	InternalBinaryField();
	InternalBinaryField(const InternalBinaryField&);
	InternalBinaryField& operator=(const InternalBinaryField&);

	GMSEC_U8* m_blob;
	size_t    m_length;
};

} //namespace internal
} //namespace api5
} //namespace gmsec

#endif
