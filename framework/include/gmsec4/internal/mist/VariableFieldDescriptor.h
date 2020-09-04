/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file VariableFieldDescriptor.h
 *
 *  @brief This file contains the VariableFieldDescriptor class
 *         which desribes Variable Fields, given no fixed type in
 *		   the ISD
 *
**/

#ifndef GMSEC_MIST_INTERNAL_VARIABLE_FIELD_DESCRIPTOR_H
#define GMSEC_MIST_INTERNAL_VARIABLE_FIELD_DESCRIPTOR_H


#include <gmsec4/internal/mist/AbstractFieldDescriptor.h>

#include <gmsec4/internal/mist/Context.h>
#include <gmsec4/internal/mist/InternalContext.h>
#include <gmsec4/internal/mist/FieldAccessTemplate.h>

#include <gmsec4/Fields.h>
#include <gmsec4/Message.h>
#include <gmsec4/MessageFieldIterator.h>
#include <gmsec4/util/Log.h>

#include <gmsec4/internal/StringUtil.h>


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


/***********************************************/
/** @class VariableFieldDescriptor
 * @brief The VariableFieldDescriptor object represents a variable field.  
 * It extends the AbstractFieldDescriptor.
 *
 * @sa gmsec::mist::internal::AbstractInternalFieldDescriptor
 * @sa gmsec::mist::SimpleFieldDescriptor
 * @sa gmsec::Field
 * @sa gmsec::mist::Context
*/
class GMSEC_API VariableFieldDescriptor : public AbstractFieldDescriptor
{
public:
	/** @fn VariableFieldDescriptor()
	 * @brief Default constructor - initializes the InternalSimpleFieldDescriptor
	 * instance, calls the AbstractInternalFieldDescriptor default constructor and
	 * sets the value to null
	 *
	 */
	VariableFieldDescriptor()
		: AbstractFieldDescriptor()
	{
	}

	/** @fn ~InternalSimpleFieldDescriptor()
	 * @brief Destructor
	 */
	virtual ~VariableFieldDescriptor()
	{
	}

	virtual void setFieldVariables(const char* name, FieldMode mode, FieldClass clss, const char* description)
	{
		if (!name || std::string(name).empty())
		{
			throw Exception(MIST_ERROR, UNINITIALIZED_OBJECT, "Name is null, or is empty string");
		}
		if (!description)
		{
			throw Exception(MIST_ERROR, UNINITIALIZED_OBJECT, "Description is null");
		}

		m_name        = name;
		m_mode        = mode;
		m_class       = clss;
		m_description = description;
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

		//MEH: This function cannot be invoked from user code. 
		//Implementation is planned for MIST 3.
		GMSEC_ERROR << "VariableFieldDescriptor::addTo not implemented";

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
		//MEH: This function cannot be invoked from user code. 
		//Implementation is planned for MIST 3.
		GMSEC_ERROR << "VariableFieldDescriptor::apply not implemented";
		return Status();
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
	VariableFieldDescriptor(const VariableFieldDescriptor&);
	VariableFieldDescriptor& operator=(const VariableFieldDescriptor&);

	std::string m_name;
	FieldMode   m_mode;
	FieldClass  m_class;
	std::string m_description;
};

} //namespace internal
} //namespace mist
} //namespace api
} //namespace gmsec

#endif
