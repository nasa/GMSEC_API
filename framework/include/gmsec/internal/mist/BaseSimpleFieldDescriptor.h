
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file BaseSimpleFieldDescriptor.h
 *
 *  @brief This file contains the BaseSimpleFieldDescriptor class
 *         which desribes Fields.
 *
**/

#ifndef gmsec_BaseSimpleFieldDescriptor_h
#define gmsec_BaseSimpleFieldDescriptor_h


#include <gmsec/mist/MIST_errors.h>

#include <gmsec/internal/mist/Context.h>
#include <gmsec/internal/mist/AbstractBaseFieldDescriptor.h>
#include <gmsec/internal/mist/BaseContext.h>
#include <gmsec/internal/mist/FieldAccessTemplate.h>

#include <gmsec/Message.h>

#include <gmsec/internal/strutil.h>


namespace gmsec
{

namespace mist
{

namespace internal
{
class AbstractFieldDescriptor;
class BaseTester;


/***********************************************/
/** @class BaseSimpleFieldDescriptor
 * @brief The BaseSimpleFieldDescriptor object represents a simple field
 * that contains a value of the template type.  It extends the 
 * BaseAbstractFieldDescriptor and adds getters and setters for value
 * as well as a function to generate a Field from the value information
 *
 * @sa gmsec::mist::internal::AbstractBaseFieldDescriptor
 * @sa gmsec::mist::SimpleFieldDescriptor
 * @sa gmsec::Field
 * @sa gmsec::mist::Context
*/
template <class type> class GMSEC_API BaseSimpleFieldDescriptor : public AbstractBaseFieldDescriptor
{
public:

	/** @fn BaseSimpleFieldDescriptor()
	 * @brief Default constructor - initializes the BaseSimpleFieldDescriptor
	 * instance, calls the AbstractBaseFieldDescriptor default constructor and
	 * sets the value to null
	 *
	 */
	BaseSimpleFieldDescriptor()
	:AbstractBaseFieldDescriptor(),
	 fvalue(0), fvalue_size(0)
	{};

	/** @fn BaseSimpleFieldDescriptor(type &newValue)
	 * @brief Constructor - initializes the BaseSimpleFieldDescriptor
	 * instance, calls the AbstractBaseFieldDescriptor default constructor and
	 * sets the children to a DescriptorList generated with newChildren, taking 
	 * ownership of the variable
	 *
	 * @param newValue - set to the value member.  Ownership is taken by 
	 * BaseSimpleFieldDescriptor
	 *
	 */
	BaseSimpleFieldDescriptor(type& newValue)
	:AbstractBaseFieldDescriptor(),
	 fvalue(newValue), fvalue_size(0)
	{};

	/** @fn BaseSimpleFieldDescriptor(type &newValue, size_t size)
	 * @brief Constructor - initializes the BaseSimpleFieldDescriptor
	 * instance, calls the AbstractBaseFieldDescriptor default constructor and
	 * sets the children to a DescriptorList generated with newChildren, taking 
	 * ownership of the variable
	 *
	 * @param newValue - set to the value member.  Ownership is taken by 
	 * @param size - set to the value member size.
	 *
	 * BaseSimpleFieldDescriptor
	 *
	 */
	BaseSimpleFieldDescriptor(type& newValue, GMSEC_U32 size)
	:AbstractBaseFieldDescriptor(),
	 fvalue(newValue), fvalue_size(size)
	{};

	/** @fn ~BaseSimpleFieldDescriptor()
	 * @brief Destructor
	 */
	virtual ~BaseSimpleFieldDescriptor()
	{};
	
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
		Status status;

		context.ptr->IncrementDepth(fname);
		std::string fullPathName;
		context.ptr->FullNamePath(fullPathName);
		context.ptr->AddVisited();	

		Field field;

		field.SetName(fullPathName.c_str());

		FieldAccessTemplate<type> field_access_template(&field);

		field_access_template.SetValue(fvalue, fvalue_size);

		if(fclass == F_HEADER)
			field.SetHeader(true);
		status = message->AddField(field);
		if(status.IsError())
		{
			std::string code("Error adding field: ");
			code += fullPathName;
			context.ptr->AddStatus(fullPathName, status.GetCode(), code);
		}
	
		context.ptr->DecrementDepth();
		return status;
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
		return FindField(message,context,VALIDATE);
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
		return FindField(message, context, validate);
	};

	/** @fn SetValue(type &value, size_t size = 0)
	 * @brief Updates the value member with the value parameter and takes ownership
	 * of the parameter
	 *
	 * @param value - The value of the type of the template and replaces the class
	 * member value
	 * @param size - set to the value member size.
	 *
	 * @return status - result of operation
	 */
	Status SetValue(type &value, GMSEC_U32 size = 0)
	{
		Status status;
		fvalue = value;
		fvalue_size = size;
		return status;
	};
	
	/** @fn GetValue(type &value)
	 * @brief Sets the value parameter to the value class member.
	 *
	 * @param value - A reference of the type of the template that is set to 
	 * the class member value
	 *
	 * @return status - result of operation
	 */
	Status GetValue(type &value) const
	{
		Status status;
		value = fvalue;
		return status;
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
		Status status;		
		context.ptr->IncrementDepth(fname);
		
		Field msgField;
		//iterating through message fields
		Status newStatus = Status();
		newStatus = message->GetFirstField(msgField); 
		while(!(newStatus.isError()))
		{
			const char * name;
			msgField.GetName(name);
			std::string fpname;
			Status status = context.ptr->FullNamePath(fpname);
			if(util::stringEquals(fpname.c_str(),name))
			{
				context.ptr->AddVisited();
				if(validate == VALIDATE ||
				   validate == VALIDATE_ON_APPLY)
				{
					type compType;
					FieldAccessTemplate<type>
					  field_access_template(&msgField);
					GMSEC_U32 size;
					status = field_access_template.GetValue(
					           compType, size
					);
					if(status.isError()) //and compare mode?
					{
						std::string code("Field ");
						code += fpname;
						code += std::string(" does not have the appropriate type");
						const char* codeChar = util::stringNew(code.c_str());
						context.ptr->AddStatus(name, GMSEC_INCORRECT_FIELD_TYPE, codeChar);
					}
				}
				//delete[] name;
				context.ptr->DecrementDepth();
				return status;
				}
			newStatus = message->GetNextField(msgField);
		}

		if (validate == 0)
		{
			context.ptr->AddVisited();
			if (fmode == F_REQUIRED)
			{
				std::string fpname;

				status = context.ptr->FullNamePath(fpname);
				std::string code("Required field ");

				code += fpname;
				code += std::string(" missing.");
				context.ptr->AddStatus(fpname, GMSEC_MISSING_REQUIRED_FIELD, code);

				status.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_MISSING_REQUIRED_FIELD, code.c_str());
			}
		}
		else
		{
			context.ptr->DecrementDepth();
			return AddTo(message, context);
		}

		context.ptr->DecrementDepth();
		return status;
	};

private:
	BaseSimpleFieldDescriptor &operator=(const BaseSimpleFieldDescriptor &);
	BaseSimpleFieldDescriptor(const BaseSimpleFieldDescriptor &);

	type       fvalue;
	GMSEC_U32  fvalue_size;

	friend class BaseTester;
};

} //end internal

} //end mist

} //end gmsec

#endif
