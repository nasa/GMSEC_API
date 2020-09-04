/*
 * Copyright 2007-2016 United States Government as represented by the
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

#ifndef GMSEC_MIST_SIMPLE_FIELD_DESCRIPTOR_H
#define GMSEC_MIST_SIMPLE_FIELD_DESCRIPTOR_H

#include <gmsec4/internal/mist/AbstractFieldDescriptor.h>

#include <gmsec4/Fields.h>


namespace gmsec
{
namespace api
{
namespace mist
{
namespace internal
{
	// Forward declaration(s)
	template<class T> class InternalSimpleFieldDescriptor;
	class InternalTester;


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
template <typename T>
class GMSEC_API SimpleFieldDescriptor : public AbstractFieldDescriptor
{
public:
	/** @fn SimpleFieldDescriptor()
	 * @brief Default constructor - initializes the SimpleFieldDescriptor and the 
	 * pointer to the BaseSimpleFieldDescriptor instance, calls the 
	 * AbstractBaseFieldDescriptor default constructor and sets the value to null
	 *
	 */
	SimpleFieldDescriptor()
		: ptr(new internal::InternalSimpleFieldDescriptor<T>())
	{
	}


	/** @fn SimpleFieldDescriptor(T& value)
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
	SimpleFieldDescriptor(T& value)
		: ptr(new internal::InternalSimpleFieldDescriptor<T>(value))
	{
	}


	/** @fn SimpleFieldDescriptor(T& newValue, size_t size)
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
	SimpleFieldDescriptor(T& value, size_t size)
		: ptr(new internal::InternalSimpleFieldDescriptor<T>(value, size))
	{
	}


	/** @fn ~BaseSimpleFieldDescriptor()
	 * @brief Destructor
	 */
	~SimpleFieldDescriptor()
	{
		delete ptr;
		ptr = 0;
	}


	/** @fn setFieldVariables(const char* name, FieldMode mode, FieldClass clss, const char* description)
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
	void setFieldVariables(const char* name, FieldMode mode, FieldClass clss, const char* description)
	{
		ptr->setFieldVariables(name, mode, clss, description);
	}


	/** @fn setValue(T& value)
	 * @brief Updates the pointer's value member with the value parameter and takes ownership
	 * of the parameter
	 *
	 * @param value - The value of type T of the template; replaces the class
	 * member value
	 */
	void setValue(T& value)
	{
		return ptr->setValue(value);
	}


	/** @fn getValue(T& value)
	 * @brief Sets the pointer's value parameter to the value class member.
	 *
	 * @param value - A reference of type T of the template that is set to 
	 * the class member value
	 */
	T getValue() const
	{
		return ptr->getValue();
	}


private:
	// Defined, but not implemented
	SimpleFieldDescriptor(const SimpleFieldDescriptor&);
	SimpleFieldDescriptor& operator=(const SimpleFieldDescriptor&);


	/** @fn addTo(Message& message, Context& context) const
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
	Status addTo(Message& message, Context& context) const
	{
		return ptr->addTo(message, context);
	}

	
	/** @fn validate(const Message& message, Context& context) const
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
	Status validate(Message& message, Context& context) const
	{
		return ptr->validate(message, context);
	}

	
	/** @fn apply(Message& message, Context& context, MIST_VALIDATE_VALS validate) const
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
	Status apply(Message& message, Context& context, MIST_VALIDATE_VALS validate) const
	{
		return ptr->apply(message, context, validate);
	}

	
	/** @fn findField(Message& message, Context& context, MIST_VALIDATE_VALS validate) const
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
	Status findField(Message& message, Context& context, MIST_VALIDATE_VALS validate) const
	{
		return ptr->findField(message, context, validate);
	}


	internal::InternalSimpleFieldDescriptor<T>* ptr;	

	friend class InternalTester;
};

} //namespace internal
} //namespace mist
} //namespace api
} //namespace gmsec

#endif
