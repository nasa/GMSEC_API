#include "TestCase.h"

#include <gmsec5/internal/MessageTemplate.h>

#include <gmsec5/util/StdUniquePtr.h>

#include <string>
#include <vector>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::util;


static MessageTemplate* msgTemplateBuilder(bool buildAlt = false)
{
	std::list<std::string> fieldValues;
	std::list<std::string> fieldTypes;

	fieldValues.push_back("true");
	fieldValues.push_back("false");

	fieldTypes.push_back("BOOL");
	fieldTypes.push_back("STRING");

	const char* fieldName1  = (buildAlt ? "FIELD-NAME-1-ALT" : "FIELD-NAME-1");
	const char* fieldName2  = (buildAlt ? "FIELD-NAME-2-ALT" : "FIELD-NAME-2");

	const char* fieldMode1  = (buildAlt ? "FIELD-MODE-1-ALT" : "FIELD-MODE-1");
	const char* fieldMode2  = (buildAlt ? "FIELD-MODE-2-ALT" : "FIELD-MODE-2");

	const char* fieldClass1 = (buildAlt ? "FIELD-CLASS-1-ALT" : "FIELD-CLASS-1");
	const char* fieldClass2 = (buildAlt ? "FIELD-CLASS-2-ALT" : "FIELD-CLASS-2");

	const char* fieldDesc1  = (buildAlt ? "FIELD-DESC-1-ALT" : "FIELD-DESC-1");
	const char* fieldDesc2  = (buildAlt ? "FIELD-DESC-2-ALT" : "FIELD-DESC-2");

	FieldTemplate tmp1(fieldName1, fieldMode1, fieldClass1, fieldValues, fieldTypes, fieldDesc1);
	FieldTemplate tmp2(fieldName2, fieldMode2, fieldClass2, fieldValues, fieldTypes, fieldDesc2);

	MessageTemplate::FieldTemplateList tmpList;
	tmpList.push_back(&tmp1);
	tmpList.push_back(&tmp2);

	MessageTemplate* tmp = new MessageTemplate("SCHEMA-ID", tmpList, 0);

	return tmp;
}


void test_constructor(Test& test)
{
	GMSEC_INFO << "Test constructor";

	StdUniquePtr<MessageTemplate> tmp( msgTemplateBuilder() );

	test.check("Unexpected schema ID", StringUtil::stringEquals(tmp->getSchemaID(), "SCHEMA-ID"));
	test.check("Unexpected schema level", tmp->getSchemaLevel() == 0);
	test.check("Unexpected field tmp size", tmp->getFieldTemplates().size() == 2);
}


void test_copy_constructor(Test& test)
{
	GMSEC_INFO << "Test copy-constructor";

	StdUniquePtr<MessageTemplate> tmp1( msgTemplateBuilder() );

	MessageTemplate tmp2( *(tmp1.get()) );

	test.check("Unexpected schema ID", StringUtil::stringEquals(tmp1->getSchemaID(), tmp2.getSchemaID()));
	test.check("Unexpected schema level", tmp1->getSchemaLevel() == tmp2.getSchemaLevel());
	test.check("Unexpected field tmp size", tmp1->getFieldTemplates().size() == tmp2.getFieldTemplates().size());
}


void test_assignment_operator(Test& test)
{
	GMSEC_INFO << "Test assignment operator";

	StdUniquePtr<MessageTemplate> tmp1( msgTemplateBuilder() );
	StdUniquePtr<MessageTemplate> tmp2( msgTemplateBuilder(true) );

	*(tmp1.get()) = *(tmp2.get());

	const MessageTemplate::FieldTemplateList& tmpList = tmp1->getFieldTemplates();

	test.check("Unexpected number of field templates", tmpList.size() == 2);

	for (MessageTemplate::FieldTemplateList::const_iterator it = tmpList.begin(); it != tmpList.end(); ++it)
	{
		if (it == tmpList.begin()) {
			test.check("Unexpected fld tmp name", (*it)->getName() == "FIELD-NAME-1-ALT");
			test.check("Unexpected fld tmp mode", (*it)->getMode() == "FIELD-MODE-1-ALT");
			test.check("Unexpected fld tmp class", (*it)->getClass() == "FIELD-CLASS-1-ALT");
			test.check("Unexpected fld tmp desc", (*it)->getDescription() == "FIELD-DESC-1-ALT");
		}
		else {
			test.check("Unexpected fld tmp name", (*it)->getName() == "FIELD-NAME-2-ALT");
			test.check("Unexpected fld tmp mode", (*it)->getMode() == "FIELD-MODE-2-ALT");
			test.check("Unexpected fld tmp class", (*it)->getClass() == "FIELD-CLASS-2-ALT");
			test.check("Unexpected fld tmp desc", (*it)->getDescription() == "FIELD-DESC-2-ALT");
		}
	}
}


void test_get_field(Test& test)
{
	GMSEC_INFO << "Test getField()";

	StdUniquePtr<MessageTemplate> tmp( msgTemplateBuilder() );

	StdUniquePtr<Field> field1( tmp->getField("FIELD-NAME-1", "BOOL") );
	StdUniquePtr<Field> field2( tmp->getField("FIELD-NAME-2", "STRING") );
	StdUniquePtr<Field> field3( tmp->getField("BOGUS-FIELD", "STRING") );

	test.require("Expected a field", field1.get() != NULL);
	test.check("Unexpected field name", StringUtil::stringEquals(field1->getName(), "FIELD-NAME-1"));
	test.check("Unexpected field type", field1->getType() == Field::Type::BOOL);

	test.require("Expected a field", field2.get() != NULL);
	test.check("Unexpected field name", StringUtil::stringEquals(field2->getName(), "FIELD-NAME-2"));
	test.check("Unexpected field type", field2->getType() == Field::Type::STRING);

	test.check("Unexpected field", field3.get() == NULL);
}


void test_get_field_template(Test& test)
{
	GMSEC_INFO << "Test getFieldTemplate()";

	StdUniquePtr<MessageTemplate> tmp( msgTemplateBuilder() );

	//o Known field templates
	try {
		const FieldTemplate& fldTmp = tmp->getFieldTemplate("FIELD-NAME-1");
		test.check("Unexpected fld tmp name", fldTmp.getName() == "FIELD-NAME-1");
		test.check("Unexpected fld tmp mode", fldTmp.getMode() == "FIELD-MODE-1");
		test.check("Unexpected fld tmp class", fldTmp.getClass() == "FIELD-CLASS-1");
		test.check("Unexpected fld tmp desc", fldTmp.getDescription() == "FIELD-DESC-1");
	}
	catch (const GmsecException& e) {
		test.check(e.what(), false);
	}

	try {
		const FieldTemplate& fldTmp = tmp->getFieldTemplate("FIELD-NAME-2");
		test.check("Unexpected fld tmp name", fldTmp.getName() == "FIELD-NAME-2");
		test.check("Unexpected fld tmp mode", fldTmp.getMode() == "FIELD-MODE-2");
		test.check("Unexpected fld tmp class", fldTmp.getClass() == "FIELD-CLASS-2");
		test.check("Unexpected fld tmp desc", fldTmp.getDescription() == "FIELD-DESC-2");
	}
	catch (const GmsecException& e) {
		test.check(e.what(), false);
	}

	//o Bogus field template
	try {
		(void) tmp->getFieldTemplate("BOGUS-NAME");
		test.check("Expected an exception", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), true);
	}
}


void test_get_subject_names(Test& test)
{
	GMSEC_INFO << "Test getSubjectNames()";

	StdUniquePtr<MessageTemplate> tmp( msgTemplateBuilder() );

	test.check("Unexpected subject names", tmp->getSubjectNames().empty());
	test.check("Unexpected subject definitions", tmp->getSubjectDefinition().empty());

	MessageTemplate::SubjectElementList subject;
	subject.push_back( MessageTemplate::SubjectElement("SPECIFICATION", "C2MS") );
	subject.push_back( MessageTemplate::SubjectElement("MESSAGE-TYPE", "MSG") );

	tmp->setSubject(subject);

	test.check("Unexpected subject names", tmp->getSubjectNames() == "SPECIFICATION.MESSAGE-TYPE");
	test.check("Unexpected subject definitions", tmp->getSubjectDefinition() == "C2MS.MSG");

	subject = tmp->getSubjectElements();
	test.check("Unexpected count of subject elements", subject.size() == 2);
	for (size_t i = 0; i < subject.size(); ++i)
	{
		const MessageTemplate::SubjectElement& elem = subject[i];
		if (i == 0) {
			test.check("Unexpected element name", elem.first == "SPECIFICATION");
			test.check("Unexpected element value", elem.second == "C2MS");
		}
		else {
			test.check("Unexpected element name", elem.first == "MESSAGE-TYPE");
			test.check("Unexpected element value", elem.second == "MSG");
		}
	}
}


void test_add_subject_header(Test& test)
{
	GMSEC_INFO << "Test addSubjectHeader()";

	StdUniquePtr<MessageTemplate> tmp( msgTemplateBuilder() );

	MessageTemplate::SubjectElementList subject, header;
	subject.push_back( MessageTemplate::SubjectElement("SPECIFICATION", "C2MS") );
	subject.push_back( MessageTemplate::SubjectElement("MESSAGE-TYPE", "MSG") );

	header.push_back( MessageTemplate::SubjectElement("FOO", "BAR") );
	header.push_back( MessageTemplate::SubjectElement("BAZ", "BAM") );

	tmp->setSubject(subject);
	tmp->addSubjectHeader(header);

	test.check("Unexpected subject names", tmp->getSubjectNames() == "FOO.BAZ.SPECIFICATION.MESSAGE-TYPE");
	test.check("Unexpected subject definitions", tmp->getSubjectDefinition() == "BAR.BAM.C2MS.MSG");
}


void test_set_get_definition(Test& test)
{
	GMSEC_INFO << "Test setDefinition() and getDefinition()";

	StdUniquePtr<MessageTemplate> tmp( msgTemplateBuilder() );

	tmp->setDefinition("My definition");

	test.check("Unexpected definition", tmp->getDefinition() == "My definition");
}


void test_get_field_templates(Test& test)
{
	GMSEC_INFO << "Test getFieldTemplates()";

	StdUniquePtr<MessageTemplate> tmp( msgTemplateBuilder() );

	const MessageTemplate::FieldTemplateList& tmpList = tmp->getFieldTemplates();

	test.check("Unexpected number of field templates", tmpList.size() == 2);

	for (MessageTemplate::FieldTemplateList::const_iterator it = tmpList.begin(); it != tmpList.end(); ++it)
	{
		if (it == tmpList.begin()) {
			test.check("Unexpected fld tmp name", (*it)->getName() == "FIELD-NAME-1");
			test.check("Unexpected fld tmp mode", (*it)->getMode() == "FIELD-MODE-1");
			test.check("Unexpected fld tmp class", (*it)->getClass() == "FIELD-CLASS-1");
			test.check("Unexpected fld tmp desc", (*it)->getDescription() == "FIELD-DESC-1");
		}
		else {
			test.check("Unexpected fld tmp name", (*it)->getName() == "FIELD-NAME-2");
			test.check("Unexpected fld tmp mode", (*it)->getMode() == "FIELD-MODE-2");
			test.check("Unexpected fld tmp class", (*it)->getClass() == "FIELD-CLASS-2");
			test.check("Unexpected fld tmp desc", (*it)->getDescription() == "FIELD-DESC-2");
		}
	}
}


void test_ostream_operator(Test& test)
{
	GMSEC_INFO << "Test ostream operator";

	StdUniquePtr<MessageTemplate> tmp( msgTemplateBuilder() );

	std::ostringstream oss;
	oss << *(tmp.get());
	test.check("Unexpected ostringstream content", oss.str().size() > 0);
}


int test_MessageTemplate(Test& test)
{
	test.setDescription("MessageTemplate");

	test_constructor(test);
	test_copy_constructor(test);
	test_assignment_operator(test);
	test_get_field(test);
	test_get_field_template(test);
	test_get_subject_names(test);
	test_add_subject_header(test);
	test_set_get_definition(test);
	test_get_field_templates(test);
	test_ostream_operator(test);

	return 0;
}

TEST_DRIVER(test_MessageTemplate)
