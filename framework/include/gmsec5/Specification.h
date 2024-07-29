/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file Specification.h
 *
 *  @brief This file contains a class for managing MessageTemplates and validating Messages.
 *
 */

#ifndef GMSEC_API5_SPECIFICATION_H
#define GMSEC_API5_SPECIFICATION_H

#include <gmsec5/SchemaIDIterator.h>

#include <gmsec5/util/List.h>
#include <gmsec5/util/wdllexp.h>


namespace gmsec
{
namespace api5
{
	// Forward declaration(s)
	class Config;
	class Message;
	class MessageSpecification;
	class SchemaIDIterator;

	namespace internal
	{
		class MessageTemplate;
		class InternalSpecification;
		class SpecificationBuddy;
	}


/**
 * @class Specification
 *
 * @brief The Specification class loads a list of templates from a designated directory
 * that are used to create and validate message schemas. Each template has an associated ID
 * that is used to identify the kind of message schema the template will be defining.
 *
 * @sa Config
 * @sa SchemaIDIterator
 */
class GMSEC_API Specification
{
public:
	/**
	 * @enum SchemaLevel
	 *
	 * @brief schema Levels for representing message specification.
	 *
	 * @sa Specification::getSchemaLevel()
	 */
	enum class SchemaLevel
	{
		LEVEL_0,    ///< C2MS
		LEVEL_1,    ///< C2MS Extensions, or optionally user-defined
		LEVEL_2,    ///< NASA/GMSEC Addendum, or optionally user-defined
		LEVEL_3,    ///< User-defined
		LEVEL_4,    ///< User-defined
		LEVEL_5,    ///< User-defined
		LEVEL_6     ///< User-defined
	};


	/**
	 * @fn Specification()
	 *
	 * @brief Constructor - initializes the Specification instance using the default templates.
	 *
	 * @throw GmsecException is thrown if the Specification instance cannot be created.
	 */
	Specification();


	/**
	 * @fn Specification(const Config& config)
	 *
	 * @brief Constructor - initializes the Specification instance with a configuration.
	 *
	 * @param config - The configuraton file that Specification uses
	 *
	 * @throw GmsecException is thrown if it cannot deduce the values passed to it by the configuration
	 */
	Specification(const Config& config);


	/**
	 * @fn Specification(const Specification& other)
	 *
	 * @brief Copy constructor - initializes the Specification instance based on another instance
	 *
	 * @param other - The Specification object to be copied
	 */
	Specification(const Specification& other);


	/**
	 * @fn ~Specification()
	 *
	 * @brief Destructor - deletes InternalSpecification pointer
	 */
	virtual ~Specification();


	/**
	 * @fn getSchemaIDIterator()
	 * 
	 * @brief returns a SchemaIDIterator which can be used to iterate over the IDs
	 * of the loaded templates.
	 *
	 * @note Only one SchemaIDIterator object is associated with a %Specification object; multiple calls to 
	 * getSchemaIDIterator() will return a reference to the same SchemaIDIterator object.  Each call will reset the iterator.
	 *
	 * @return a reference to a SchemaIDIterator object
	 */
	SchemaIDIterator& CALL_TYPE getSchemaIDIterator() const;


	/**
	 * @fn getVersion()
	 *
	 * @brief Returns the version of the templates that have been loaded.
	 */
	unsigned int CALL_TYPE getVersion() const;


	/**
	 * @fn SchemaLevel::Level getSchemaLevel() const;
	 *
	 * @brief Returns the operating schema level for the specification.
	 */
	SchemaLevel CALL_TYPE getSchemaLevel() const;


	/**
	 * @fn const util::List<MessageSpecification*>& getMessageSpecifications() const
	 *
	 * @brief Returns the list of MessageSpecification objects, each of which represents a message template.
	 */
	const util::List<MessageSpecification*>& CALL_TYPE getMessageSpecifications() const;


	/**
	 * @fn const util::List<const char*> CALL_TYPE getHeaderFieldNames(const char* schemaName) const
	 *
	 * @brief Returns a list of the field names for the specified header 
	 * 
	 * @param schemaName - The name of the specified schema level (e.g. C2MS, C2MS-PIPE, GMSEC, DEFAULT).  An empty or NULL string refers to DEFAULT.
	 *
	 * @return a list of strings that correspond to the names of the header fields.
	 */
	const util::List<const char*> CALL_TYPE getHeaderFieldNames(const char* schemaName = "DEFAULT") const;

private:
	friend class gmsec::api5::internal::SpecificationBuddy;

	// Defined, but not implemented
	Specification& operator=(const Specification&);

	internal::InternalSpecification* m_internal;
};

} // namespace api5
} // namespace gmsec

#endif
