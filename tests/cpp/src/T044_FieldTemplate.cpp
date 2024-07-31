#include "TestCase.h"

#include <gmsec5/internal/FieldTemplate.h>
#include <gmsec5/internal/FieldTemplateDependency.h>
#include <gmsec5/internal/MessageBuddy.h>

#include <gmsec5/util/StdUniquePtr.h>

#include <list>
#include <string>
#include <vector>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::util;


void test_constructor_1(Test& test)
{
	GMSEC_INFO << "Test constructor 1";

	FieldTemplate tmp;

	test.check("Unexpected name", tmp.getName() == "");
	test.check("Unexpected modified name", tmp.getModifiedName() == "");
	test.check("Unexpected mode", tmp.getMode() == "");
	test.check("Unexpected class", tmp.getClass() == "");
	test.check("Unexpected field values", tmp.getValues().size() == 0);
	test.check("Unexpected pattern ", tmp.getPattern() == "");
	test.check("Unexpected types", tmp.getTypes().size() == 0);
	test.check("Unexpected description", tmp.getDescription() == "");
	test.check("Unexpected size", tmp.getSize() == "");
	test.check("Unexpected prefix", tmp.getPrefix() == "");
	test.check("Unexpected type", tmp.getType() == "");
	test.check("Unexpected concatenated types", tmp.getConcatenatedTypes() == "");
	test.check("Unexpected concatenated values", tmp.getConcatenatedValues() == "");
	test.check("Unexpected array control value", tmp.getArrayControlValue() == "");
	test.check("Unexpected explicit type", tmp.hasExplicitType() == false);
	test.check("Unexpected explicit value", tmp.hasExplicitValue() == false);
	test.check("Unexpected type variable", tmp.isTypeVariable() == false);
	test.check("Unexpected children", tmp.hasChildren() == false);
	test.check("Unexpected header", tmp.isHeader() == false);

	std::ostringstream oss;
	oss << tmp;
	test.check("Expected stream content", oss.str().size() > 0);
}


void test_constructor_2(Test& test)
{
	GMSEC_INFO << "Test constructor 2";

	std::list<std::string> fieldValues;
	std::list<std::string> fieldTypes;

	fieldValues.push_back("true");
	fieldValues.push_back("false");

	fieldTypes.push_back("BOOL");
	fieldTypes.push_back("STRING");

	FieldTemplate tmp("FIELD-NAME", "FIELD-MODE", "FIELD-CLASS", fieldValues, fieldTypes, "FIELD-DESCRIPTION");

	test.check("Unexpected name", tmp.getName() == "FIELD-NAME");
	test.check("Unexpected modified name", tmp.getModifiedName() == "FIELD-NAME");
	test.check("Unexpected mode", tmp.getMode() == "FIELD-MODE");
	test.check("Unexpected class", tmp.getClass() == "FIELD-CLASS");
	test.check("Unexpected field values", tmp.getValues().size() == 2);
	test.check("Unexpected pattern ", tmp.getPattern() == "");
	test.check("Unexpected types", tmp.getTypes().size() == 2);
	test.check("Unexpected description", tmp.getDescription() == "FIELD-DESCRIPTION");
	test.check("Unexpected size", tmp.getSize() == "");
	test.check("Unexpected prefix", tmp.getPrefix() == "");
	test.check("Unexpected type", tmp.getType() == "");
	test.check("Unexpected concatenated types", tmp.getConcatenatedTypes() == "BOOL, STRING");
	test.check("Unexpected concatenated values", tmp.getConcatenatedValues() == "true, false");
	test.check("Unexpected array control value", tmp.getArrayControlValue() == "");
	test.check("Unexpected explicit type", tmp.hasExplicitType() == false);
	test.check("Unexpected explicit value", tmp.hasExplicitValue() == false);
	test.check("Unexpected type variable", tmp.isTypeVariable() == false);
	test.check("Unexpected children", tmp.hasChildren() == false);
	test.check("Unexpected header", tmp.isHeader() == false);

	std::ostringstream oss;
	oss << tmp;
	test.check("Expected stream content", oss.str().size() > 0);
}


void test_copy_constructor(Test& test)
{
	GMSEC_INFO << "Test copy constructor";

	std::list<std::string> fieldValues;
	std::list<std::string> fieldTypes;

	fieldValues.push_back("true");
	fieldValues.push_back("false");

	fieldTypes.push_back("BOOL");
	fieldTypes.push_back("STRING");

	FieldTemplate tmp1("FIELD-NAME", "FIELD-MODE", "FIELD-CLASS", fieldValues, fieldTypes, "FIELD-DESCRIPTION");
	FieldTemplate tmp2 = tmp1;

	std::ostringstream oss1, oss2;
	oss1 << tmp1;
	oss2 << tmp2;

	test.check("Unexpected ostream output", oss1.str() == oss2.str());
	test.check("Unexected BOOL field", tmp1.toXML("BOOL") == tmp2.toXML("BOOL"));
	test.check("Unexected STRING field", tmp1.toXML("STRING") == tmp2.toXML("STRING"));
}


void test_set_get_name(Test& test)
{
	GMSEC_INFO << "Test setName() and getName()";

	FieldTemplate tmp;
	tmp.setName("foobar");
	test.check("Unexpected name", tmp.getName() == "foobar");
}


void test_set_get_modified_name(Test& test)
{
	GMSEC_INFO << "Test setModifiedName() and getModifiedName()";

	FieldTemplate tmp;
	tmp.setModifiedName("modified");
	test.check("Unexpected modified name", tmp.getModifiedName() == "modified");
}


void test_set_get_mode(Test& test)
{
	GMSEC_INFO << "Test setMode() and getMode()";

	FieldTemplate tmp;
	tmp.setMode("mode");
	test.check("Unexpected mode", tmp.getMode() == "mode");
}


void test_set_get_class(Test& test)
{
	GMSEC_INFO << "Test setClass() and getClass()";

	FieldTemplate tmp;
	tmp.setClass("class");
	test.check("Unexpected class", tmp.getClass() == "class");
}


void test_set_get_values(Test& test)
{
	GMSEC_INFO << "Test setValues() and getValues()";

	std::list<std::string> values;
	values.push_back("one");
	values.push_back("two");

	FieldTemplate tmp;
	tmp.setValues(values);

	values = tmp.getValues();

	for (std::list<std::string>::const_iterator it = values.begin(); it != values.end(); ++it)
	{
		if (it == values.begin()) {
			test.check("Unexpected value", *it == "one");
		}
		else {
			test.check("Unexpected value", *it == "two");
		}
	}
}


void test_set_get_pattern(Test& test)
{
	GMSEC_INFO << "Test setPattern() and getPattern()";

	FieldTemplate tmp;
	tmp.setPattern("pattern");
	test.check("Unexpected pattern", tmp.getPattern() == "pattern");
}


void test_set_get_types(Test& test)
{
	GMSEC_INFO << "Test setTypes() and getTypes()";

	std::list<std::string> types;
	types.push_back("I32");
	types.push_back("STRING");

	FieldTemplate tmp;
	tmp.setTypes(types);

	std::list<Field::Type> fieldTypes = tmp.getTypes();

	for (std::list<Field::Type>::const_iterator it = fieldTypes.begin(); it != fieldTypes.end(); ++it)
	{
		if (it == fieldTypes.begin()) {
			test.check("Unexpected type", *it == Field::Type::I32);
		}
		else {
			test.check("Unexpected type", *it == Field::Type::STRING);
		}
	}

	test.check("Expected explicit type", tmp.hasExplicitType() == false);
}


void test_set_get_description(Test& test)
{
	GMSEC_INFO << "Test setDescription() and getDescription()";

	FieldTemplate tmp;
	tmp.setDescription("description");
	test.check("Unexpected description", tmp.getDescription() == "description");
}


void test_set_get_size(Test& test)
{
	GMSEC_INFO << "Test setSize() and getSize()";

	FieldTemplate tmp;
	tmp.setSize("size");
	test.check("Unexpected size", tmp.getSize() == "size");
}


void test_set_get_children(Test& test)
{
	GMSEC_INFO << "Test setChildren() and getChildren()";

	FieldTemplate tmp1, tmp2;
	tmp1.setName("One");
	tmp2.setName("Two");

	FieldTemplate::FieldTemplateList tmpList;
	tmpList.push_back(&tmp1);
	tmpList.push_back(&tmp2);

	FieldTemplate tmp;
	tmp.setChildren(tmpList);

	tmpList = tmp.getChildren();
	test.check("Unexpected size", tmpList.size() == 2);
	for (FieldTemplate::FieldTemplateList::const_iterator it = tmpList.begin(); it != tmpList.end(); ++it)
	{
		if (it == tmpList.begin()) {
			test.check("Unexpected field template name", (*it)->getName() == "One");
		}
		else {
			test.check("Unexpected field template name", (*it)->getName() == "Two");
		}
	}
}


void test_set_get_prefix(Test& test)
{
	GMSEC_INFO << "Test setPrefix() and getPrefix()";

	FieldTemplate tmp;
	tmp.setPrefix("prefix");
	test.check("Unexpected prefix", tmp.getPrefix() == "prefix");
}


void test_set_get_value(Test& test)
{
	GMSEC_INFO << "Test setValue() and getValue()";

	std::list<std::string> types;
	types.push_back("I32");

	FieldTemplate tmp;
	tmp.setTypes(types);
	tmp.setValue("10");
	test.check("Expected explicit type", tmp.hasExplicitType());
	test.check("Expected explicit value", tmp.hasExplicitValue());
}


void test_is_type_variable(Test& test)
{
	GMSEC_INFO << "Test isTypeVariable()";

	std::list<std::string> types;
	types.push_back("VARIABLE");

	FieldTemplate tmp;
	tmp.setTypes(types);
	test.check("Unexpected type", tmp.isTypeVariable());

	types.clear();
	types.push_back("STRING");

	tmp.setTypes(types);
	test.check("Unexpected type", tmp.isTypeVariable() == false);
}


void test_has_children(Test& test)
{
	GMSEC_INFO << "Test hasChildren()";

	FieldTemplate tmp;
	test.check("Unexpected children", tmp.hasChildren() == false);

	FieldTemplate tmp1, tmp2;
	tmp1.setName("One");
	tmp2.setName("Two");

	FieldTemplate::FieldTemplateList tmpList;
	tmpList.push_back(&tmp1);
	tmpList.push_back(&tmp2);

	tmp.setChildren(tmpList);
	test.check("Expected to have children", tmp.hasChildren());
}


void test_init_and_check_dependencies(Test& test)
{
	GMSEC_INFO << "Test checkDependencies()";

	std::list<std::string> values{ "1" };
	std::list<std::string> types{ "I16" };

	std::list<std::string> newValues{ "changed" };
	std::list<std::string> newTypes{ "STRING" };

	FieldTemplate tmp;
	tmp.setMode("OPTIONAL");
	tmp.setValues(values);
	tmp.setTypes(types);
	tmp.initDependency("DEPENDENT-FIELD", NULL, NULL, NULL, "REQUIRED", NULL, newValues, newTypes);

	Config config("gmsec-schema-level=2", DataType::KEY_VALUE_DATA);

	MessageFactory msgFactory(config);

	Message msg = msgFactory.createMessage();

	msg.addField("DEPENDENT-FIELD", "true");

	const InternalMessage& intMsg = MessageBuddy::getInternal(msg);

	tmp.checkDependencies(intMsg);

	test.check("checkDependencies failed to modify mode", tmp.getMode() == "REQUIRED");
	test.check("checkDependencies failed to modify mode", tmp.getValues().front() == "changed");
	test.check("checkDependencies failed to modify mode", tmp.getTypes().front() == Field::Type::STRING);

	tmp.resetCheckedDependencies();

	test.check("resetCheckedDependencies failed to revert mode", tmp.getMode() == "OPTIONAL");
	test.check("resetCheckedDependencies failed to revert mode", tmp.getValues().front() == "1");
	test.check("resetCheckedDependencies failed to revert mode", tmp.getTypes().front() == Field::Type::I16);
}


void test_is_header(Test& test)
{
	GMSEC_INFO << "Test isHeader()";

	FieldTemplate tmp;
	test.check("Unexpected header designation", tmp.isHeader() == false);

	tmp.setClass("HEADER");
	test.check("Expected header designation", tmp.isHeader());
}


void test_to_xml(Test& test)
{
	GMSEC_INFO << "Test toXML()";

	std::list<std::string> fieldValues;
	std::list<std::string> fieldTypes;

	fieldValues.push_back("true");
	fieldTypes.push_back("BOOL");

	FieldTemplate tmp("FIELD-NAME", "FIELD-MODE", "FIELD-CLASS", fieldValues, fieldTypes, "FIELD-DESCRIPTION");

	test.check("Unexpected XML content", tmp.toXML("BOOL") == "<FIELD TYPE=\"BOOL\" NAME=\"FIELD-NAME\">true</FIELD>");
}


void test_to_field(Test& test)
{
	GMSEC_INFO << "Test toField()";

	std::list<std::string> fieldValues;
	std::list<std::string> fieldTypes;

	fieldValues.push_back("true");
	fieldTypes.push_back("BOOL");

	FieldTemplate tmp("FIELD-NAME", "FIELD-MODE", "FIELD-CLASS", fieldValues, fieldTypes, "FIELD-DESCRIPTION");

	try
	{
		StdUniquePtr<Field> field( tmp.toField("BOOL") );

		test.check("Unexpected XML content", StringUtil::stringEquals(field->toXML(), "<FIELD NAME=\"FIELD-NAME\" TYPE=\"BOOL\">TRUE</FIELD>"));
	}
	catch (const GmsecException& e)
	{
		GMSEC_ERROR << e.what();
	}
}


int test_FieldTemplate(Test& test)
{
	test.setDescription("FieldTemplate");

	test_constructor_1(test);
	test_constructor_2(test);
	test_copy_constructor(test);
	test_set_get_name(test);
	test_set_get_modified_name(test);
	test_set_get_mode(test);
	test_set_get_class(test);
	test_set_get_values(test);
	test_set_get_pattern(test);
	test_set_get_types(test);
	test_set_get_description(test);
	test_set_get_size(test);
	test_set_get_children(test);
	test_set_get_prefix(test);
	test_set_get_value(test);
	test_is_type_variable(test);
	test_has_children(test);
	test_init_and_check_dependencies(test);
	test_is_header(test);
	test_to_xml(test);
	test_to_field(test);

	return 0;
}

TEST_DRIVER(test_FieldTemplate)
