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

#ifndef gmsec_FieldAccessTemplate_h
#define gmsec_FieldAccessTemplate_h

#include <gmsec/Field.h>

namespace gmsec
{

namespace mist
{

namespace internal
{

/***********************************************/
/** @class FieldAcessTemplate
 * @brief A FieldAccessTemplate just allows a template class to access
 *        different overloaded methods of the GMSEC Field class,
 *        which have different numbers-of-parameters depending on type.
 *
 *        This could be a static class too, with no constructor
 *        or destructor and the relevant field just provided
 *        as an argument to the GetValue() and SetValue() functions
 *        here.  It's debatable what is the best specific layout choice.
 *
 *        The function definitions are included in this header file
 *        because of template link problems otherwise during the use of
 *        libGMSECAPI.so, though there is a FieldAccessTemplate.cpp (and
 *        the definitions can be stripped outof his header) if 
 *        these link problems are resolved.  The FieldAccessTemplate
 *        class is expected to be included only from one place
 *        (BaseFieldDescriptor.h, which itself is apparently only included
 *        from one place) so there are no immediate muliply-defined
 *        link problems.
 *
 * @sa gmsec::Field
*/

template <class type> class GMSEC_API FieldAccessTemplate
{
  public:

	/** @fn FieldAccessTemplate()
	 * @brief Constructor where user specifies Field to associate with.
	 *
	 */
	FieldAccessTemplate(Field* field)
	{
		m_fieldPtr = field;
	};

	~FieldAccessTemplate()
	{
	};

	Status CALL_TYPE GetValue(type& value, GMSEC_U32& size)
	{
		size = sizeof(type);
		return m_fieldPtr->GetValue(value);
	};

	Status CALL_TYPE SetValue(type value, GMSEC_U32 size = 0)
	{
		return m_fieldPtr->SetValue(value);
	};

  private:

	/** @fn FieldAccessTemplate()
	 * @brief No access to default constructor.
	 *
	 */
	FieldAccessTemplate() {};

	Field* m_fieldPtr;

};

//
// The following is the FieldAccessTemplate specialization for type GMSEC_BIN
//
template <> class FieldAccessTemplate<GMSEC_BIN>
{
  public:

	/** @fn FieldAccessTemplate()
	 * @brief Constructor where user specifies Field to associate with.
	 *
	 */
	FieldAccessTemplate(Field* field)
	{
		m_fieldPtr = field;
	};

	~FieldAccessTemplate()
	{
	};

	Status CALL_TYPE GetValue(GMSEC_BIN& value, GMSEC_U32& size)
	{
		return m_fieldPtr->GetValue(&value, size);
	};

	Status CALL_TYPE SetValue(GMSEC_BIN value, GMSEC_U32 size = 0)
	{
		return m_fieldPtr->SetValue(value, size);
	};

  private:

	/** @fn FieldAccessTemplate()
	 * @brief No access to default constructor.
	 *
	 */
	FieldAccessTemplate() {};

	Field* m_fieldPtr;

};

//
// The following is the FieldAccessTemplate specialization for type GMSEC_BLOB
//
template <> class FieldAccessTemplate<GMSEC_BLOB>
{
  public:

	/** @fn FieldAccessTemplate()
	 * @brief Constructor where user specifies Field to associate with.
	 *
	 */
	FieldAccessTemplate(Field* field)
	{
		m_fieldPtr = field;
	};

	~FieldAccessTemplate()
	{
	};

	Status CALL_TYPE GetValue(GMSEC_BLOB& value, GMSEC_U32& size)
	{
		return m_fieldPtr->GetValue(value, size);
	};

	Status CALL_TYPE SetValue(GMSEC_BLOB value, GMSEC_U32 size = 0)
	{
		return m_fieldPtr->SetValue(value, size);
	};

  private:

	/** @fn FieldAccessTemplate()
	 * @brief No access to default constructor.
	 *
	 */
	FieldAccessTemplate() {};

	Field* m_fieldPtr;

};


} //end internal

} //end mist

} //end gmsec

#endif
