/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#ifndef GMSEC_API5_SCHEMA_TEMPLATE_H
#define GMSEC_API5_SCHEMA_TEMPLATE_H

#include <gmsec5/util/List.h>
#include <gmsec5/util/wdllexp.h>

#include <string>
#include <list>

namespace gmsec
{
namespace api5
{
namespace internal
{

class GMSEC_API SchemaTemplate
{
public:
	SchemaTemplate();

	SchemaTemplate(const SchemaTemplate& other);

	~SchemaTemplate();

	const char* CALL_TYPE getID() const;

	const char* CALL_TYPE getDescription() const;

	const std::list<std::string>& CALL_TYPE getDefinitions() const;
	std::string getDefinitionString() const;

	const char* CALL_TYPE getFirstDefinition();
	const char* CALL_TYPE getNextDefinition();
	bool CALL_TYPE hasNextDefinition() const;

	int CALL_TYPE getLevel() const;
	const char* CALL_TYPE getLevelName() const;

	void CALL_TYPE setID(const char* id);

	void CALL_TYPE setDescription(const char* description);

	void CALL_TYPE setLevel(int level);

	void CALL_TYPE setLevelName(const char* name);

	void CALL_TYPE addDefinition(const char* def);

	bool CALL_TYPE isDefinitionEmpty() const;

private:
	std::string m_id;
	std::string m_description;
	std::string m_levelName;
	std::list<std::string> m_definitions;
	int m_level;
	std::list<std::string>::const_iterator m_index;

};

}  //end namespace internal
}  //end namespace api5
}  //end namespace gmsec

#endif
