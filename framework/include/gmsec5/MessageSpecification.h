/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/**
 * @file MessageSpecification.h
 *
 * @brief Contains definition for the MessageSpecification class.
 */

#ifndef GMSEC_API5_MESSAGE_SPECIFICATION_H
#define GMSEC_API5_MESSAGE_SPECIFICATION_H

#include <gmsec5/util/List.h>
#include <gmsec5/util/wdllexp.h>


namespace gmsec
{
namespace api5
{
	// Forward declaration(s)
	class FieldSpecification;

	namespace internal
	{
		class InternalMessageSpecification;
		class InternalSpecification;
	}

/**
 * @class MessageSpecification
 *
 * @brief The MessageSpecification class represents a message defintion derived
 * from a message schema.
 *
 * @sa FieldSpecification
 * @sa Specification
 */
class GMSEC_API MessageSpecification
{
public:
	~MessageSpecification();


	/**
	 * @fn const char* getSchemaID() const
	 *
	 * @brief Returns the schema (template) ID for this Message Specification.
	 */
	const char* CALL_TYPE getSchemaID() const;


	/**
	 * @fn const char* getSubjectTemplate() const
	 *
	 * @brief Returns the subject template for this Message Specification.
	 */
	const char* CALL_TYPE getSubjectTemplate() const;


	/**
	 * @fn const List<FieldSpecification*>& getFieldSpecifications() const
	 *
	 * @brief Returns the list of FieldSpecification objects for this Message Specification.
	 */
	const util::List<FieldSpecification*>& CALL_TYPE getFieldSpecifications() const;


private:
	friend class gmsec::api5::internal::InternalSpecification;


	MessageSpecification(const char* schemaID, const char* subjectTemplate, util::List<FieldSpecification*>& fieldSpecs);


	// Defined, but not implemented.
	MessageSpecification();
	MessageSpecification(const MessageSpecification&);
	MessageSpecification& operator=(const MessageSpecification&);


	internal::InternalMessageSpecification* m_internal;
};

} // namespace api5
} // namespace gmsec

#endif
