/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file InternalSimpleFieldDescriptor.h
 *
 *  @brief This file contains the InternalSimpleFieldDescriptor class
 *         which desribes Fields.
 *
**/

#ifndef GMSEC_MIST_INTERNAL_SIMPLE_FIELD_DESCRIPTOR_H
#define GMSEC_MIST_INTERNAL_SIMPLE_FIELD_DESCRIPTOR_H


#include <gmsec4/internal/mist/AbstractBaseFieldDescriptor.h>

#include <gmsec4/internal/mist/Context.h>
#include <gmsec4/internal/mist/InternalContext.h>
#include <gmsec4/internal/mist/FieldAccessTemplate.h>

#include <gmsec4/internal/StringUtil.h>

#include <gmsec4/Fields.h>
#include <gmsec4/Message.h>
#include <gmsec4/MessageFieldIterator.h>

#include <string>
#include <typeinfo>


namespace gmsec
{
namespace api
{
namespace mist
{
namespace internal
{
	// Forward declaration(s)
	class AbstractFieldDescriptor;
	class InternalTester;


/***********************************************/
/** @class InternalSimpleFieldDescriptor
 * @brief The InternalSimpleFieldDescriptor object represents a simple field
 * that contains a value of the template type.  It extends the 
 * InternalAbstractFieldDescriptor and adds getters and setters for value
 * as well as a function to generate a Field from the value information
 *
 * @sa gmsec::mist::internal::AbstractInternalFieldDescriptor
 * @sa gmsec::mist::SimpleFieldDescriptor
 * @sa gmsec::Field
 * @sa gmsec::mist::Context
*/
template <typename T>
class GMSEC_API InternalSimpleFieldDescriptor : public AbstractBaseFieldDescriptor
{
public:
	/** @fn InternalSimpleFieldDescriptor()
	 * @brief Default constructor - initializes the InternalSimpleFieldDescriptor
	 * instance, calls the AbstractInternalFieldDescriptor default constructor and
	 * sets the value to null
	 *
	 */
	InternalSimpleFieldDescriptor()
		: AbstractBaseFieldDescriptor(),
	 	  m_value(0),
		  m_valueSize(0)
	{
	}


	/** @fn InternalSimpleFieldDescriptor(T& newValue)
	 * @brief Constructor - initializes the InternalSimpleFieldDescriptor
	 * instance, calls the AbstractInternalFieldDescriptor default constructor and
	 * sets the children to a DescriptorList generated with newChildren, taking 
	 * ownership of the variable
	 *
	 * @param newValue - set to the value member.  Ownership is taken by 
	 * InternalSimpleFieldDescriptor
	 *
	 */
	InternalSimpleFieldDescriptor(T& value)
		: AbstractBaseFieldDescriptor(),
		  m_value(value),
		  m_valueSize(0)
	{
	}


	/** @fn InternalSimpleFieldDescriptor(T& newValue, size_t size)
	 * @brief Constructor - initializes the InternalSimpleFieldDescriptor
	 * instance, calls the AbstractInternalFieldDescriptor default constructor and
	 * sets the children to a DescriptorList generated with newChildren, taking 
	 * ownership of the variable
	 *
	 * @param newValue - set to the value member.  Ownership is taken by 
	 * @param size - set to the value member size.
	 *
	 * InternalSimpleFieldDescriptor
	 *
	 */
	InternalSimpleFieldDescriptor(T& value, size_t size)
		: AbstractBaseFieldDescriptor(),
	 	  m_value(value),
		  m_valueSize(size)
	{
	}


	/** @fn ~InternalSimpleFieldDescriptor()
	 * @brief Destructor
	 */
	virtual ~InternalSimpleFieldDescriptor()
	{
	}

	
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
		Status status;

		context.ptr->incrementDepth(m_name);

		std::string fullPathName = context.ptr->fullNamePath();

		context.ptr->addVisited();	

		FieldAccessTemplate<T> fieldAccess(fullPathName, m_value, m_valueSize);
		Field&                 field = fieldAccess.getField();

		if (m_class == F_HEADER)
		{
			field.isHeader(true);
		}

		message.addField(field);

		context.ptr->decrementDepth();

		return status;
	}


	/** @fn validate(Message& message, Context& context) const
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
		return findField(message, context, VALIDATE);
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
		return findField(message, context, validate);
	}


	/** @fn setValue(T& value, size_t size = 0)
	 * @brief Updates the value member with the value parameter and takes ownership
	 * of the parameter
	 *
	 * @param value - The value of the type of the template and replaces the class
	 * member value
	 * @param size - set to the value member size.
	 *
	 */
	void setValue(T& value, size_t size = 0)
	{
		m_value     = value;
		m_valueSize = size;
	}
	

	/** @fn getValue(T& value)
	 * @brief Sets the value parameter to the value class member.
	 *
	 * @param value - A reference of the type of the template that is set to 
	 * the class member value
	 *
	 */
	T getValue() const
	{
		return m_value;
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
		Status status;

		context.ptr->incrementDepth(m_name);
		
		MessageFieldIterator& iter = message.getFieldIterator();

		while (iter.hasNext())
		{
			const Field& field = iter.next();

			std::string fpname = context.ptr->fullNamePath();

			if (fpname == field.getName())
			{
				context.ptr->addVisited();

				if (validate == VALIDATE || validate == VALIDATE_ON_APPLY)
				{
					//MEH - This code is intended to facilitate value validation,
					//which is not currently enabled. Therefore, it is commented
					//out for now. When value validation is added to MIST, it is
					//recommended that this block be refactored to better encapsulate
					//value checking logic.
					/*
					FieldAccessTemplate<T> fieldAccess(field);

					T      compType;
					size_t size;

					if (field.getType() != GMSEC_TYPE_BIN)
					{
						fieldAccess.getValue(compType, size);
					}
					*/
					
					
					if (!InternalSimpleFieldDescriptor::fieldMatchCurrentType(field, typeid(T).name()))
					{
						std::string desc("Field ");
						desc += fpname;
						desc += std::string(" does not have the appropriate type");

						ContextStatus cs;
						cs.fieldName   = fpname;
						cs.description = desc;
						cs.errorLevel  = INCORRECT_FIELD_TYPE;

						context.ptr->addStatus(cs);

						status.set(MIST_ERROR, INCORRECT_FIELD_TYPE, desc.c_str());
					}
					
				}

				context.ptr->decrementDepth();
				return status;
			}
		}

		if (validate == VALIDATE)
		{
			context.ptr->addVisited();

			if (m_mode == F_REQUIRED)
			{
				std::string fpname = context.ptr->fullNamePath();

				std::string desc("Required field ");
				desc += fpname;
				desc += std::string(" missing.");

				ContextStatus cs;
				cs.fieldName   = fpname;
				cs.description = desc;
				cs.errorLevel  = MISSING_REQUIRED_FIELD;

				context.ptr->addStatus(cs);

				status.set(MIST_ERROR, MISSING_REQUIRED_FIELD, desc.c_str());
			}
		}
		else
		{
			context.ptr->decrementDepth();
			return addTo(message, context);
		}

		context.ptr->decrementDepth();

		return status;
	}

private:
	// Defined, but not implemented.
	InternalSimpleFieldDescriptor(const InternalSimpleFieldDescriptor&);
	InternalSimpleFieldDescriptor& operator=(const InternalSimpleFieldDescriptor&);

	static bool fieldMatchCurrentType(const Field &field, std::string typeName){
		std::string desiredName;
		if(field.getType() == Field::BIN_TYPE){
			desiredName = typeid(GMSEC_BIN).name();
		} else if (field.getType() == Field::BOOL_TYPE){
			desiredName = typeid(GMSEC_BOOL).name();
		} else if (field.getType() == Field::CHAR_TYPE){
			desiredName = typeid(GMSEC_CHAR).name();
		} else if (field.getType() == Field::I8_TYPE){
			desiredName = typeid(GMSEC_I8).name();
		} else if (field.getType() == Field::I16_TYPE){
			desiredName = typeid(GMSEC_I16).name();
		} else if (field.getType() == Field::I32_TYPE){
			desiredName = typeid(GMSEC_I32).name();
		} else if (field.getType() == Field::I64_TYPE){
			desiredName = typeid(GMSEC_I64).name();
		} else if (field.getType() == Field::F32_TYPE){
			desiredName = typeid(GMSEC_F32).name();
		} else if (field.getType() == Field::F64_TYPE){
			desiredName = typeid(GMSEC_F64).name();
		} else if (field.getType() == Field::STRING_TYPE){
			desiredName = typeid(char*).name();
		} else if (field.getType() == Field::U8_TYPE){
			desiredName = typeid(GMSEC_U8).name();
		} else if (field.getType() == Field::U16_TYPE){
			desiredName = typeid(GMSEC_U16).name();
		} else if (field.getType() == Field::U32_TYPE){
			desiredName = typeid(GMSEC_U32).name();
		} else if (field.getType() == Field::U64_TYPE){
			desiredName = typeid(GMSEC_U64).name();
		} else {
			desiredName = "Unknown field";
		}

		return typeName.compare(desiredName) == 0;
	}

	T      m_value;
	size_t m_valueSize;

	friend class InternalTester;
};

} //namespace internal
} //namespace mist
} //namespace api
} //namespace gmsec

#endif
