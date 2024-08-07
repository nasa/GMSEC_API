/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#ifndef GMSEC_API5_INTERNAL_FIELD_TEMPLATE_H
#define GMSEC_API5_INTERNAL_FIELD_TEMPLATE_H

#include <gmsec5/field/Field.h>

#include <gmsec5/util/wdllexp.h>

#include <iostream>
#include <string>
#include <list>
#include <vector>


namespace gmsec
{
	namespace api5
	{
		// Forward declaration(s)
		class Field;

		namespace internal
		{
			class FieldTemplateDependency;
			class InternalMessage;

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
				const std::string& getModifiedName() const; //this member data will be filled in if it's a child of a field array
				const std::string& getMode() const;
				const std::string& getClass() const;//use with isHeader(). true if m_class is "header", false if m_class is "standard"
				const std::list<std::string>& getValues() const;
				const std::string& getPattern() const;

				std::list<gmsec::api5::Field::Type> getTypes() const;

				const std::string& getDescription() const;
				const std::string& getSize() const;
				const std::string& getPrefix() const;

				//For use if the type is explicitly set, gets the first in the list
				//(an explicitly defined type will only have one item in the list)
				std::string getType() const; 

				//Returns the lists as concatenated strings
				std::string getConcatenatedTypes() const;
				std::string getConcatenatedValues() const;

				//If an array control, returns the placeholder value for the index number
				std::string getArrayControlValue() const;

				const FieldTemplateList& getChildren() const;

				//recursively get children
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

				//for use by Specification's parser when reading in dependencies from file
				void initDependency(
					const char* name,
					const char* valueEquals,
					const char* valueGreaterThan,
					const char* valueLessThan,
					const char* newMode,
					const char* newPattern,
					const std::list<std::string>& newValues,
					const std::list<std::string>& newTypes);

				//for use by Specification when prepping headers for validation
				//used to set an explicit value for the field template
				void setValue(const std::string& value);

				//explicitly defined means having only one valid type/value
				bool hasExplicitType() const; 
				bool hasExplicitValue() const;

				bool isTypeVariable() const;
				bool hasChildren() const;

				//check if any dependencies apply and modify field template
				//if checkDependencies is called for a field instance to be validated, 
				//it MUST be followed by resetCheckDependencies() 
				//to not contaminate the next field instance to be validated
				void checkDependencies(const InternalMessage& message);
				void resetCheckedDependencies();

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
					os << "Name            : " << ftmp.m_name << "\n"
						<< "Modified Name   : " << ftmp.m_modifiedName << "\n"
						<< "Mode            : " << ftmp.m_mode << "\n"
						<< "Modified Mode   : " << ftmp.m_modifiedMode << "\n"
						<< "Class           : " << ftmp.m_class << "\n"
						<< "Pattern         : " << ftmp.m_pattern << "\n"
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
				std::string m_modifiedName; //this member data will be filled in if it's a child of a field array
				std::string m_mode;
				std::string m_modifiedMode; //this member data will be filled if a dependency was applied

				std::string m_class; //corresponds to bool header in standard fields

				std::list<std::string> m_values;
				std::list<std::string> m_modifiedValues;//this member data will be filled if a dependency was applied

				std::string m_pattern;
				std::string m_modifiedPattern;//this member data will be filled if a dependency was applied

				std::list<std::string> m_types;
				std::list<std::string> m_modifiedTypes;//this member data will be filled if a dependency was applied

				std::string m_description;
				std::string m_size; //used only by ARRAY-START control fields
									//name of the field that indicates the size of the array
				std::string m_prefix; //used only by CONTAINER control fields
									  //the prefix name that gets attached to the front of all child fields

				//the child fields that appear as part of an array of indexed fields
				FieldTemplateList m_children;

				FieldTemplateDependencyList m_dependencies;


				//For use if the value is explicitly set, gets the first in the list
				//(an explicitly defined value will only have one item in the list)
				std::string getValue() const;

				//helper method to checkDependency()
				//If the function finds an applicable dependency, this call applies it to the FieldTemplate
				void applyDependency(const FieldTemplateDependency* dependency);
			};

		} //end namespace internal
	} //end namespace api5
} //end namespace gmsec

#endif
