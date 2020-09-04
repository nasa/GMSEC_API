/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file InternalSpecification.h
 *
 *  @brief This file contains a class handle MessageTemplates and Message validation
 *
**/

#ifndef GMSEC_MIST_INTERNAL_SPECIFICATION_H
#define GMSEC_MIST_INTERNAL_SPECIFICATION_H

#include <gmsec4/mist/ConnectionManager.h>

#include <gmsec4/Status.h>

#include <gmsec4/internal/tinyxml2.h>

#include <list>
#include <map>
#include <string>
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
	// Forward declaration(s)
	class AbstractFieldDescriptor;
	class Context;
	class MessageTemplate;
	class InternalTester;


/** @class InternalSpecification
 * @brief The InternalSpecification object handles the storing and loading of
 * MessageTemplates.  If a user does not load or create their own custom
 * MessageTemplate, this and InternalContext are the only classes that will be 
 * used.  It's Apply, Instantiate, and Validate functions are the functions
 * called when a user wants to generate or validate a Message from a 
 * MessageTemplate.  It contains a map, matching templateIds to MessageTemplates
 *
 * @sa gmsec::mist::MessageTemplate
 * @sa gmsec::mist::Context
*/
class GMSEC_API InternalSpecification
{
public:
	/** @fn InternalSpecification()
	 * @brief Default constructor - initializes the InternalSpecification instance.
	 * Sets the config member to the config parameter.
	 *
	 * @param config - The configuraton file that Specification uses
	 */
	InternalSpecification();


	/** @fn ~InternalSpecification()
	 * @brief Destructor - cleans up the MessageTemplates
	 */
	~InternalSpecification();


	/** @fn load(const char* basePath = NULL, SpecificationVersion version)
	 * @brief Load(const char* basePath = NULL) uses the config member and looks up the MessageTemplates
	 * stored in xml.  Using this it generates the map matching Template ids to 
	 * MessageTemplates.
	 *
	 * @param basePath - Optional pathname of the directory containing
	 *				   message template schema XML files during
	 *				   run-time, typically GMSEC_API/templates.
	 *
	 * @param version  - GMSEC ISD version.
	 *
	 * @return status - result of operation
	 */
	Status load(const char* basePath = NULL, unsigned int version = gmsec::api::mist::GMSEC_ISD_CURRENT);


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
	 * @brief Adds the MessageTemplate and id to the map, taking ownership
	 * of newTemplate
	 *
	 * @param id - The const char * to store as the key in the MessageTemplate map
	 * @param newTemplate - The MessageTemplate to be paired with the template id
	 *
	 * @return status - result of operation
	 */
	Status registerTemplate(const char* id, MessageTemplate* newTemplate);


	/**
	 * @fn getTemplateIds(gmsec::util::List<gmsec::util::String>& idList);
	 * @brief Fetches list of template IDs.
	 * @return A list of names of the MessageTemplate instances this Specification maintains.
	 */
	 std::list<std::string> getTemplateIds() const;


	/** @fn std::string lookupTemplate(const Message& message)
	 * 
	 * @brief Checks if a given message (inputted as parameter message
	 *		by the user) has a matching message template in this 
	 *		InternalSpecification object, and if so gives the user the 
	 *		matching template ID name in output parameter idString.  If
	 *		there is no matching template ID, false is returned
	 *		and the output parameter idString is assigned to be
	 *		the empty string.
	 *
	 * @param message  - (input)
	 *				   GMSEC Message to validate.
	 *
	 * @return Message template ID in this InternalSpecification
	 *         which matches the given message, if one exists;
	 *         otherwise the empty string is assigned.
	 *
	 * @throw An Exception is thrown is the message has an invalid Field type
	 *        or if a MessageTemplate cannot be found.
	 */
	std::string lookupTemplate(const Message& message);


private:
	/** @fn parser
	 *		(tinyxml2::XMLElement* xmlSchema,
	 *		 std::string& id,
	 *		 std::string& idFieldNames,
	 *		 std::vector<AbstractFieldDescriptor*>& schemaFields)
	 *
	 *  @brief Translate the XML specification in a TinyXML (which is 
	 *		 a 3rd-party package) node into an ID string and list of 
	 *		 MIST FieldDescriptors, which is intended to be inserted 
	 *		 into a MIST MessageTemplate.
	 *
	 *  @param xmlSchema	- (input)
	 *                        TinyXML node which is the root of an XML tree
	 *
	 *  @param id		- (output)
	 *                        MIST Specification id for this schema to be
	 *                        assigned.
	 *
	 *  @param idFieldNames	- (output)
	 *                        MIST Specification id field names in a message
	 *                        to be checked against an id name.
	 *
	 *  @param schemaFields - (output)
	 *                        List of Field instances
	 *                        that will be produced from the xmlSchema
	 *
	 * @return status - result of operation
	 */
	Status parser(tinyxml2::XMLElement* xmlSchema, std::string& id, std::string& idFieldNames,
	              std::vector<AbstractFieldDescriptor*>& schemaFields);


	/** @fn loadDefaultTemplatesFromMemory(unsigned int version)
	 *
	 * @brief Fetch default message schemas, from static memory, into
	 *		the private InternalSpecification::templates list
	 *		(declared below).  It is presumed that this function
	 *		will only be called if a schema load from a directory
	 *		location fails.
	 *
	 * @return status - result of operation
	 */
	Status loadDefaultTemplatesFromMemory(unsigned int version);


	typedef std::map<std::string, MessageTemplate*> Templates;    //id, template
	typedef std::map<std::string, std::string>      FieldNames;   //id, field name
	typedef std::list<unsigned char*>               HeapList;

	Templates    m_templates;
	FieldNames   m_idFieldNames;
	std::string  m_specificationId;
	HeapList     m_heapList;

	friend class InternalTester;
};

} //namespace internal
} //namespace mist
} //namespace api
} //namespace gmsec

#endif
