/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file Specification.h
 *
 *  @brief This file contains a class for managing MessageTemplates and validating Messages.
 *
**/

#ifndef GMSEC_MIST_SPECIFICATION_H
#define GMSEC_MIST_SPECIFICATION_H

#include <gmsec4/mist/SchemaIDIterator.h>

namespace gmsec
{
namespace api
{
	class Config;
	class Message;

namespace mist
{

	namespace internal
	{
		// Forward declaration(s)
		class MessageTemplate;
		class InternalSpecification;
		class SpecificationBuddy;
	}

/** @class Specification
 *
 * @brief The Specification class loads a list of templates from a designated directory
 * that are used to create and validate message schemas. Each template has an associated ID
 * that is used to identify the kind of message schema the template will be defining.
 *
 * @sa Config @n
 *     SchemaIDIterator
 */
class GMSEC_API Specification
{
public:
	/** @fn Specification(const Config& config)
	 *
	 * @brief Default constructor - initializes the Specification instance with a configuration.
	 *
	 * @param config - The configuraton file that Specification uses
	 *
	 * @throw An exception is thrown if it cannot deduce the values passed to it by the configuration
	 */
	Specification(const Config& config);


	/** @fn Specification(const Specification& other)
	 *
	 * @brief Copy constructor - initializes the Specification instance based on another instance
	 *
	 * @param other - The Specification object to be copied
	 */
	Specification(const Specification& other);


	/** @fn ~Specification()
	 *
	 * @brief Destructor - deletes InternalSpecification pointer
	 */
	virtual ~Specification();


	/** @fn validateMessage(const Message& msg)
	 *
	 * @brief Looks up the message subject in the message registry to grab the
	 * appropriate template (based on its assigned schema ID).  The contents of the 
	 * message are then compared to the template to ensure the message complies with 
	 * the ISD. This function will also register the message with an appropriate schema 
	 * ID if it is not found in the registry.
	 *
	 * @param message - The message to be validated.
	 * 
	 * @throws An exception is thrown if the message fails to pass validation.  
	 * A list of errors will be given of any issues found with the message.
	 */
	virtual void CALL_TYPE validateMessage(const Message& msg);


   /**
    * @fn getSchemaIDIterator()
	* 
	* @brief returns a SchemaIDIterator, which can be used to iterated over the IDs
	* of the loaded templates.
	*
	* @note Only one SchemaIDIterator object is associated with a %Specification object; multiple calls to 
	* getSchemaIDIterator() will return a reference to the same SchemaIDIterator object.  Each call will reset the iterator.
	*
	* @return a reference to a SchemaIDIterator object
    */
	SchemaIDIterator& getSchemaIDIterator();


   /**
    * @fn getVersion()
	*
	* @brief returns the version of the ISD being used as an unsigned integer.
    */
	unsigned int getVersion() const;


   /**
    * @fn getTemplateXML(const char* subject, const char* schemaID)
	*
	* @brief Returns a message schema from the template with the matching schemaID.
	* This message schema can be used as input for a Message data constructor
	*
	* @param subject - message subject
	* @param schemaID - id used to look up the appropriate template
	*
	* @return XML string representation of the message schema.
	*
	* @throw An exception is thrown if the provided schemaID is not available in the list of loaded schema.
    */
	const char* getTemplateXML(const char* subject, const char* schemaID);

private:
	friend class gmsec::api::mist::internal::SpecificationBuddy;

	// Defined, but not implemented
	Specification& operator=(const Specification&);

	internal::InternalSpecification* m_iSpec;
};

} // namespace mist
} // namespace api
} // namespace gmsec

#endif
