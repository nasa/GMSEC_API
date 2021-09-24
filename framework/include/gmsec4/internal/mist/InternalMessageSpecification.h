/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#ifndef GMSEC_MIST_INTERNAL_MESSAGE_SPECIFICATION_H
#define GMSEC_MIST_INTERNAL_MESSAGE_SPECIFICATION_H

#include <gmsec4/util/DataList.h>

#include <string>


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

class InternalMessageSpecification
{
public:
	InternalMessageSpecification(const char* schemaID, util::DataList<FieldSpecification*>& fieldSpecs);

	~InternalMessageSpecification();

	const char* getSchemaID() const;

	const util::DataList<FieldSpecification*>& getFieldSpecifications() const;

private:
	std::string                         m_schemaID;
	util::DataList<FieldSpecification*> m_fieldSpecs;
};

} // namespace internal
} // namespace mist
} // namespace api
} // namespace gmsec

#endif
