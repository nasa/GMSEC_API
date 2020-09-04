/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#ifndef GMSEC_MIST_INTERNAL_FIELD_TEMPLATE_H
#define GMSEC_MIST_INTERNAL_FIELD_TEMPLATE_H

#include <gmsec4/util/wdllexp.h>

#include <string>


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


	FieldTemplate(const char* fieldName, 
				  const char* fieldMode, 
				  const char* fieldClass, 
				  const char* fieldValue, 
				  const char* fieldType, 
				  const char* fieldDescription);


	~FieldTemplate();


	//accessors to member data
	const char* CALL_TYPE getName() const;
	const char* CALL_TYPE getMode() const;
	const char* CALL_TYPE getClass() const;//use with isHeader()
										   //true if m_class is "header"
										   //false if m_class is "standard"
	const char* CALL_TYPE getValue() const;
	const char* CALL_TYPE getType() const;
	const char* CALL_TYPE getDescription() const;
	const char* CALL_TYPE getSize() const;


	//mutators
	void CALL_TYPE setName(const char* name);
	void CALL_TYPE setMode(const char* mode);
	void CALL_TYPE setClass(const char* value);
	void CALL_TYPE setValue(const char* value);
	void CALL_TYPE setType(const char* type);
	void CALL_TYPE setDescription(const char* description);
	void CALL_TYPE setSize(const char* size);



	//included this call anyway for simplicity
	bool CALL_TYPE isHeader() const;
	

	//returns XML-formatted string in the form of a standard field
	const char* CALL_TYPE toXML() const;
	

	//creates a new field based off of the template
	Field* CALL_TYPE toField() const;


protected:
	mutable std::string m_xml;


private:
	std::string m_name;
	std::string m_mode;
	std::string m_class; //corresponds to bool header in standard fields
	std::string m_value;
	std::string m_type;
	std::string m_description;
	std::string m_size; //used only by ARRAY-START control fields
};

}//namespace internal
}//namespace mist
}//namespace api
}//namespace gmsec

#endif
