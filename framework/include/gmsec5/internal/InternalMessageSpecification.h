/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#ifndef GMSEC_API5_INTERNAL_MESSAGE_SPECIFICATION_H
#define GMSEC_API5_INTERNAL_MESSAGE_SPECIFICATION_H

#include <gmsec5/util/List.h>

#include <string>


namespace gmsec
{
namespace api5
{
	// Forward declaration(s)
	class FieldSpecification;

namespace internal
{

class InternalMessageSpecification
{
public:
	InternalMessageSpecification(const std::string& schemaID,
	                             const std::string& subjectTemplate,
	                             util::List<FieldSpecification*>& fieldSpecs);

	~InternalMessageSpecification();

	const char* getSchemaID() const;

	const char* getSubjectTemplate() const;

	const util::List<FieldSpecification*>& getFieldSpecifications() const;

private:
	std::string                     m_schemaID;
	std::string                     m_subjectTemplate;
	util::List<FieldSpecification*> m_fieldSpecs;
};

} // namespace internal
} // namespace api5
} // namespace gmsec

#endif
