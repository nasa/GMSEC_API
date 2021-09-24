
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file Specification.h
 *
 *  @brief This file contains a class for managing MessageTemplates and validating Messages.
 *
**/

#ifndef gmsec_Specification_h
#define gmsec_Specification_h

#include <gmsec/mist/ConnectionManager.h>

#include <gmsec/internal/mist/Context.h>
#include <gmsec/internal/mist/MessageTemplate.h>


namespace gmsec
{
namespace mist
{
namespace internal
{
class BaseTester;


/** @class Specification
 * @brief The Specification class is a wrapper from the BaseSpecification object.
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
	 * Initializes the pointer to BaseSpecification.
	 *
	 * @param config - The configuraton file that Specification uses
	 */
	Specification();

	/** @fn ~Specification()
	 * @brief Destructor - deletes BaseSpecification pointer
	 */
	~Specification();

	/** @fn LoadTemplate(const char * id, const char * path)
	 * @brief LoadTemplate uses the path parameter and looks up the MessageTemplate
	 * stored in xml.  Using this it adds the parameter id and MessageTemplate to 
	 * the map.
	 *
	 * @param id - The const char * stores the id for the template
	 * @param path - The const char * stores the path for the template
	 *
	 * @return status - result of operation
	 */
	Status LoadTemplate(const char * id, const char * path);

	/** @fn Load(const char* basePath = NULL, SpecificationVersion version = ISD_CURRENT)
	 * @brief Load(const char* basePath) uses the config member and looks up the MessageTemplates
	 * stored in xml.  Using this the BaseSpecification's function generates the 
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
	Status Load(const char* basePath = NULL, unsigned int version = gmsec::mist::ConnectionManager::GMSEC_ISD_CURRENT);

	/** @fn Apply(Message *message, Context &context) const
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
	Status Apply(Message *message, Context &context) const;

	/** @fn Instantiate(Message *message, Context &context) const
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
	Status Instantiate(Message *message, Context &context) const;

	/** @fn Validate(Message &message, Context &context) const
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
	Status Validate(Message *message, Context &context) const;

	/** @fn Register(const char* id, MessageTemplate* newTemplate)
	 * @brief Adds the MessageTemplate and id to BaseSpecification's map, passing ownership
	 * of newTemplate to BaseSpecification
	 *
	 * @param id - The const char * to store as the key in the MessageTemplate map
	 * @param newTemplate - The MessageTemplate to be paired with the template id
	 *
	 * @return status - result of operation
	 */
	Status Register(const char* id, MessageTemplate* newTemplate);

	/** @fn GetTemplateIds(gmsec::util::List<gmsec::util::String>& idList);
	 * @brief Fetches list of template IDs.
	 *
	 * @param idList - (output)
	 *	 Basically all the string keys to the private member
	 *	 templates, which is a map of all the template IDs
	 *	 to actual MessageTemplate instances in this
	 *	 object.
	 *
	 */
	void GetTemplateIds(gmsec::util::List<gmsec::util::String>& idList);

	/** @fn LookupTemplate(Message* message, gmsec::util::String& idString)
	 * 
	 * @brief Checks if a given message (inputted as parameter message
	 *	by the user) has a matching message template in this 
	 *	BaseSpecification object, and if so gives the user the 
	 *	matching template ID name in output parameter idString.  If
	 *	there is no matching template ID, false is returned
	 *	and the output parameter idString is assigned to be
	 *	the empty string.
	 *
	 * @param message  - (input)
	 *   GMSEC Message to validate.
	 *
	 * @param idString - (output)
	 *   Message template ID in this BaseSpecification
	 *   which matches the given message, if one exists;
	 *   otherwise the empty string is assigned.
	 *
	 * @return status  - result of operation
	 */
	Status LookupTemplate(Message* message, gmsec::util::String& idString);

private:
	// not implemented
	Specification &operator=(const Specification &);
	Specification(const Specification &);

	internal::BaseSpecification* ptr;

	friend class internal::BaseTester;
};

} //end internal

} //end mist

} //end gmsec

#endif
