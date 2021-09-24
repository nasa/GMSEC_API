/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#ifndef GMSEC_MIST_INTERNAL_SCHEMA_TEMPLATE_H
#define GMSEC_MIST_INTERNAL_SCHEMA_TEMPLATE_H

#include <gmsec4/util/DataList.h>

#include <string>

namespace gmsec
{
namespace api
{
namespace mist
{
namespace internal
{

class GMSEC_API SchemaTemplate
{
public:
	SchemaTemplate();

	~SchemaTemplate();

	const char* CALL_TYPE getID() const;

	const char* CALL_TYPE getDescription() const;

	const char* CALL_TYPE getFirstDefinition();
	const char* CALL_TYPE getNextDefinition();
	bool CALL_TYPE hasNextDefinition() const;

	unsigned int CALL_TYPE getLevel() const;
	const char* CALL_TYPE getLevelName() const;

	void CALL_TYPE setID(const char* id);

	void CALL_TYPE setDescription(const char* description);

	void CALL_TYPE setLevel(unsigned int level);

	void CALL_TYPE setLevelName(const char* name);

	void CALL_TYPE addDefinition(const char* def);

	bool CALL_TYPE isDefinitionEmpty() const;

private:
	std::string m_id;
	std::string m_description;
	std::string m_levelName;
	gmsec::api::util::DataList<std::string> m_definitions;
	unsigned int m_level;
	unsigned int m_definitionIndex;

};

}//namespace internal
}//namespace mist
}//namespace api
}//namespace gmsec

#endif
