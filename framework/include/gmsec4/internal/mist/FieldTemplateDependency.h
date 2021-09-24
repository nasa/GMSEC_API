/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#ifndef GMSEC_MIST_INTERNAL_FIELD_TEMPLATE_DEPENDENCY_H
#define GMSEC_MIST_INTERNAL_FIELD_TEMPLATE_DEPENDENCY_H

#include <gmsec4/util/wdllexp.h>

#include <gmsec4_defs.h>

#include <string>
#include <list>


namespace gmsec
{
namespace api
{

namespace mist
{
namespace internal
{

class GMSEC_API FieldTemplateDependency
{
public:

	FieldTemplateDependency();

	FieldTemplateDependency(const char* name,
							const char* valueEquals,
							const char* valueGreaterThan,
							const char* valueLessThan,
							const char* newMode,
							const char* newPattern,
							const std::list<std::string>& newValues,
							const std::list<std::string>& newTypes);

	FieldTemplateDependency(const FieldTemplateDependency& other);


	~FieldTemplateDependency();

	const std::string& getName() const;
	const std::string& getNewMode() const;
	const std::string& getNewPattern() const;
	const std::list<std::string>& getNewValues() const;
	const std::list<std::string>& getNewTypes() const;
	
	void setName(const std::string& name);
	void setNewMode(const std::string& newMode);
	void setNewPattern(const std::string& newPattern);
	void setNewValues(const std::list<std::string>& newValues);
	void setNewTypes(const std::list<std::string>& newTypes);

	void setConditions(const std::string& valueEquals, const std::string& valueGreaterThan, const std::string& valueLessThan);
	bool checkConditions(const std::string& value) const;
	bool checkConditions(GMSEC_I64 value) const;
	bool checkConditions(GMSEC_U64 value) const;
	bool checkConditions(GMSEC_F64 value) const;
	bool checkConditions(bool value) const;

protected:


private:
	std::string m_name; //name of the dependent field to be evaluated

	//value the dependent field should be checked against to see if condition is met
	//values will be empty if not used
	std::string m_valueEquals;
	std::string m_valueGreaterThan;
	std::string m_valueLessThan;


	std::string m_newMode; //modified mode (REQUIRED/OPTIONAL/RESTRICTED)to be used if conditions are met, will be empty if not used

	std::string m_newPattern;
	
	std::list<std::string> m_newValues; //modifed list of valid values to be used if conditions are met, will be empty if not used
	
	std::list<std::string> m_newTypes; //modified list of types to be used if conditions are met, will be empty of not used


};

}//namespace internal
}//namespace mist
}//namespace api
}//namespace gmsec

#endif
