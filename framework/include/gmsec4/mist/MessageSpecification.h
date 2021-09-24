/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/**
 * @file MessageSpecification.h
 *
 * @brief Contains definition for the MessageSpecification class.
 */

#ifndef GMSEC_MIST_MESSAGE_SPECIFICATION_H
#define GMSEC_MIST_MESSAGE_SPECIFICATION_H

#include <gmsec4/util/DataList.h>
#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
namespace api
{
namespace mist
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
	 * @fn const DataList<FieldSpecification*>& getFieldSpecifications() const
	 *
	 * @brief Returns the list of FieldSpecification objects for this Message Specification.
	 */
	const util::DataList<FieldSpecification*>& CALL_TYPE getFieldSpecifications() const;


private:
	friend class gmsec::api::mist::internal::InternalSpecification;


	MessageSpecification(const char* schemaID, util::DataList<FieldSpecification*>& fieldSpecs);


	// Defined, but not implemented.
	MessageSpecification();
	MessageSpecification(const MessageSpecification&);
	MessageSpecification& operator=(const MessageSpecification&);


	internal::InternalMessageSpecification* m_internal;
};

} // namespace mist
} // namespace api
} // namespace gmsec

#endif
