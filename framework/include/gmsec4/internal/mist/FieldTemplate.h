/*
 * Copyright 2007-2019 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#ifndef GMSEC_MIST_INTERNAL_FIELD_TEMPLATE_H
#define GMSEC_MIST_INTERNAL_FIELD_TEMPLATE_H

#include <gmsec4/util/wdllexp.h>

#include <gmsec4/field/Field.h>

#include <string>
#include <list>


namespace gmsec
{
namespace api
{
	// Forward declaration(s)
	class Field;

namespace mist
{
namespace internal
{

class GMSEC_API FieldTemplate
{
public:
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
	std::string CALL_TYPE getName() const;
	std::string CALL_TYPE getModifiedName() const;
	std::string CALL_TYPE getMode() const;
	std::string CALL_TYPE getClass() const;//use with isHeader()
										   //true if m_class is "header"
										   //false if m_class is "standard"
	std::list<std::string> CALL_TYPE getTypeStrings() const;
	std::list<gmsec::api::Field::FieldType> getTypes() const;
	std::list<std::string> getValues() const;
	std::string CALL_TYPE getDescription() const;
	std::string CALL_TYPE getSize() const;

	//For use if the type or value is explicitly set, gets the first in the list
	//(an explicitly defined type will only have one item in the list)
	std::string getType() const;
	std::string getValue() const;

	//Returns the lists as concatenated strings
	std::string getConcatenatedTypes() const;
	std::string getConcatenatedValues() const;

	//If an array control, returns the placeholder value for the index number
	std::string getArrayControlValue() const;

	//mutators
	void CALL_TYPE setName(const std::string& name);
	void CALL_TYPE setModifiedName(const std::string& name);
	void CALL_TYPE setMode(const std::string& mode);
	void CALL_TYPE setClass(const std::string& value);
	void CALL_TYPE setValues(const std::list<std::string>& values);
	void CALL_TYPE setTypes(const std::list<std::string>& types);
	void CALL_TYPE setDescription(const std::string& description);
	void CALL_TYPE setSize(const std::string& size);

	//used to set an explicit value for the field template
	void setValue(const std::string& value);

	//additional information
	bool hasExplicitType() const;
	bool hasExplicitValue() const;
	bool isTypeVariable() const;

	//included this call anyway for simplicity
	bool CALL_TYPE isHeader() const;
	

	//returns XML-formatted string in the form of a standard field
	std::string CALL_TYPE toXML(const std::string& type) const;
	

	//creates a new field based off of the template
	//note: caller is responsible for deleting the returned field object
	Field* CALL_TYPE toField(const std::string& type) const;


protected:
	mutable std::string m_xml;


private:
	// Defined, but not implemented
	FieldTemplate& operator=(const FieldTemplate&);


	std::string m_name;
	std::string m_modifiedName;
	std::string m_mode;
	std::string m_class; //corresponds to bool header in standard fields

	std::list<std::string> m_values;
	
	
	std::list<std::string> m_types;
	std::string m_description;
	std::string m_size; //used only by ARRAY-START control fields
};

}//namespace internal
}//namespace mist
}//namespace api
}//namespace gmsec

#endif
