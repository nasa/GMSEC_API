#include "TestCase.h"

#include <gmsec5/internal/Value.h>

#include <string>
#include <sstream>

using namespace gmsec::api5;
using namespace gmsec::api5::internal;


class MyValue : public Value
{
public:
	MyValue(int v) : value(v) {}

	virtual ~MyValue() {};

	int getValue() const { return value; }

	bool isOpaque() const { return true; }

	Status getBoolean(bool& v) const {
		return Status(FIELD_ERROR, FIELD_TYPE_MISMATCH, "Opaque value cannot be converted to a bool value");
	}

	Status getI32(GMSEC_I32& v) const {
		v = value;
		return Status();
	}

	Status getF64(GMSEC_F64& v) const {
		return Status(FIELD_ERROR, FIELD_TYPE_MISMATCH, "Opaque value cannot be converted to a GMSEC_F64 value");
	}

	Status getString(std::string& v) const {
		return Status(FIELD_ERROR, FIELD_TYPE_MISMATCH, "Opaque value cannot be converted to an std::string value");
	}

	Value* clone() const {
		return new MyValue(value);
	}

private:
	int value;
};


void test_constructor(Test& test)
{
	GMSEC_INFO << "Test constructor";

	ValueMap vm;
	test.check("Unexpected size", vm.size() == 0);
}


void test_contains(Test& test)
{
	GMSEC_INFO << "Test contains()";

	ValueMap vm;
	test.check("Unexpected entry", vm.contains("foobar") == false);

	vm.setBoolean("foobar", true);
	test.check("Expected an entry", vm.contains("foobar"));
}


void test_size(Test& test)
{
	GMSEC_INFO << "Test size()";

	ValueMap vm;
	test.check("Unexpected size", vm.size() == 0);
	vm.setBoolean("foobar", true);
	test.check("Unexpected size", vm.size() == 1);
	vm.setBoolean("foobar2", false);
	test.check("Unexpected size", vm.size() == 2);
}


void test_setters_getters(Test& test)
{
	GMSEC_INFO << "Test setters and getters";

	ValueMap vm;
	vm.setBoolean("value1", true);
	vm.setI32("value2", 1);
	vm.setF64("value3", 3.14);
	vm.setString("value4", "foobar");
	vm.setOpaqueValue("value5", new MyValue(123));

	Status      status;
	bool        boolValue;
	GMSEC_I32   i32Value;
	GMSEC_F64   f64Value;
	std::string strValue;
	Value*      opaqueValue;

	//o getBoolean
	status = vm.getBoolean("value1", boolValue);
	test.check("Unexpected status", !status.hasError());
	test.check("Unexpected bool value", boolValue);

	status = vm.getBoolean("no-value", boolValue);
	test.check("Unexpected status", status.hasError());

	status = vm.getBoolean("no-value", boolValue, &boolValue);
	test.check("Unexpected status", !status.hasError());
	test.check("Unexpected bool value", boolValue);

	//o getI32
	status = vm.getI32("value2", i32Value);
	test.check("Unexpected status", !status.hasError());
	test.check("Unexpected I32 value", i32Value == 1);

	status = vm.getI32("no-value", i32Value);
	test.check("Unexpected status", status.hasError());

	status = vm.getI32("no-value", i32Value, &i32Value);
	test.check("Unexpected status", !status.hasError());
	test.check("Unexpected I32 value", i32Value == 1);

	//o getF64
	status = vm.getF64("value3", f64Value);
	test.check("Unexpected status", !status.hasError());
	test.check("Unexpected F64 value", f64Value == 3.14);

	status = vm.getF64("no-value", f64Value);
	test.check("Unexpected status", status.hasError());

	status = vm.getF64("no-value", f64Value, &f64Value);
	test.check("Unexpected status", !status.hasError());
	test.check("Unexpected F64 value", f64Value == 3.14);

	//o getString
	status = vm.getString("value4", strValue);
	test.check("Unexpected status", !status.hasError());
	test.check("Unexpected string value", strValue == "foobar");

	status = vm.getString("no-value", strValue);
	test.check("Unexpected status", status.hasError());

	status = vm.getString("no-value", strValue, &strValue);
	test.check("Unexpected status", !status.hasError());
	test.check("Unexpected string value", strValue == "foobar");

	//o getOpaque
	opaqueValue = vm.getOpaqueValue("value5");
	test.require("Unexpected opaque value", opaqueValue != NULL);
	test.check("Unexpected opaque value", reinterpret_cast<MyValue*>(opaqueValue)->getValue() == 123);

	opaqueValue = vm.getOpaqueValue("no-value");
	test.check("Unexpected opaque value", opaqueValue == NULL);
}


void test_clear_value(Test& test)
{
	GMSEC_INFO << "Test clearValue()";

	ValueMap vm;
	vm.setBoolean("value1", true);

	bool boolValue = false;

	vm.clearValue("value1");

	(void) vm.getBoolean("value1", boolValue, &boolValue);
	test.check("Unexpected value", boolValue == false);
}


void test_iterator(Test& test)
{
	GMSEC_INFO << "Test iterator";

	ValueMap vm;
	vm.setI32("value1", 1);
	vm.setI32("value2", 2);
	vm.setI32("value3", 3);

	ValueMap::Iterator iter;
	const Value* value = iter.next();
	test.check("Expected NULL from uninitialized Iterator", value == NULL);

	vm.reset(iter);

	int i = 0;
	while ((value = iter.next()))
	{
		GMSEC_I32 v;

		(void) value->getI32(v);

		test.check("Unexpected value", v == (i+1));

		if (i == 0) test.check("Unexpected ID", iter.getID() == "value1");
		if (i == 1) test.check("Unexpected ID", iter.getID() == "value2");
		if (i == 2) test.check("Unexpected ID", iter.getID() == "value3");

		++i;
	}
}


void test_clone_values(Test& test)
{
	GMSEC_INFO << "Test cloneValues()";

	ValueMap vm1, vm2;
	vm1.setI32("value1", 1);
	vm1.setI32("value2", 2);
	vm1.setI32("value3", 3);

	//o Call cloneValues (first time)
	vm2.cloneValues(vm1);
	test.check("Unexpected size", vm1.size() == vm2.size());

	//o Call cloneValues (second time, to ensure previous entries are deleted)
	vm2.cloneValues(vm1);
	test.check("Unexpected size", vm1.size() == vm2.size());

	ValueMap::Iterator iter;
	vm2.reset(iter);

	const Value* value = iter.next();
	int i = 0;
	while (value)
	{
		GMSEC_I32 v;

		(void) value->getI32(v);

		test.check("Unexpected value", v == (i+1));

		if (i == 0) test.check("Unexpected ID", iter.getID() == "value1");
		if (i == 1) test.check("Unexpected ID", iter.getID() == "value2");
		if (i == 2) test.check("Unexpected ID", iter.getID() == "value3");

		value = iter.next();

		++i;
	}
}


void test_ostream(Test& test)
{
	GMSEC_INFO << "Test ostream operator";

	ValueMap vm;
	vm.setI32("value1", 1);
	vm.setI32("value2", 2);
	vm.setI32("value3", 3);

	std::ostringstream oss;
	oss << vm;
	test.check("Unexpected ostream", oss.str().find("value1") != std::string::npos);
	test.check("Unexpected ostream", oss.str().find("value2") != std::string::npos);
	test.check("Unexpected ostream", oss.str().find("value3") != std::string::npos);
}


void test_transfer_header_fields(Test& test)
{
	GMSEC_INFO << "Test transferHeaderFields()";

	MessageFactory factory;
	Message msg = factory.createMessage("HB");

	//o Add non-header field to the message
	msg.addField("NON-HEADER", static_cast<GMSEC_U32>(1));

	//o Add misc header fields to the message
	msg.addField(BooleanField("BOOL", true, true));
	msg.addField(CharField("CHAR", 'c', true));
	msg.addField(I8Field("I8", static_cast<GMSEC_I8>(1), true));
	msg.addField(U8Field("U8", static_cast<GMSEC_U8>(1), true));
	msg.addField(I16Field("I16", static_cast<GMSEC_I16>(1), true));
	msg.addField(U16Field("U16", static_cast<GMSEC_U16>(1), true));
	msg.addField(I32Field("I32", static_cast<GMSEC_I32>(1), true));
	msg.addField(U32Field("U32", static_cast<GMSEC_U32>(1), true));
	msg.addField(F32Field("F32", static_cast<GMSEC_F32>(1), true));
	msg.addField(F64Field("F64", static_cast<GMSEC_F64>(1), true));

	ValueMap vm;

	transferHeaderFields(msg, vm);

	test.check("Unexpected ValueMap size", vm.size() == 14);
	test.check("Unexpected NON-HEADER", !vm.contains("NON-HEADER"));
	test.check("Missing CHAR", vm.contains("CHAR"));
	test.check("Missing I8", vm.contains("I8"));
	test.check("Missing U8", vm.contains("U8"));
	test.check("Missing I16", vm.contains("I16"));
	test.check("Missing U16", vm.contains("U16"));
	test.check("Missing I32", vm.contains("I32"));
	test.check("Missing U32", vm.contains("U32"));
	test.check("Missing F32", vm.contains("F32"));
	test.check("Missing F64", vm.contains("F64"));
	test.check("Missing HEADER-VERSION", vm.contains("HEADER-VERSION"));
	test.check("Missing MESSAGE-SUBTYPE", vm.contains("MESSAGE-SUBTYPE"));
	test.check("Missing MESSAGE-TYPE", vm.contains("MESSAGE-TYPE"));
	test.check("Missing SPECIFICATION", vm.contains("SPECIFICATION"));
}


int test_ValueMap(Test& test)
{
	test.setDescription("ValueMap");

	test_constructor(test);
	test_contains(test);
	test_size(test);
	test_setters_getters(test);
	test_clear_value(test);
	test_iterator(test);
	test_clone_values(test);
	test_ostream(test);
	test_transfer_header_fields(test);

	return 0;
}

TEST_DRIVER(test_ValueMap)
