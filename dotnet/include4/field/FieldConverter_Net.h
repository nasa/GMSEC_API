/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_FIELD_CONVERTER_NET_H
#define GMSEC_API_FIELD_CONVERTER_NET_H


// Managed
#include <field/Field_Net.h>

// C++ API native
#include <gmsec4/field/Field.h>


namespace GMSEC
{
namespace API
{


public ref class FieldConverter
{
internal:
	static Field^ CreateFieldReference(gmsec::api::Field* nativeField);
};


}  // end namespace API
}  // end namespace GMSEC


#endif
