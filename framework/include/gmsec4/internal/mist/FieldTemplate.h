/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#ifndef GMSEC_MIST_INTERNAL_FIELD_TEMPLATE_H
#define GMSEC_MIST_INTERNAL_FIELD_TEMPLATE_H

#include <gmsec4/util/wdllexp.h>

#include <gmsec4/field/Field.h>

#include <iostream>
#include <string>
#include <list>
#include <vector>


namespace gmsec
{
namespace api
{
	// Forward declaration(s)
	class Field;
	class Message;

namespace mist
{
namespace internal
{
	class FieldTemplateDependency;

class GMSEC_API FieldTemplate
{
public:
	typedef std::vector<FieldTemplate*> FieldTemplateList;
	typedef std::list<FieldTemplateDependency*> FieldTemplateDependencyList;

	FieldTemplate();


	FieldTemplate(const std::string& fieldName, 
				  const std::string& fieldMode, 
				  const std::string& fieldClass, 
				  const std::list<std::string>& fieldValues, 
				  const std::list<std::string>& fieldTypes, 
				  const std::string& fieldDescription);


	FieldTemplate(const FieldTemplate& other);


	~FieldTemplate();


	//accessors to member data
	const std::string& getName() const;
	const std::string& getModifiedName() const;
	const std::string& getMode() const;
	const std::string& getClass() const;//use with isHeader(). true if m_class is "header", false if m_class is "standard"
	const std::list<std::string>& getValues() const;
	const std::string& getPattern() const;
	const std::list<std::string>& getTypeStrings() const;

	std::list<gmsec::api::Field::FieldType> getTypes() const;

	const std::string& getDescription() const;
	const std::string& getSize() const;
	const std::string& getPrefix() const;

	//For use if the type or value is explicitly set, gets the first in the list
	//(an explicitly defined type will only have one item in the list)
	std::string getType() const;
	std::string getValue() const;

	//Returns the lists as concatenated strings
	std::string getConcatenatedTypes() const;
	std::string getConcatenatedValues() const;

	//If an array control, returns the placeholder value for the index number
	std::string getArrayControlValue() const;

	const FieldTemplateList& getChildren() const;
	void getAllChildren(FieldTemplateList& fields, const std::string& prefix = "");
	
	//mutators
	void setName(const std::string& name);
	void setModifiedName(const std::string& name);
	void setMode(const std::string& mode);
	void setClass(const std::string& value);
	void setValues(const std::list<std::string>& values);
	void setPattern(const std::string& pattern);
	void setTypes(const std::list<std::string>& types);
	void setDescription(const std::string& description);
	void setSize(const std::string& size);
	void setChildren(const FieldTemplateList& children);
	void setPrefix(const std::string& prefix);
	void setDependencies(const FieldTemplateDependencyList& dependencies);

	void addDependency(const char* name, const char* valueEquals, const char* valueGreaterThan, const char* valueLessThan, const char* newMode, const char* newPattern, const std::list<std::string>& newValues, const std::list<std::string>& newTypes);

	//used to set an explicit value for the field template
	void setValue(const std::string& value);

	//additional information
	bool hasExplicitType() const;
	bool hasExplicitValue() const;
	bool isTypeVariable() const;
	bool hasChildren() const;
	bool hasDependencies() const;

	//check if any dependencies apply and modify field template
	void checkDependencies(const Message& message);
	void applyDependency(const FieldTemplateDependency* dependency);
	void resetDependencies();

	//included this call anyway for simplicity
	bool isHeader() const;
	

	//returns XML-formatted string in the form of a standard field
	const std::string& toXML(const std::string& type) const;
	

	//creates a new field based off of the template
	//note: caller is responsible for deleting the returned field object
	Field* toField(const std::string& type) const;


	//convenience method to display the info associated with the FieldTemplate
	//usage: std::cout << ftmp << std::endl
	friend std::ostream& operator<<(std::ostream& os, const FieldTemplate& ftmp)
	{
		os << "Name            : " << ftmp.m_name            << "\n"
		   << "Modified Name   : " << ftmp.m_modifiedName    << "\n"
		   << "Mode            : " << ftmp.m_mode            << "\n"
		   << "Modified Mode   : " << ftmp.m_modifiedMode    << "\n"
		   << "Class           : " << ftmp.m_class           << "\n"
		   << "Pattern         : " << ftmp.m_pattern         << "\n"
		   << "Modified Pattern: " << ftmp.m_modifiedPattern << "\n"
		   << "isTypeVariable  : " << (ftmp.isTypeVariable() ? "true" : "false");

		//TODO: Stream other content

		return os;
	}

protected:
	mutable std::string m_xml;


private:
	// Defined, but not implemented
	FieldTemplate& operator=(const FieldTemplate&);


	std::string m_name;
	std::string m_modifiedName;
	std::string m_mode;
	std::string m_modifiedMode;

	std::string m_class; //corresponds to bool header in standard fields

	std::list<std::string> m_values;
	std::list<std::string> m_modifiedValues;

	std::string m_pattern;
	std::string m_modifiedPattern;
	
	std::list<std::string> m_types;
	std::list<std::string> m_modifiedTypes;

	std::string m_description;
	std::string m_size; //used only by ARRAY-START control fields
						//name of the field that indicates the size of the array
	std::string m_prefix; //used only by CONTAINER control fields
						  //the prefix name that gets attached to the front of all child fields

	//the child fields that appear as part of an array of indexed fields
	FieldTemplateList m_children;

	FieldTemplateDependencyList m_dependencies;

};

}//namespace internal
}//namespace mist
}//namespace api
}//namespace gmsec

#endif
