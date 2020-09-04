/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file MessageTemplate.h
 *
 *  @brief This file contains a class for defining a type of Message.
 *
**/

#ifndef GMSEC_MIST_MESSAGE_TEMPLATE_H
#define GMSEC_MIST_MESSAGE_TEMPLATE_H

#include <gmsec4/internal/mist/Context.h>

#include <gmsec4/Status.h>

#include <vector>


namespace gmsec
{
namespace api
{
	// Forward declaration(s)
	class Message;

namespace mist
{
namespace internal
{

enum FieldMode
{
	F_OPTIONAL,
	F_REQUIRED
};

enum FieldClass
{
	F_NONE,
	F_STANDARD,
	F_HEADER,
	F_USER,
	F_API,
	F_VARIABLE
};

enum MIST_VALIDATE_VALS
{
	VALIDATE = 0,
	VALIDATE_ON_APPLY = 1,
	NO_VALIDATE_ON_APPLY = 2
};

	// Forward declaration(s)
	class AbstractFieldDescriptor;
	class InternalMessageTemplate;
	class InternalSpecification;
	class InternalTester;


/** @class MessageTemplate
 * @brief The MessageTemplate is the a wrapper class for the BaseMessageTempalte
 * class.  It allows for the construction of a user defined MessageTemplate through
 * the constructor by supplying a list of AbstractFieldDescriptors and a bool 
 * that specifies if additional fields are allowed.
 *
 * @sa gmsec::mist::AbstractFieldDescriptor
 * @sa gmsec::mist::internal::BaseMessageTemplate
 * @sa gmsec::mist::Context
*/
class GMSEC_API MessageTemplate
{
public:
	/** @fn MessageTemplate()
	 * @brief Default constructor - initializes the MessageTemplate instance and the
	 * pointer to BaseMessageTemplate is initialized with the allowUserDefinedFields 
	 * set to false and fields to NULL
	 *
	 */
	MessageTemplate();


	/** @fn MessageTemplate(std::vector<AbstractFieldDescriptor*>& inputFields, bool allow)
	 * @brief sets the BaseMessageTemplate's fields class member to a DescriptorList 
	 * generated from inputFields and BaseMessageTemplate takes ownership of inputFields
	 *
	 * @param inputFields - The list of Fields is used as the fields member
	 */
	MessageTemplate(std::vector<AbstractFieldDescriptor*>& inputFields, bool allow);


	/** @fn ~MessageTemplate()
	 * @brief Destructor - cleans up the BaseMessageTemplate pointer
	 */
	~MessageTemplate();

private:
	// Defined, but not implemented
	MessageTemplate(const MessageTemplate&);
	MessageTemplate& operator=(const MessageTemplate&);


	/** @fn validate(Message& message, Context& context) const
	 * @brief Compares the fields of the Message parameter with the field 
	 * descriptors described by the MessageTemplate's fields to verify that
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


	/** @fn Instantiate(Message& message, Context& context) const
	 * @brief Adds the fields described by the MessageTemplate's fields
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


	/** @fn Apply(Message& message, Context& context) const
	 * @brief Adds the fields described by the MessageTemplate's fields
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


	internal::InternalMessageTemplate* m_iMsgTemplate;

	friend class InternalSpecification;
	friend class InternalTester;
};

} //namespace internal
} //namespace mist
} //namespace api
} //namespace gmsec

#endif
