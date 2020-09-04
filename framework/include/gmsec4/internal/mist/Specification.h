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

#include <gmsec4/mist/ConnectionManager.h>

#include <gmsec4/util/DataList.h>

#include <string>


namespace gmsec
{
namespace api
{
namespace mist
{
namespace internal
{
	// Forward declaration(s)
	class Context;
	class MessageTemplate;
	class InternalSpecification;
	class InternalTester;


/** @class Specification
 * @brief The Specification class is a wrapper from the InternalSpecification object.
 * It handles the storing and loading of MessageTemplates and allows users to
 * register their own templates.  If a user does not load or create their own custom
 * MessageTemplate, this is the only class in MIST that will be 
 * used.  It's Apply, Instantiate, and Validate functions are the functions
 * called when a user wants to generate or validate a Message from a 
 * MessageTemplate.
 *
 * @sa gmsec::mist::MessageTemplate
 * @sa gmsec::mist::Context
*/
class GMSEC_API Specification
{
public:

	/** @fn Specification()
	 * @brief Default constructor - initializes the Specification instance.
	 * Initializes the pointer to InternalSpecification.
	 *
	 * @param config - The configuraton file that Specification uses
	 */
	Specification();


	/** @fn ~Specification()
	 * @brief Destructor - deletes InternalSpecification pointer
	 */
	~Specification();


	/** @fn loadTemplate(const char* id, const char* path)
	 * @brief LoadTemplate uses the path parameter and looks up the MessageTemplate
	 * stored in xml.  Using this it adds the parameter id and MessageTemplate to 
	 * the map.
	 *
	 * @param id - The const char * stores the id for the template
	 * @param path - The const char * stores the path for the template
	 *
	 * @return status - result of operation
	 */
	Status loadTemplate(const char* id, const char* path);


	/** @fn load(const char* basePath = NULL, SpecificationVersion version = ISD_CURRENT)
	 * @brief Load(const char* basePath) uses the config member and looks up the MessageTemplates
	 * stored in xml.  Using this the InternalSpecification's function generates the 
	 * map matching Template ids to MessageTemplates.
	 *
	 * @param basePath - Optional user-supplied parameter indicating
	 *				   directory path where XML schema template files
	 *				   reside.
	 *
	 * @param version  - GMSEC ISD version.
	 *
	 * @return status - result of operation
	 */
	Status load(const char* basePath = NULL, unsigned int version = gmsec::api::mist::GMSEC_ISD_CURRENT);


	/** @fn apply(Message& message, Context& context) const
	 * @brief Uses the templateId stored in context to lookup a MessageTemplate,
	 * then adds the fields described by the MessageTemplate fields
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


	/** @fn instantiate(Message& message, Context& context) const
	 * @brief Uses the templateId stored in context to lookup a MessageTemplate,
	 * then adds the fields described by the MessageTemplates fields
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


	/** @fn validate(Message& message, Context& context) const
	 * @brief Uses the templateId stored in context to lookup a MessageTemplate,
	 * then compares the fields of the Message parameter with the field 
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


	/** @fn registerTemplate(const char* id, MessageTemplate* newTemplate)
	 * @brief Adds the MessageTemplate and id to InternalSpecification's map, passing ownership
	 * of newTemplate to InternalSpecification
	 *
	 * @param id - The const char * to store as the key in the MessageTemplate map
	 * @param newTemplate - The MessageTemplate to be paired with the template id
	 *
	 * @return status - result of operation
	 */
	Status registerTemplate(const char* id, MessageTemplate* newTemplate);


	/**
	 * @fn getTemplateIds(gmsec::api::util::DataList<std::string>& idList);
	 * @brief Fetches list of template IDs.
	 * @param templateIDs - (output) A list of names of the MessageTemplate instances this Specification maintains.
	 */
	void getTemplateIds(gmsec::api::util::DataList<std::string>& templateIDs) const;


	/**
	 * @fn Status lookupTemplate(const Message& message, std::string& templateID)
	 * 
	 * @brief Checks if a given message (inputted as parameter message
	 *	by the user) has a matching message template in this 
	 *	InternalSpecification object, and if so gives the user the 
	 *	matching template ID name in output parameter idString.  If
	 *	there is no matching template ID, false is returned
	 *	and the output parameter idString is assigned to be
	 *	the empty string.
	 *
	 * @param message    - (input) GMSEC Message to validate.
	 * @param templateID - (output) Template ID which matches the given message.
	 *
	 * @return A Status indicating whether the operation was successful or not.
	 */
	Status lookupTemplate(const Message& message, std::string& templateID);

private:
	// Defined, but not implemented
	Specification(const Specification&);
	Specification& operator=(const Specification&);

	internal::InternalSpecification* m_iSpec;

	friend class InternalTester;
};

} //namespace internal
} //namespace mist
} //namespace api
} //namespace gmsec

#endif
