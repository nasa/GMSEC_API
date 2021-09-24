
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file BaseMessageTemplate.h
 *
 *  @brief This file contains a class define a Message type
 *
**/

#ifndef gmsec_BaseMessageTemplate_h
#define gmsec_BaseMessageTemplate_h

#include <gmsec/internal/mist/Context.h>

#include <gmsec/Message.h>

namespace gmsec
{
namespace mist
{


namespace internal
{
class AbstractFieldDescriptor;
class BaseTester;
class BaseSpecification;
class DescriptorList;

/** @class BaseMessageTemplate
 * @brief The BaseMessageTemplate is the class that represents a specific message
 * type.  It contains a list of AbstractFieldDescriptors stored in a 
 * DescriptorList object.  It contains a bool for allowing user defined fields
 * and methods for applying, instantiating, and validating a Message.
 *
 * @sa gmsec::mist::AbstractFieldDescriptor
 * @sa gmsec::mist::DescriptorList
 * @sa gmsec::mist::Context
*/
class GMSEC_API BaseMessageTemplate
{
public:

	/** @fn BaseMessageTemplate()
	 * @brief Default constructor - initializes the BaseMessageTemplate instance.
	 * Sets the allowUserDefinedFields to false and fields to NULL
	 *
	 */
	BaseMessageTemplate();

	/** @fn ~BaseMessageTemplate()
	 * @brief Destructor - cleans up the fields member
	 */
	~BaseMessageTemplate();

	/** @fn Validate(Message *message, Context &context) const
	 * @brief Compares the fields of the Message parameter with the field 
	 * descriptors described by the MessageTemplates fields to verify that
	 * types and names are correct and all required fields are present
	 *
	 * @param message - The fields contained in the reference Message are
	 * validated against the MessageTemplate fields
	 * @param context - The reference to the Context object stores the statuses
	 * that are generated
	 *
	 * @return status - result of operation
	 */
	Status Validate(Message *message, Context &context) const;

	/** @fn Instantiate(Message *message, Context &context) const
	 * @brief Adds the fields described by the MessageTemplates fields
	 * to the Message parameter
	 *
	 * @param message - The MessageTemplate fields are added to the Message
	 * reference
	 * @param context - The reference to the Context object stores the statuses
	 * that are generated
	 *
	 * @return status - result of operation
	 */
	Status Instantiate(Message *message, Context &context) const;

	/** @fn Apply(Message *message, Context &context) const
	 * @brief Adds the fields described by the MessageTemplates fields
	 * to the Message parameter if they do not already exist.  Validates
	 * that already existing fields are correct based on MessageTemplate
	 * fields if context's validateOnApply is set to VALIDATE_ON_APPLY
	 *
	 * @param message - The MessageTemplate fields are added to the Message
	 * reference
	 * @param context - The reference to the Context object stores the statuses
	 * that are generated
	 *
	 * @return status - result of operation
	 */
	Status Apply(Message *message, Context &context) const;

	/** @fn SetFields(util::List<AbstractFieldDescriptor*> &inputFields)
	 * @brief sets the fields class member to a DescriptorList generated from
	 * inputFields and takes ownership of inputFields
	 *
	 * @param inputFields - The list of AbstractFieldDescriptor is used as 
	 * the fields member
	 *
	 * @return status - result of operation
	 */
	Status SetFields(util::List<AbstractFieldDescriptor*> &inputFields);

	/** @fn SetAllowUserDefinedFields(bool allow)
	 * @brief updates the allowUserDefinedFields to the allow parameter
	 *
	 * @param allow - bool that specifies messages whether being validated against
	 * this template are allowed to have additional fields
	 *
	 * @return status - result of operation
	 */
	Status SetAllowUserDefinedFields(bool allow);

private:
	BaseMessageTemplate &operator=(const BaseMessageTemplate &);
	BaseMessageTemplate(const BaseMessageTemplate &);

	DescriptorList *fields;
	bool allowUserDefinedFields;	

	friend class BaseTester;
};

} //end internal

} //end mist

} //end gmsec

#endif
