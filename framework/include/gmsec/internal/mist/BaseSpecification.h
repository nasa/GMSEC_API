
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file BaseSpecification.h
 *
 *  @brief This file contains a class handle MessageTemplates and Message validation
 *
**/

#ifndef gmsec_BaseSpecification_h
#define gmsec_BaseSpecification_h

#include <gmsec/mist/ConnectionManager.h>

#include <gmsec/util/List.h>
#include <gmsec/util/String.h>

#include <tinyxml2.h>
#include <map>
#include <string>


namespace gmsec
{

namespace mist
{
namespace internal
{

class AbstractFieldDescriptor;
class Context;
class MessageTemplate;


class BaseTester;

/** @class BaseSpecification
 * @brief The BaseSpecification object handles the storing and loading of
 * MessageTemplates.  If a user does not load or create their own custom
 * MessageTemplate, this and BaseContext are the only classes that will be 
 * used.  It's Apply, Instantiate, and Validate functions are the functions
 * called when a user wants to generate or validate a Message from a 
 * MessageTemplate.  It contains a map, matching templateIds to MessageTemplates
 *
 * @sa gmsec::mist::MessageTemplate
 * @sa gmsec::mist::Context
*/
class GMSEC_API BaseSpecification
{
public:

	/** @fn BaseSpecification()
	 * @brief Default constructor - initializes the BaseSpecification instance.
	 * Sets the config member to the config parameter.
	 *
	 * @param config - The configuraton file that Specification uses
	 */
	BaseSpecification();

	/** @fn ~BaseSpecification()
	 * @brief Destructor - cleans up the MessageTemplates
	 */
	~BaseSpecification();

	/** @fn Load(const char* basePath = NULL, SpecificationVersion version)
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
	Status Load(const char* basePath = NULL, unsigned int version = gmsec::mist::ConnectionManager::GMSEC_ISD_CURRENT);

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
	 * @brief Adds the MessageTemplate and id to the map, taking ownership
	 * of newTemplate
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
	 *				 Basically all the string keys to the private member
	 *				 templates, which is a map of all the template IDs
	 *				 to actual MessageTemplate instances in this
	 *				 object.
	 *
	 */
	void GetTemplateIds(gmsec::util::List<gmsec::util::String>& idList);

	/** @fn LookupTemplate(Message* message, gmsec::util::String& idString)
	 * 
	 * @brief Checks if a given message (inputted as parameter message
	 *		by the user) has a matching message template in this 
	 *		BaseSpecification object, and if so gives the user the 
	 *		matching template ID name in output parameter idString.  If
	 *		there is no matching template ID, false is returned
	 *		and the output parameter idString is assigned to be
	 *		the empty string.
	 *
	 * @param message  - (input)
	 *				   GMSEC Message to validate.
	 *
	 * @param idString - (output)
	 *				   Message template ID in this BaseSpecification
	 *				   which matches the given message, if one exists;
	 *				   otherwise the empty string is assigned.
	 *
	 * @return status  - result of operation
	 */
	Status LookupTemplate(Message* message, gmsec::util::String& idString);

private:

	/** @fn parser
	 *		(tinyxml2::XMLElement*             xmlSchema,
	 *		 std::string&                      id,
	 *		 std::string&                      idFieldNames,
	 *		 List<AbstractFieldDescriptor*>&   schemaFields)
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
	 *                        List of MIST AbstractFieldDescriptor instances
	 *                        that will be produced from the xmlSchema
	 *
	 * @return status - result of operation
	 */
	Status parser(tinyxml2::XMLElement*                        xmlSchema,
	              std::string&                                 id,
	              std::string&                                 idFieldNames,
	              gmsec::util::List<AbstractFieldDescriptor*>& schemaFields);

	/** @fn loadDefaultTemplatesFromMemory()
	 *
	 * @brief Fetch default message schemas, from static memory, into
	 *		the private BaseSpecification::templates list
	 *		(declared below).  It is presumed that this function
	 *		will only be called if a schema load from a directory
	 *		location fails.
	 *
	 * @return status - result of operation
	 */
	Status loadDefaultTemplatesFromMemory();

	typedef std::map<std::string, MessageTemplate*> Templates;    //id, template
	typedef std::map<std::string, std::string>      FieldNames;   //id, field name

	Templates  templates;
	FieldNames m_idFieldNames;

	std::string specificationId;

	gmsec::util::List<unsigned char*> m_heapList;

	friend class BaseTester;
};

} //end internal

} //end mist

} //end gmsec

#endif
