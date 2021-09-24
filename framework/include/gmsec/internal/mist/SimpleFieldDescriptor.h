
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file SimpleFieldDescriptor.h
 *
 *  @brief This file contains the SimpleFieldDescriptor class,
 *         which is one of the MIST classes which desribe Fields.
 *
**/

#ifndef gmsec_SimpleFieldDescriptor_h
#define gmsec_SimpleFieldDescriptor_h


#include <gmsec/internal/mist/AbstractFieldDescriptor.h>


namespace gmsec
{

namespace mist
{
namespace internal
{
class BaseTester;
template<class T> class BaseSimpleFieldDescriptor;


/** @class SimpleFieldDescriptor
 * @brief The SimpleFieldDescriptor object is a wrapper class for
 * BaseSimpleFieldDescriptor and extends the AbstractFieldDescriptor. 
 * It allows for the setting of the value through the constructor and
 * the value T is specified as the template parameter.
 *
 * @sa gmsec::mist::AbstractFieldDescriptor
 * @sa gmsec::mist::internal::BaseCompoundFieldDescriptor
 * @sa gmsec::mist::Context
*/
template <class T>
class GMSEC_API SimpleFieldDescriptor : public AbstractFieldDescriptor
{
private:
	internal::BaseSimpleFieldDescriptor<T>* ptr;	

	friend class internal::BaseTester;

	// not implemented
	SimpleFieldDescriptor &operator=(const SimpleFieldDescriptor &);
	SimpleFieldDescriptor(const SimpleFieldDescriptor &);

	/** @fn AddTo(Message *message, Context &context) const
	 * @brief AddTo function generates a field from the class members and adds
	 * it to the Message
	 *
	 * @param message - The field represented by the class members is added 
	 * to the reference Message
	 * @param context - The reference to the Context object stores the statuses
	 * that are generated
	 *
	 * @return status - result of operation
	 */
	Status AddTo(Message *message, Context &context) const
	{
		return ptr->AddTo(message, context);
	};
	
	/** @fn Validate(Message *message, Context &context) const
	 * @brief Compares the fields of the Message parameter with the field 
	 * described by the class members. If user added fields are not allowed
	 * it verifies that there are no extra fields
	 *
	 * @param message - The fields contained in the reference Message are
	 * validated against the class varibles
	 * @param context - The reference to the Context object stores the statuses
	 * that are generated
	 *
	 * @return status - result of operation
	 */
	Status Validate(Message *message, Context &context) const
	{
		return ptr->Validate(message, context);
	};
	
	/** @fn Apply(Message *message, Context &context, MIST_VALIDATE_VALS validate) const
	 * @brief Apply generates a Field out of the class variables and adds it
	 * to the Message if the field does not already exist.  It compares fields
	 * that already exist in the message if the name matches the class member 
	 * name and validates the types are correct if enum validate is set to 
	 * VALIDATE_ON_APPLY
	 *
	 * @param message - A field is generated and added to the reference Message 
	 * if it does not already exist
	 * @param context - The reference to the Context object stores the statuses
	 * that are generated
	 * @param validate - The enum determines if validation occurs and can be set to
	 * either VALIDATE_ON_APPLY or NO_VALIDATE_ON_APPLY
	 *
	 * @return status - result of operation
	 */
	Status Apply(Message *message, Context &context, MIST_VALIDATE_VALS validate) const
	{
		return ptr->Apply(message, context, validate);
	};
	
	/** @fn FindField(Message *message, Context &context, MIST_VALIDATE_VALS validate) const
	 * @brief The helper function to the Apply, Instantiate, and Validate
	 * functions
	 *
	 * @param message - The reference to the Message is updated based on either
	 * Apply, Instantiate, or Validate
	 * @param context - The reference to the Context object stores the statuses
	 * that are generated
	 * @param validate - The enum determines if validation occurs and can be set to
	 * VALIDATE, VALIDATE_ON_APPLY, or NO_VALIDATE_ON_APPLY
	 *
	 * @return status - result of operation
	 */
	Status FindField(Message *message, Context &context, MIST_VALIDATE_VALS validate) const
	{
		return ptr->FindField(message, context, validate);
	};

public:

	/** @fn SimpleFieldDescriptor()
	 * @brief Default constructor - initializes the SimpleFieldDescriptor and the 
	 * pointer to the BaseSimpleFieldDescriptor instance, calls the 
	 * AbstractBaseFieldDescriptor default constructor and sets the value to null
	 *
	 */
	SimpleFieldDescriptor()
	{
		ptr = new internal::BaseSimpleFieldDescriptor<T>();
	};

	/** @fn BaseSimpleFieldDescriptor(T &newValue)
	 * @brief Constructor - initializes the SimpleFieldDescriptor and the 
	 * pointer to the BaseSimpleFieldDescriptor instance, calls the AbstractBaseFieldDescriptor
	 * default constructor and sets the children to a DescriptorList generated with newChildren,
	 * taking ownership of the variable
	 *
	 * @param value - set to the value member.  Ownership is taken by 
	 *
	 * BaseSimpleFieldDescriptor
	 *
	 */
	SimpleFieldDescriptor(T &value)
	{
		ptr = new internal::BaseSimpleFieldDescriptor<T>(value);
	};

	/** @fn BaseSimpleFieldDescriptor(T &newValue, size_t size)
	 * @brief Constructor - initializes the SimpleFieldDescriptor and the 
	 * pointer to the BaseSimpleFieldDescriptor instance, calls the AbstractBaseFieldDescriptor
	 * default constructor and sets the children to a DescriptorList generated with newChildren,
	 * taking ownership of the variable
	 *
	 * @param value - set to the value member.  Ownership is taken by 
	 * @param size  -- size of value parameter; needed only when
	 *                 T is GMSEC_BIN or GMSEC_BLOB
	 * BaseSimpleFieldDescriptor
	 *
	 */
	SimpleFieldDescriptor(T &value, size_t size)
	{
		ptr = new internal::BaseSimpleFieldDescriptor<T>(value,
		                                                    (GMSEC_U32) size);
	};

	/** @fn ~BaseSimpleFieldDescriptor()
	 * @brief Destructor
	 */
	~SimpleFieldDescriptor()
	{
		delete ptr;
		ptr = 0;
	};

	/** @fn SetFieldVariables(const char * name, FieldMode mode,
		FieldClass clss, const char * description)
	 * @brief Sets the BaseSimpleFieldDescriptor's name, description,
	 * mode, and class variables
	 *
	 * @param name - The BaseSimpleFieldDescriptor's name member is set to
	 * the name parameter
	 * @param mode - The BaseSimpleFieldDescriptor's mode member is set to
	 * the mode parameter
	 * @param clss - The BaseSimpleFieldDescriptor's class member is set to
	 * the clss parameter
	 * @param description - The BaseSimpleFieldDescriptor's description 
	 * member is set to the description parameter
	 *
	 * @return status - result of operation
	 */
	Status SetFieldVariables(const char * name, FieldMode mode,
		FieldClass clss, const char * description)
	{
		return ptr->SetFieldVariables(name, mode, clss, description);
	};

	/** @fn SetValue(T &value)
	 * @brief Updates the pointer's value member with the value parameter and takes ownership
	 * of the parameter
	 *
	 * @param value - The value of type T of the template; replaces the class
	 * member value
	 *
	 * @return status - result of operation
	 */
	Status SetValue(T &newValue)
	{
		return ptr->SetValue(newValue);
	};

	/** @fn GetValue(T &value)
	 * @brief Sets the pointer's value parameter to the value class member.
	 *
	 * @param value - A reference of type T of the template that is set to 
	 * the class member value
	 *
	 * @return status - result of operation
	 */
	Status GetValue(T &newValue) const
	{
		return ptr->GetValue(newValue);
	};
};

} // end internal

} //end mist

} //end gmsec

#endif
