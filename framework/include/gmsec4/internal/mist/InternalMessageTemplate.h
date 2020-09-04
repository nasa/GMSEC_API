/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file InternalMessageTemplate.h
 *
 *  @brief This file contains a class define a Message type
 *
**/

#ifndef GMSEC_MIST_INTERNAL_MESSAGE_TEMPLATE_H
#define GMSEC_MIST_INTERNAL_MESSAGE_TEMPLATE_H

#include <gmsec4/internal/mist/Context.h>

#include <vector>


namespace gmsec
{
namespace api
{
	// Forward declaration(s)
	class Field;
	class Message;

namespace mist
{
namespace internal
{
	// Forward declaration(s)
	class AbstractFieldDescriptor;
	class DescriptorList;
	class InternalSpecification;
	class InternalTester;


/** @class InternalMessageTemplate
 * @brief The InternalMessageTemplate is the class that represents a specific message
 * type.  It contains a list of AbstractFieldDescriptors stored in a 
 * DescriptorList object.  It contains a bool for allowing user defined fields
 * and methods for applying, instantiating, and validating a Message.
 *
 * @sa gmsec::mist::DescriptorList
 * @sa gmsec::mist::Context
*/
class GMSEC_API InternalMessageTemplate
{
public:

	/** @fn InternalMessageTemplate()
	 * @brief Default constructor - initializes the InternalMessageTemplate instance.
	 * Sets the allowUserDefinedFields to false and fields to NULL
	 *
	 */
	InternalMessageTemplate();


	/** @fn ~InternalMessageTemplate()
	 * @brief Destructor - cleans up the fields member
	 */
	~InternalMessageTemplate();


	/** @fn validate(Message& message, Context& context)
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
	Status validate(Message& message, Context& context) const;


	/** @fn instantiate(Message& message, Context& context) const
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
	Status instantiate(Message& message, Context& context) const;


	/** @fn apply(Message& message, Context& context) const
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
	Status apply(Message& message, Context& context) const;


	/** @fn setFields(const std::vector<AbstractFieldDescriptor*>& inputFields)
	 * @brief sets the fields class member to a DescriptorList generated from
	 * inputFields and takes ownership of inputFields
	 *
	 * @param inputFields - The list of AbstractFieldDescriptor is used as 
	 * the fields member
	 */
	void setFields(std::vector<AbstractFieldDescriptor*>& inputFields);


	/** @fn SetAllowUserDefinedFields(bool allow)
	 * @brief updates the allowUserDefinedFields to the allow parameter
	 *
	 * @param allow - bool that specifies messages whether being validated against
	 * this template are allowed to have additional fields
	 */
	void setAllowUserDefinedFields(bool allow);


private:
	InternalMessageTemplate(const InternalMessageTemplate&);
	InternalMessageTemplate& operator=(const InternalMessageTemplate&);

	DescriptorList* m_fields;
	bool            m_allowUserDefinedFields;

	friend class InternalTester;
};

} //namespace internal
} //namespace mist
} //namespace api
} //namespace gmsec

#endif
