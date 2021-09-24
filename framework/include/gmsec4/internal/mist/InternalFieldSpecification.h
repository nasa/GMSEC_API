/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#ifndef GMSEC_MIST_INTERNAL_FIELD_SPECIFICATION_H
#define GMSEC_MIST_INTERNAL_FIELD_SPECIFICATION_H

#include <string>


namespace gmsec
{
namespace api
{
namespace mist
{
namespace internal
{

class InternalFieldSpecification
{
public:
	InternalFieldSpecification(const std::string& name, const std::string& type, const std::string& mode, const std::string& clazz, const std::string& value, const std::string& desc);

	const char* getName() const;

	const char* getType() const;

	const char* getMode() const;

	const char* getClassification() const;

	const char* getValue() const;

	const char* getDescription() const;

private:
	std::string m_name;
	std::string m_type;
	std::string m_mode;
	std::string m_class;
	std::string m_value;
	std::string m_desc;
};

} // namespace internal
} // namespace mist
} // namespace api
} // namespace gmsec

#endif
