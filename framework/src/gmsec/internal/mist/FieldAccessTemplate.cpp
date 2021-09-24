/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





/** @file FieldAccessTemplate
 *
 *  @brief This file contains a the FieldAccessTemplate class, whose purpose
 *  is to have a uniform public interface that can run the different
 *  overloaded versions of Field::GetValue() and Field::SetValue(),
 *  by specializing the FieldAccessTemplate class for GMSEC_BIN and
 *  GMSEC_BLOB, which need two arguments rather than one (which the rest of the
 *  types need) for each of those functions.
 *
 *  This appeared to be the simplest, smallest way to allow  a template class 
 *  to access those Field functions which need different numbers-of-arguments
 *  for different types.  Partial specialization of a user template class
 *  (where specifially needed) appeared possible, too, but looked like it 
 *  would be more likely incompatible between different operating systems.
 *
**/

#include <gmsec/internal/mist/FieldAccessTemplate.h>

#include <gmsec/Field.h>

namespace gmsec
{

namespace mist
{

namespace internal
{


template <class type>
FieldAccessTemplate<type>::FieldAccessTemplate(Field* field)
{
	m_fieldPtr = field;

}// FieldAccessTemplate Constructor


template <class type>
FieldAccessTemplate<type>::FieldAccessTemplate()
{
}// FieldAccessTemplate Default Constructor, which is private and never used


template <class type>
FieldAccessTemplate<type>::~FieldAccessTemplate()
{
}// FieldAccessTemplate Destructor


template <class type>
Status FieldAccessTemplate<type>::GetValue(type& value, GMSEC_U32& size)
{
	size = sizeof(type);
	return m_fieldPtr->GetValue(value);

}// GetValue()


template <class type>
Status FieldAccessTemplate<type>::SetValue(type value, GMSEC_U32 size)
{
	return m_fieldPtr->SetValue(value);

}// SetValue()


FieldAccessTemplate<GMSEC_BIN>::FieldAccessTemplate(Field* field)
{
	m_fieldPtr = field;

}// FieldAccessTemplate Constructor


FieldAccessTemplate<GMSEC_BIN>::FieldAccessTemplate()
{
}// FieldAccessTemplate Default Constructor, which is private and never used


FieldAccessTemplate<GMSEC_BIN>::~FieldAccessTemplate()
{
}// FieldAccessTemplate Destructor


Status FieldAccessTemplate<GMSEC_BIN>::GetValue(GMSEC_BIN& value,
                                                GMSEC_U32& size)
{
	return m_fieldPtr->GetValue(&value, size);

}// GetValue() for <GMSEC_BIN>


Status FieldAccessTemplate<GMSEC_BIN>::SetValue(GMSEC_BIN value,
                                                GMSEC_U32 size)
{
	return m_fieldPtr->SetValue(value, size);

}// SetValue() for <GMSEC_BLOB>


FieldAccessTemplate<GMSEC_BLOB>::FieldAccessTemplate(Field* field)
{
	m_fieldPtr = field;

}// FieldAccessTemplate Constructor


FieldAccessTemplate<GMSEC_BLOB>::FieldAccessTemplate()
{
}// FieldAccessTemplate Default Constructor, which is private and never used


FieldAccessTemplate<GMSEC_BLOB>::~FieldAccessTemplate()
{
}// FieldAccessTemplate Destructor


Status FieldAccessTemplate<GMSEC_BLOB>::GetValue(GMSEC_BLOB& value,
                                                 GMSEC_U32& size)
{
	return m_fieldPtr->GetValue(value, size);

}// GetValue() for <GMSEC_BIN>


Status FieldAccessTemplate<GMSEC_BLOB>::SetValue(GMSEC_BLOB value,
                                                 GMSEC_U32 size)
{
	return m_fieldPtr->SetValue(value, size);

}// SetValue() for <GMSEC_BLOB>


template class FieldAccessTemplate<GMSEC_CHAR>;
template class FieldAccessTemplate<GMSEC_BOOL>;
template class FieldAccessTemplate<GMSEC_I8>;
template class FieldAccessTemplate<GMSEC_U8>;
template class FieldAccessTemplate<GMSEC_I64>;
template class FieldAccessTemplate<GMSEC_U64>;
template class FieldAccessTemplate<GMSEC_I16>;
template class FieldAccessTemplate<GMSEC_U16>;
template class FieldAccessTemplate<GMSEC_I32>;
template class FieldAccessTemplate<GMSEC_U32>;
template class FieldAccessTemplate<GMSEC_F32>;
template class FieldAccessTemplate<GMSEC_F64>;
template class FieldAccessTemplate<GMSEC_STRING>;
template class FieldAccessTemplate<GMSEC_BLOB>;
template class FieldAccessTemplate<GMSEC_BIN>;


}// end namespace internal

}// end namespace mist

}// end namespace gmsec
