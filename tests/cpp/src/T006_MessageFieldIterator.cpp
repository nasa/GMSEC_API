#include "TestCase.h"

#include <gmsec5/internal/field/InternalField.h>

using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::util;


class TestMessage
{
public:
	static TestMessage& instance()
	{
		static TestMessage tm;
		return tm;
	}

	const Message& getMessage() const
	{
		return msg;
	}

private:
	TestMessage()
		: msg()
	{
		StringField field1("FIELD-1", "1", true);
		StringField field2("FIELD-2", "2", true);
		I32Field    field3("FIELD-3", 3, false);
		StringField field4("FIELD-4", "4", true);

		FieldBuddy::getInternal(field4).isTracking(true);

		msg.addField(field1);
		msg.addField(field2);
		msg.addField(field3);
		msg.addField(field4);
	}

	Message msg;
};


void test_all_fields(Test& test)
{
	GMSEC_INFO << "Test Selector::ALL_FIELDS()";

	const Message& msg = TestMessage::instance().getMessage();

	MessageFieldIterator& iter = msg.getFieldIterator( MessageFieldIterator::Selector::ALL_FIELDS );

	int fieldsFound = 0;

	while (iter.hasNext())
	{
		const Field& field = iter.next();

		++fieldsFound;

		switch (fieldsFound)
		{
		case 1:
			test.check("Unexpected field type 1", Field::Type::STRING == field.getType());
			test.check("Unexpected field name 1", std::string("FIELD-1") == field.getName());
			test.check("Unexpected field value 1", std::string("1") == field.getStringValue());
			test.check("Unexpected tracking field 1", field.isTracking() == false);
			break;

		case 2:
			test.check("Unexpected field type 2", Field::Type::STRING == field.getType());
			test.check("Unexpected field name 2", std::string("FIELD-2") == field.getName());
			test.check("Unexpected field value 2", std::string("2") == field.getStringValue());
			test.check("Unexpected tracking field 2", field.isTracking() == false);
			break;

		case 3:
			test.check("Unexpected field type 3", Field::Type::I32 == field.getType());
			test.check("Unexpected field name 3", std::string("FIELD-3") == field.getName());
			test.check("Unexpected field value 3", 3 == field.getI32Value());
			test.check("Unexpected tracking field 3", field.isTracking() == false);
			break;

		case 4:
			test.check("Unexpected field type 4", Field::Type::STRING == field.getType());
			test.check("Unexpected field name 4", std::string("FIELD-4") == field.getName());
			test.check("Unexpected field value 4", std::string("4") == field.getStringValue());
			test.check("Unexpected tracking field 4", field.isTracking());
			break;
		}
	}

	test.check("Unexpected field count", 4 == fieldsFound);
}


void test_header_fields(Test& test)
{
	GMSEC_INFO << "Test Selector::HEADER_FIELDS()";

	const Message& msg = TestMessage::instance().getMessage();

	MessageFieldIterator& iter = msg.getFieldIterator( MessageFieldIterator::Selector::HEADER_FIELDS );

	int fieldsFound = 0;

	while (iter.hasNext())
	{
		const Field& field = iter.next();

		++fieldsFound;

		switch (fieldsFound)
		{
		case 1:
			test.check("Unexpected field type 1", Field::Type::STRING == field.getType());
			test.check("Unexpected field name 1", std::string("FIELD-1") == field.getName());
			test.check("Unexpected field value 1", std::string("1") == field.getStringValue());
			test.check("Unexpected tracking field 1", field.isTracking() == false);
			break;

		case 2:
			test.check("Unexpected field type 2", Field::Type::STRING == field.getType());
			test.check("Unexpected field name 2", std::string("FIELD-2") == field.getName());
			test.check("Unexpected field value 2", std::string("2") == field.getStringValue());
			test.check("Unexpected tracking field 1", field.isTracking() == false);
			break;

		case 3:
			test.check("Unexpected field type 4", Field::Type::STRING == field.getType());
			test.check("Unexpected field name 4", std::string("FIELD-4") == field.getName());
			test.check("Unexpected field value 4", std::string("4") == field.getStringValue());
			test.check("Unexpected tracking field 4", field.isTracking());
			break;

		default:
			test.check("Unexpected field returned", false);
			break;
		}
	}

	test.check("Unexpected field count", 3 == fieldsFound);
}


void test_non_header_fields(Test& test)
{
	GMSEC_INFO << "Test Selector::NON_HEADER_FIELDS()";

	const Message& msg = TestMessage::instance().getMessage();

	MessageFieldIterator& iter = msg.getFieldIterator( MessageFieldIterator::Selector::NON_HEADER_FIELDS );

	int fieldsFound = 0;

	while (iter.hasNext())
	{
		const Field& field = iter.next();

		++fieldsFound;

		switch (fieldsFound)
		{
		case 1:
			test.check("Unexpected field type 3", Field::Type::I32 == field.getType());
			test.check("Unexpected field name 3", std::string("FIELD-3") == field.getName());
			test.check("Unexpected field value 3", 3 == field.getI32Value());
			test.check("Unexpected tracking field 3", field.isTracking() == false);
			break;

		default:
			test.check("Unexpected field returned", false);
			break;
		}
	}

	test.check("Unexpected field count", 1 == fieldsFound);
}


void test_tracking_fields(Test& test)
{
	GMSEC_INFO << "Test Selector::TRACKING_FIELDS()";

	const Message& msg = TestMessage::instance().getMessage();

	MessageFieldIterator& iter = msg.getFieldIterator( MessageFieldIterator::Selector::TRACKING_FIELDS );

	int fieldsFound = 0;

	while (iter.hasNext())
	{
		const Field& field = iter.next();

		++fieldsFound;

		switch (fieldsFound)
		{
		case 1:
			test.check("Unexpected field type 4", Field::Type::STRING == field.getType());
			test.check("Unexpected field name 4", std::string("FIELD-4") == field.getName());
			test.check("Unexpected field value 4", std::string("4") == field.getStringValue());
			test.check("Unexpected tracking field 4", field.isTracking());
			break;

		default:
			test.check("Unexpected field returned", false);
			break;
		}
	}

	test.check("Unexpected field count", 1 == fieldsFound);
}


void test_reset(Test& test)
{
	GMSEC_INFO << "Test reset()";

	const Message& msg = TestMessage::instance().getMessage();

	MessageFieldIterator& iter = msg.getFieldIterator();

	int fieldsFound = 0;

	while (iter.hasNext())
	{
		(void) iter.next();
		++fieldsFound;
	}

	test.check("Unexpected field count", 4 == fieldsFound);

	test.check("Should be no more field(s) to iterate over", false == iter.hasNext());

	iter.reset();

	test.check("Should have field(s) to iterator over", true == iter.hasNext());

	fieldsFound = 0;

	while (iter.hasNext())
	{
		(void) iter.next();
		++fieldsFound;
	}

	test.check("Unexpected field count", 4 == fieldsFound);
}


int test_MessageFieldIterator(Test& test)
{
	test.setDescription("MessageFieldIterator");

	test_all_fields(test);          // test implicitly tests hasNext() and next()
	test_header_fields(test);       // test implicitly tests hasNext() and next()
	test_non_header_fields(test);   // test implicitly tests hasNext() and next()
	test_tracking_fields(test);     // test implicitly tests hasNext() and next()
	test_reset(test);

	return 0;
}

TEST_DRIVER(test_MessageFieldIterator)
