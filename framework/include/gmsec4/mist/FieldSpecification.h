/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/**
 * @file FieldSpecification.h
 *
 * @brief Contains definition for the FieldSpecification class.
 */

#ifndef GMSEC_MIST_FIELD_SPECIFICATION_H
#define GMSEC_MIST_FIELD_SPECIFICATION_H

#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
namespace api
{
namespace mist
{
	namespace internal
	{
		// Forward declaration(s)
		class InternalFieldSpecification;
		class InternalSpecification;
	}

/**
 * @class FieldSpecification
 *
 * @brief The FieldSpecification class represents a single field defintion from a message schema.
 *
 * @sa MessageSpecification
 * @sa Specification
 */
class GMSEC_API FieldSpecification
{
public:
	~FieldSpecification();


	/**
	 * @fn const char* getName() const
	 *
	 * @brief Returns the name for the Field Specification.
	 */
	const char* CALL_TYPE getName() const;


	/**
	 * @fn const char* getType() const
	 *
	 * @brief Returns the type (e.g. STRING, I16, etc.) for the Field Specification.
	 */
	const char* CALL_TYPE getType() const;


	/**
	 * @fn const char* getMode() const
	 *
	 * @brief Returns the mode (e.g. REQUIRED, OPTIONAL, etc.) for the Field Specification.
	 */
	const char* CALL_TYPE getMode() const;


	/**
	 * @fn const char* getClassification() const
	 *
	 * @brief Returns the field class type for the Field Specification.
	 */
	const char* CALL_TYPE getClassification() const;


	/**
	 * @fn const char* getValue() const
	 *
	 * @brief Returns the default value, if any, for the Field Specification. If a
	 * value is not available, then an empty string is returned.
	 */
	const char* CALL_TYPE getValue() const;


	/**
	 * @fn const char* getDescription() const
	 *
	 * @brief Returns the description for the Field Specification.
	 */
	const char* CALL_TYPE getDescription() const;


private:
	friend class gmsec::api::mist::internal::InternalSpecification;


	FieldSpecification(const char* name, const char* type, const char* mode, const char* clazz, const char* value, const char* desc);


	// Defined, but not implemented
	FieldSpecification();
	FieldSpecification(const FieldSpecification&);
	FieldSpecification& operator=(const FieldSpecification&);


	internal::InternalFieldSpecification* m_internal;
};

} // namespace mist
} // namespace api
} // namespace gmsec

#endif
