#include "TestCase.h"

#include <limits>

using namespace gmsec::api5;


void test_BinaryField(Test& test)
{
	GMSEC_INFO << "Test BinaryField...";

	unsigned char blob[] = { 0x00, 0x02, 0x04, 0x08, 0xFF };
	size_t blobLen = sizeof(blob)/sizeof(unsigned char);
	std::string name = "BINARY-FIELD";

	BinaryField f1(name.c_str(), blob, blobLen);

	GMSEC_U8 const* f1_blob = f1.getValue();
	size_t          f1_blobLen = f1.getLength();

	test.check("Unexpected field name", name == f1.getName());
	test.check("Unexpected field type", Field::Type::BINARY == f1.getType());
	test.check("Unexpected blob length", blobLen == f1_blobLen);
	for (size_t i = 0; i < blobLen; ++i)
	{
		bool same = (blob[i] == f1_blob[i]);

		if (!same)
		{
			std::ostringstream oss;
			oss << "Blob data not the same at position: " << i;
			test.check(oss.str().c_str(), false);
		}
	}
	test.check("Unexpected XML string", std::string("<FIELD NAME=\"BINARY-FIELD\" TYPE=\"BIN\">00020408FF</FIELD>") == f1.toXML());
	test.check("Unexpected JSON string", std::string("{\"NAME\":\"BINARY-FIELD\",\"TYPE\":\"BIN\",\"VALUE\":\"00020408FF\"}") == f1.toJSON());


	GMSEC_INFO << "Test BinaryField copy-constructor...";

	BinaryField f2(f1);

	GMSEC_U8 const* f2_blob = f2.getValue();
	size_t          f2_blobLen = f2.getLength();

	test.check("Unexpected field name", name == f2.getName());
	test.check("Unexpected field type", f1.getType() == f2.getType());
	test.check("Unexpcted blob length", f1_blobLen == f2_blobLen);
	for (size_t i = 0; i < blobLen; ++i)
	{
		bool same = (f1_blob[i] == f2_blob[i]);

		if (!same)
		{
			std::ostringstream oss;
			oss << "Blob data not the same at position: " << i;
			test.check(oss.str().c_str(), false);
		}
	}
	test.check("Unexpected XML string", std::string("<FIELD NAME=\"BINARY-FIELD\" TYPE=\"BIN\">00020408FF</FIELD>") == f2.toXML());
	test.check("Unexpected JSON string", std::string("{\"NAME\":\"BINARY-FIELD\",\"TYPE\":\"BIN\",\"VALUE\":\"00020408FF\"}") == f2.toJSON());


	GMSEC_INFO << "Test BinaryField off-nominal cases...";

	try {
		BinaryField f3(NULL, blob, blobLen);
		test.check("Expected BinaryField to disallow NULL field name", false);
	}
	catch (...) {
		test.check("Okay", true);
	}


	try {
		BinaryField f3("", blob, blobLen);
		test.check("Expected BinaryField to disallow empty-string field name", false);
	}
	catch (...) {
		test.check("Okay", true);
	}
}


void test_BooleanField(Test& test)
{
	GMSEC_INFO << "Test BooleanField...";

	std::string name = "BOOLEAN-FIELD";
	bool value = true;

	BooleanField f1(name.c_str(), value);

	test.check("Unexpected field name", name == f1.getName());
	test.check("Unexpected field type", Field::Type::BOOL == f1.getType());
	test.check("Unexpected field value", value == f1.getValue());
	test.check("Unexpected XML string", std::string("<FIELD NAME=\"BOOLEAN-FIELD\" TYPE=\"BOOL\">TRUE</FIELD>") == f1.toXML());
	test.check("Unexpected JSON string", std::string("{\"NAME\":\"BOOLEAN-FIELD\",\"TYPE\":\"BOOL\",\"VALUE\":\"TRUE\"}") == f1.toJSON());


	GMSEC_INFO << "Test BooleanField copy-constructor...";

	BooleanField f2(f1);

	test.check("Unexpected field name", name == f2.getName());
	test.check("Unexpected field type", f1.getType() == f2.getType());
	test.check("Unexpected field value", f1.getValue() == f2.getValue());
	test.check("Unexpected XML string", std::string(f1.toXML()) == f2.toXML());
	test.check("Unexpected JSON string", std::string(f1.toJSON()) == f2.toJSON());


	GMSEC_INFO << "Test BooleanField off-nominal cases...";

	try {
		BooleanField f3(NULL, value);
		test.check("Expected BooleanField to disallow NULL field name", false);
	}
	catch (...) {
		test.check("Okay", true);
	}


	try {
		BooleanField f3("", value);
		test.check("Expected BooleanField to disallow empty-string field name", false);
	}
	catch (...) {
		test.check("Okay", true);
	}
}


void test_CharField(Test& test)
{
	GMSEC_INFO << "Test CharField...";

	std::string name = "CHAR-FIELD";
	char value = 'z';

	CharField f1(name.c_str(), value);

	test.check("Unexpected field name", name == f1.getName());
	test.check("Unexpected field type", Field::Type::CHAR == f1.getType());
	test.check("Unexpected field value", value == f1.getValue());
	test.check("Unexpected XML string", std::string("<FIELD NAME=\"CHAR-FIELD\" TYPE=\"CHAR\">z</FIELD>") == f1.toXML());
	test.check("Unexpected JSON string", std::string("{\"NAME\":\"CHAR-FIELD\",\"TYPE\":\"CHAR\",\"VALUE\":\"z\"}") == f1.toJSON());


	GMSEC_INFO << "Test CharField copy-constructor...";

	CharField f2(f1);

	test.check("Unexpected field name", name == f2.getName());
	test.check("Unexpected field type", f1.getType() == f2.getType());
	test.check("Unexpected field value", f1.getValue() == f2.getValue());
	test.check("Unexpected XML string", std::string(f1.toXML()) == f2.toXML());
	test.check("Unexpected JSON string", std::string(f1.toJSON()) == f2.toJSON());


	GMSEC_INFO << "Test CharField off-nominal cases...";

	try {
		CharField f3(NULL, value);
		test.check("Expected CharField to disallow NULL field name", false);
	}
	catch (...) {
		test.check("Okay", true);
	}


	try {
		CharField f3("", value);
		test.check("Expected CharField to disallow empty-string field name", false);
	}
	catch (...) {
		test.check("Okay", true);
	}
}


void test_F32Field(Test& test)
{
	GMSEC_INFO << "Test F32Field...";

	std::string name = "F32-FIELD";
	GMSEC_F32 value = 3.14f;

	F32Field f1(name.c_str(), value);

	test.check("Unexpected field name", name == f1.getName());
	test.check("Unexpected field type", Field::Type::F32 == f1.getType());
	test.check("Unexpected field value", value == f1.getValue());
	test.check("Unexpected XML string", std::string("<FIELD NAME=\"F32-FIELD\" TYPE=\"F32\" BITS=\"4048F5C3\">3.14</FIELD>") == f1.toXML());
	test.check("Unexpected JSON string", std::string("{\"NAME\":\"F32-FIELD\",\"TYPE\":\"F32\",\"BITS\":\"4048F5C3\",\"VALUE\":\"3.14\"}") == f1.toJSON());


	GMSEC_INFO << "Test F32Field copy-constructor...";

	F32Field f2(f1);

	test.check("Unexpected field name", name == f2.getName());
	test.check("Unexpected field type", f1.getType() == f2.getType());
	test.check("Unexpected field value", f1.getValue() == f2.getValue());
	test.check("Unexpected XML string", std::string(f1.toXML()) == f2.toXML());
	test.check("Unexpected JSON string", std::string(f1.toJSON()) == f2.toJSON());


	GMSEC_INFO << "Test F32Field off-nominal cases...";

	try {
		F32Field f3(NULL, value);
		test.check("Expected F32Field to disallow NULL field name", false);
	}
	catch (...) {
		test.check("Okay", true);
	}


	try {
		F32Field f3("", value);
		test.check("Expected F32Field to disallow empty-string field name", false);
	}
	catch (...) {
		test.check("Okay", true);
	}
}


void test_F64Field(Test& test)
{
	GMSEC_INFO << "Test F64Field...";

	std::string name = "F64-FIELD";
	GMSEC_F64 value = 3.14;

	F64Field f1(name.c_str(), value);

	test.check("Unexpected field name", name == f1.getName());
	test.check("Unexpected field type", Field::Type::F64 == f1.getType());
	test.check("Unexpected field value", value == f1.getValue());
	test.check("Unexpected XML string", std::string("<FIELD NAME=\"F64-FIELD\" TYPE=\"F64\" BITS=\"40091EB851EB851F\">3.14</FIELD>") == f1.toXML());
	test.check("Unexpected JSON string", std::string("{\"NAME\":\"F64-FIELD\",\"TYPE\":\"F64\",\"BITS\":\"40091EB851EB851F\",\"VALUE\":\"3.14\"}") == f1.toJSON());


	GMSEC_INFO << "Test F64Field copy-constructor...";

	F64Field f2(f1);

	test.check("Unexpected field name", name == f2.getName());
	test.check("Unexpected field type", f1.getType() == f2.getType());
	test.check("Unexpected field value", f1.getValue() == f2.getValue());
	test.check("Unexpected XML string", std::string(f1.toXML()) == f2.toXML());
	test.check("Unexpected JSON string", std::string(f1.toJSON()) == f2.toJSON());


	GMSEC_INFO << "Test F64Field off-nominal cases...";

	try {
		F64Field f3(NULL, value);
		test.check("Expected F64Field to disallow NULL field name", false);
	}
	catch (...) {
		test.check("Okay", true);
	}


	try {
		F64Field f3("", value);
		test.check("Expected F64Field to disallow empty-string field name", false);
	}
	catch (...) {
		test.check("Okay", true);
	}
}


void test_I16Field(Test& test)
{
	GMSEC_INFO << "Test I16Field...";

	std::string name = "I16-FIELD";
	GMSEC_I16 value = -10;

	I16Field f1(name.c_str(), value);

	test.check("Unexpected field name", name == f1.getName());
	test.check("Unexpected field type", Field::Type::I16 == f1.getType());
	test.check("Unexpected field value", value == f1.getValue());
	test.check("Unexpected XML string", std::string("<FIELD NAME=\"I16-FIELD\" TYPE=\"I16\">-10</FIELD>") == f1.toXML());
	test.check("Unexpected JSON string", std::string("{\"NAME\":\"I16-FIELD\",\"TYPE\":\"I16\",\"VALUE\":\"-10\"}") == f1.toJSON());


	GMSEC_INFO << "Test I16Field copy-constructor...";

	I16Field f2(f1);

	test.check("Unexpected field name", name == f2.getName());
	test.check("Unexpected field type", f1.getType() == f2.getType());
	test.check("Unexpected field value", f1.getValue() == f2.getValue());
	test.check("Unexpected XML string", std::string(f1.toXML()) == f2.toXML());
	test.check("Unexpected JSON string", std::string(f1.toJSON()) == f2.toJSON());


	GMSEC_INFO << "Test I16Field off-nominal cases...";

	try {
		I16Field f3(NULL, value);
		test.check("Expected I16Field to disallow NULL field name", false);
	}
	catch (...) {
		test.check("Okay", true);
	}


	try {
		I16Field f3("", value);
		test.check("Expected I16Field to disallow empty-string field name", false);
	}
	catch (...) {
		test.check("Okay", true);
	}
}


void test_I32Field(Test& test)
{
	GMSEC_INFO << "Test I32Field...";

	std::string name = "I32-FIELD";
	GMSEC_I32 value = 5;

	I32Field f1(name.c_str(), value);

	test.check("Unexpected field name", name == f1.getName());
	test.check("Unexpected field type", Field::Type::I32 == f1.getType());
	test.check("Unexpected field value", value == f1.getValue());
	test.check("Unexpected XML string", std::string("<FIELD NAME=\"I32-FIELD\" TYPE=\"I32\">5</FIELD>") == f1.toXML());
	test.check("Unexpected JSON string", std::string("{\"NAME\":\"I32-FIELD\",\"TYPE\":\"I32\",\"VALUE\":\"5\"}") == f1.toJSON());


	GMSEC_INFO << "Test I32Field copy-constructor...";

	I32Field f2(f1);

	test.check("Unexpected field name", name == f2.getName());
	test.check("Unexpected field type", f1.getType() == f2.getType());
	test.check("Unexpected field value", f1.getValue() == f2.getValue());
	test.check("Unexpected XML string", std::string(f1.toXML()) == f2.toXML());
	test.check("Unexpected JSON string", std::string(f1.toJSON()) == f2.toJSON());


	GMSEC_INFO << "Test I32Field off-nominal cases...";

	try {
		I32Field f3(NULL, value);
		test.check("Expected I32Field to disallow NULL field name", false);
	}
	catch (...) {
		test.check("Okay", true);
	}


	try {
		I32Field f3("", value);
		test.check("Expected I32Field to disallow empty-string field name", false);
	}
	catch (...) {
		test.check("Okay", true);
	}
}


void test_I64Field(Test& test)
{
	GMSEC_INFO << "Test I64Field...";

	std::string name = "I64-FIELD";
	GMSEC_I64 value = -5;

	I64Field f1(name.c_str(), value);

	test.check("Unexpected field name", name == f1.getName());
	test.check("Unexpected field type", Field::Type::I64 == f1.getType());
	test.check("Unexpected field value", value == f1.getValue());
	test.check("Unexpected XML string", std::string("<FIELD NAME=\"I64-FIELD\" TYPE=\"I64\">-5</FIELD>") == f1.toXML());
	test.check("Unexpected JSON string", std::string("{\"NAME\":\"I64-FIELD\",\"TYPE\":\"I64\",\"VALUE\":\"-5\"}") == f1.toJSON());


	GMSEC_INFO << "Test I64Field copy-constructor...";

	I64Field f2(f1);

	test.check("Unexpected field name", name == f2.getName());
	test.check("Unexpected field type", f1.getType() == f2.getType());
	test.check("Unexpected field value", f1.getValue() == f2.getValue());
	test.check("Unexpected XML string", std::string(f1.toXML()) == f2.toXML());
	test.check("Unexpected JSON string", std::string(f1.toJSON()) == f2.toJSON());


	GMSEC_INFO << "Test I64Field off-nominal cases...";

	try {
		I64Field f3(NULL, value);
		test.check("Expected I64Field to disallow NULL field name", false);
	}
	catch (...) {
		test.check("Okay", true);
	}


	try {
		I64Field f3("", value);
		test.check("Expected I64Field to disallow empty-string field name", false);
	}
	catch (...) {
		test.check("Okay", true);
	}
}


void test_I8Field(Test& test)
{
	GMSEC_INFO << "Test I8Field...";

	std::string name = "I8-FIELD";
	GMSEC_I8 value = 5;

	I8Field f1(name.c_str(), value);

	test.check("Unexpected field name", name == f1.getName());
	test.check("Unexpected field type", Field::Type::I8 == f1.getType());
	test.check("Unexpected field value", value == f1.getValue());
	test.check("Unexpected XML string", std::string("<FIELD NAME=\"I8-FIELD\" TYPE=\"I8\">5</FIELD>") == f1.toXML());
	test.check("Unexpected JSON string", std::string("{\"NAME\":\"I8-FIELD\",\"TYPE\":\"I8\",\"VALUE\":\"5\"}") == f1.toJSON());


	GMSEC_INFO << "Test I8Field copy-constructor...";

	I8Field f2(f1);

	test.check("Unexpected field name", name == f2.getName());
	test.check("Unexpected field type", f1.getType() == f2.getType());
	test.check("Unexpected field value", f1.getValue() == f2.getValue());
	test.check("Unexpected XML string", std::string(f1.toXML()) == f2.toXML());
	test.check("Unexpected JSON string", std::string(f1.toJSON()) == f2.toJSON());


	GMSEC_INFO << "Test I8Field off-nominal cases...";

	try {
		I8Field f3(NULL, value);
		test.check("Expected I8Field to disallow NULL field name", false);
	}
	catch (...) {
		test.check("Okay", true);
	}


	try {
		I8Field f3("", value);
		test.check("Expected I8Field to disallow empty-string field name", false);
	}
	catch (...) {
		test.check("Okay", true);
	}
}


void test_U16Field(Test& test)
{
	GMSEC_INFO << "Test U16Field...";

	std::string name = "U16-FIELD";
	GMSEC_U16 value = 10;

	U16Field f1(name.c_str(), value);

	test.check("Unexpected field name", name == f1.getName());
	test.check("Unexpected field type", Field::Type::U16 == f1.getType());
	test.check("Unexpected field value", value == f1.getValue());
	test.check("Unexpected XML string", std::string("<FIELD NAME=\"U16-FIELD\" TYPE=\"U16\">10</FIELD>") == f1.toXML());
	test.check("Unexpected JSON string", std::string("{\"NAME\":\"U16-FIELD\",\"TYPE\":\"U16\",\"VALUE\":\"10\"}") == f1.toJSON());


	GMSEC_INFO << "Test U16Field copy-constructor...";

	U16Field f2(f1);

	test.check("Unexpected field name", name == f2.getName());
	test.check("Unexpected field type", f1.getType() == f2.getType());
	test.check("Unexpected field value", f1.getValue() == f2.getValue());
	test.check("Unexpected XML string", std::string(f1.toXML()) == f2.toXML());
	test.check("Unexpected JSON string", std::string(f1.toJSON()) == f2.toJSON());


	GMSEC_INFO << "Test U16Field off-nominal cases...";

	try {
		U16Field f3(NULL, value);
		test.check("Expected U16Field to disallow NULL field name", false);
	}
	catch (...) {
		test.check("Okay", true);
	}


	try {
		U16Field f3("", value);
		test.check("Expected U16Field to disallow empty-string field name", false);
	}
	catch (...) {
		test.check("Okay", true);
	}
}


void test_U32Field(Test& test)
{
	GMSEC_INFO << "Test U32Field...";

	std::string name = "U32-FIELD";
	GMSEC_U32 value = 5;

	U32Field f1(name.c_str(), value);

	test.check("Unexpected field name", name == f1.getName());
	test.check("Unexpected field type", Field::Type::U32 == f1.getType());
	test.check("Unexpected field value", value == f1.getValue());
	test.check("Unexpected XML string", std::string("<FIELD NAME=\"U32-FIELD\" TYPE=\"U32\">5</FIELD>") == f1.toXML());
	test.check("Unexpected JSON string", std::string("{\"NAME\":\"U32-FIELD\",\"TYPE\":\"U32\",\"VALUE\":\"5\"}") == f1.toJSON());


	GMSEC_INFO << "Test U32Field copy-constructor...";

	U32Field f2(f1);

	test.check("Unexpected field name", name == f2.getName());
	test.check("Unexpected field type", f1.getType() == f2.getType());
	test.check("Unexpected field value", f1.getValue() == f2.getValue());
	test.check("Unexpected XML string", std::string(f1.toXML()) == f2.toXML());
	test.check("Unexpected JSON string", std::string(f1.toJSON()) == f2.toJSON());


	GMSEC_INFO << "Test U32Field off-nominal cases...";

	try {
		U32Field f3(NULL, value);
		test.check("Expected U32Field to disallow NULL field name", false);
	}
	catch (...) {
		test.check("Okay", true);
	}


	try {
		U32Field f3("", value);
		test.check("Expected U32Field to disallow empty-string field name", false);
	}
	catch (...) {
		test.check("Okay", true);
	}
}


void test_U64Field(Test& test)
{
	GMSEC_INFO << "Test U64Field...";

	std::string name = "U64-FIELD";
	GMSEC_U64 value = 50;

	U64Field f1(name.c_str(), value);

	test.check("Unexpected field name", name == f1.getName());
	test.check("Unexpected field type", Field::Type::U64 == f1.getType());
	test.check("Unexpected field value", value == f1.getValue());
	test.check("Unexpected XML string", std::string("<FIELD NAME=\"U64-FIELD\" TYPE=\"U64\">50</FIELD>") == f1.toXML());
	test.check("Unexpected JSON string", std::string("{\"NAME\":\"U64-FIELD\",\"TYPE\":\"U64\",\"VALUE\":\"50\"}") == f1.toJSON());


	GMSEC_INFO << "Test U64Field copy-constructor...";

	U64Field f2(f1);

	test.check("Unexpected field name", name == f2.getName());
	test.check("Unexpected field type", f1.getType() == f2.getType());
	test.check("Unexpected field value", f1.getValue() == f2.getValue());
	test.check("Unexpected XML string", std::string(f1.toXML()) == f2.toXML());
	test.check("Unexpected JSON string", std::string(f1.toJSON()) == f2.toJSON());


	GMSEC_INFO << "Test U64Field off-nominal cases...";

	try {
		U64Field f3(NULL, value);
		test.check("Expected U64Field to disallow NULL field name", false);
	}
	catch (...) {
		test.check("Okay", true);
	}


	try {
		U64Field f3("", value);
		test.check("Expected U64Field to disallow empty-string field name", false);
	}
	catch (...) {
		test.check("Okay", true);
	}
}


void test_U8Field(Test& test)
{
	GMSEC_INFO << "Test U8Field...";

	std::string name = "U8-FIELD";
	GMSEC_U8 value = 5;

	U8Field f1(name.c_str(), value);

	test.check("Unexpected field name", name == f1.getName());
	test.check("Unexpected field type", Field::Type::U8 == f1.getType());
	test.check("Unexpected field value", value == f1.getValue());
	test.check("Unexpected XML string", std::string("<FIELD NAME=\"U8-FIELD\" TYPE=\"U8\">5</FIELD>") == f1.toXML());
	test.check("Unexpected JSON string", std::string("{\"NAME\":\"U8-FIELD\",\"TYPE\":\"U8\",\"VALUE\":\"5\"}") == f1.toJSON());


	GMSEC_INFO << "Test U8Field copy-constructor...";

	U8Field f2(f1);

	test.check("Unexpected field name", name == f2.getName());
	test.check("Unexpected field type", f1.getType() == f2.getType());
	test.check("Unexpected field value", f1.getValue() == f2.getValue());
	test.check("Unexpected XML string", std::string(f1.toXML()) == f2.toXML());
	test.check("Unexpected JSON string", std::string(f1.toJSON()) == f2.toJSON());


	GMSEC_INFO << "Test U8Field off-nominal cases...";

	try {
		U8Field f3(NULL, value);
		test.check("Expected U8Field to disallow NULL field name", false);
	}
	catch (...) {
		test.check("Okay", true);
	}


	try {
		U8Field f3("", value);
		test.check("Expected U8Field to disallow empty-string field name", false);
	}
	catch (...) {
		test.check("Okay", true);
	}
}


void test_StringField(Test& test)
{
	GMSEC_INFO << "Test StringField...";

	std::string name = "STRING-FIELD";
	std::string value = "Test String";

	StringField f1(name.c_str(), value.c_str());

	test.check("Unexpected field name", name == f1.getName());
	test.check("Unexpected field type", Field::Type::STRING == f1.getType());
	test.check("Unexpected field value", value == f1.getValue());
	test.check("Unexpected XML string", std::string("<FIELD NAME=\"STRING-FIELD\" TYPE=\"STRING\">Test String</FIELD>") == f1.toXML());
	test.check("Unexpected JSON string", std::string("{\"NAME\":\"STRING-FIELD\",\"TYPE\":\"STRING\",\"VALUE\":\"Test String\"}") == f1.toJSON());


	GMSEC_INFO << "Test StringField copy-constructor...";

	StringField f2(f1);

	test.check("Unexpected field name", name == f2.getName());
	test.check("Unexpected field type", f1.getType() == f2.getType());
	test.check("Unexpected field value", std::string(f1.getValue()) == f2.getValue());
	test.check("Unexpected XML string", std::string(f1.toXML()) == f2.toXML());
	test.check("Unexpected JSON string", std::string(f1.toJSON()) == f2.toJSON());


	GMSEC_INFO << "Test StringField off-nominal cases...";

	try {
		StringField f3(NULL, value.c_str());
		test.check("Expected StringField to disallow NULL field name", false);
	}
	catch (...) {
		test.check("Okay", true);
	}


	try {
		StringField f3("", value.c_str());
		test.check("Expected StringField to disallow empty-string field name", false);
	}
	catch (...) {
		test.check("Okay", true);
	}


	try {
		StringField f3(name.c_str(), NULL);
		test.check("Expected StringField to disallow NULL field value", false);
	}
	catch (...) {
		test.check("Okay", true);
	}
}


void test_Field(Test& test)
{
	//o getStringValue()
	{
		GMSEC_INFO << "Test Field::getStringField()";

		unsigned char blob[] = { 0x00, 0x02, 0x04, 0x08, 0xFF };
		size_t blobLen = sizeof(blob)/sizeof(unsigned char);

		CharField    field_01("CHAR-FIELD", 'c');
		BooleanField field_02("BOOL-FIELD", true);
		I16Field     field_03("I16-FIELD", static_cast<GMSEC_I16>(-1234));
		I32Field     field_04("I32-FIELD", -1234);
		I64Field     field_05("I64-FIELD", static_cast<GMSEC_I64>(-1234));
		I8Field      field_06("I8-FIELD", static_cast<GMSEC_I8>(-128));
		U16Field     field_07("U16-FIELD", static_cast<GMSEC_U16>(1234));
		U32Field     field_08("U32-FIELD", 1234);
		U64Field     field_09("U64-FIELD", static_cast<GMSEC_U64>(1234));
		U8Field      field_10("U8-FIELD", static_cast<GMSEC_U8>(127));
		F32Field     field_11("F32-FIELD", 1234.56f);
		F64Field     field_12("F64-FIELD", 1234.56);
		StringField  field_13("STR-FIELD", "Hello World");
		BinaryField  field_14("BIN-FIELD", blob, blobLen);

		test.check("01: Fail getStringValue()", strcompare(field_01.getStringValue(), "c") == 0);
		test.check("02: Fail getStringValue()", strcompare(field_02.getStringValue(), "true") == 0);
		test.check("03: Fail getStringValue()", strcompare(field_03.getStringValue(), "-1234") == 0);
		test.check("04: Fail getStringValue()", strcompare(field_04.getStringValue(), "-1234") == 0);
		test.check("05: Fail getStringValue()", strcompare(field_05.getStringValue(), "-1234") == 0);
		test.check("06: Fail getStringValue()", strcompare(field_06.getStringValue(), "-128") == 0);
		test.check("07: Fail getStringValue()", strcompare(field_07.getStringValue(), "1234") == 0);
		test.check("08: Fail getStringValue()", strcompare(field_08.getStringValue(), "1234") == 0);
		test.check("09: Fail getStringValue()", strcompare(field_09.getStringValue(), "1234") == 0);
		test.check("10: Fail getStringValue()", strcompare(field_10.getStringValue(), "127") == 0);
		test.check("11: Fail getStringValue()", strcompare(field_11.getStringValue(), "1234.56") == 0);
		test.check("12: Fail getStringValue()", strcompare(field_12.getStringValue(), "1234.56") == 0);
		test.check("13: Fail getStringValue()", strcompare(field_13.getStringValue(), "Hello World") == 0);
		test.check("14: Fail getStringValue()", strcompare(field_14.getStringValue(), "00020408FF") == 0);
	}

	//o getBooleanValue()
	{
		GMSEC_INFO << "Test Field::getBooleanField()";

		BooleanField field1("BOOL-FIELD", true);
		U32Field     field2("U32-FIELD1", 1234);
		I32Field     field3("I32-FIELD2", -1234);
		F32Field     field4("F32-FIELD", 0.0f);
		StringField  field5("STR-FIELD1", "FALSE");
		StringField  field6("STR-FIELD2", "true");
		StringField  field7("STR-FIELD3", "Hello World");

		test.check("Fail getBooleanValue 1", field1.getBooleanValue() == true);
		test.check("Fail getBooleanValue 2", field2.getBooleanValue() == true);
		test.check("Fail getBooleanValue 3", field3.getBooleanValue() == true);
		test.check("Fail getBooleanValue 4", field4.getBooleanValue() == false);
		test.check("Fail getBooleanValue 5", field5.getBooleanValue() == false);
		test.check("Fail getBooleanValue 6", field6.getBooleanValue() == true);

		try {
			field7.getBooleanValue();
			test.check("Expected boolean conversion exception", false);
		}
		catch (const GmsecException& e) {
			test.check(e.what(), std::string(e.what()).find("Field cannot be represented as a boolean") != std::string::npos);
		}
	}

	//o getI16Value()
	{
		GMSEC_INFO << "Test Field::getI16Field()";

		unsigned char blob1[] = { 0x7F, 0xFF };
		size_t blob1Len = sizeof(blob1)/sizeof(unsigned char);

		unsigned char blob2[] = { 0x01, 0x02, 0x03, 0x04 };
		size_t blob2Len = sizeof(blob2)/sizeof(unsigned char);

		I16Field    field1("I16-FIELD", -1234);
		U16Field    field2("U16-FIELD", 1234);
		F32Field    field3("F32-FIELD", 1234.56f);
		StringField field4("STR-FIELD", "Hello World");
		BinaryField field5("BIN1-FIELD", blob1, blob1Len);
		BinaryField field6("BIN2-FIELD", blob2, blob2Len);
		I32Field    field7("I32-FIELD", 32768);

		test.check("1: Fail getI16Value", -1234 == field1.getI16Value());
		test.check("2: Fail getI16Value", 1234 == field2.getI16Value());
		test.check("3: Fail getI16Value", 1234 == field3.getI16Value());

		try {
			field4.getI16Value();
			test.check("4: Fail getI16Value", false);
		}
		catch (const GmsecException& e) {
			test.check(e.what(), std::string(e.what()).find("Field cannot be converted to a GMSEC_I16") != std::string::npos);
		}

		test.check("5: Fail getI16Value", 32767 == field5.getI16Value());

		try {
			field6.getI16Value();
			test.check("6: Fail getI16Value", false);
		}
		catch (const GmsecException& e) {
			test.check(e.what(), std::string(e.what()).find("Field cannot be converted to a GMSEC_I16") != std::string::npos);
		}

		try {
			field7.getI16Value();
			test.check("7: Fail getI16Value", false);
		}
		catch (const GmsecException& e) {
			test.check(e.what(), std::string(e.what()).find("Field cannot be converted to a GMSEC_I16") != std::string::npos);
		}
	}

	//o getI32Value()
	{
		GMSEC_INFO << "Test Field::getI32Field()";

		unsigned char blob1[] = { 0x7F, 0xFF, 0xFF, 0xFF };
		size_t blob1Len = sizeof(blob1)/sizeof(unsigned char);

		unsigned char blob2[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
		size_t blob2Len = sizeof(blob2)/sizeof(unsigned char);

		I32Field    field1("I32-FIELD", -1234);
		U32Field    field2("U32-FIELD", 1234);
		F32Field    field3("F32-FIELD", 1234.56f);
		StringField field4("STR-FIELD", "Hello World");
		BinaryField field5("BIN1-FIELD", blob1, blob1Len);
		BinaryField field6("BIN2-FIELD", blob2, blob2Len);
		F64Field    field7("F64-FIELD", static_cast<GMSEC_F64>(2147483648));

		test.check("1: Fail getI32Value", -1234 == field1.getI32Value());
		test.check("2: Fail getI32Value", 1234 == field2.getI32Value());
		test.check("3: Fail getI32Value", 1234 == field3.getI32Value());

		try {
			field4.getI32Value();
			test.check("4: Fail getI32Value", false);
		}
		catch (const GmsecException& e) {
			test.check(e.what(), std::string(e.what()).find("Field cannot be converted to a GMSEC_I32") != std::string::npos);
		}

		test.check("5: Fail getI32Value", 2147483647 == field5.getI32Value());

		try {
			field6.getI32Value();
			test.check("6: Fail getI32Value", false);
		}
		catch (const GmsecException& e) {
			test.check(e.what(), std::string(e.what()).find("Field cannot be converted to a GMSEC_I32") != std::string::npos);
		}

		try {
			field7.getI32Value();
			test.check("7: Fail getI32Value", false);
		}
		catch (const GmsecException& e) {
			test.check(e.what(), std::string(e.what()).find("Field cannot be converted to a GMSEC_I32") != std::string::npos);
		}
	}

	//o getI64Value()
	{
		GMSEC_INFO << "Test Field::getI64Field()";

		unsigned char blob1[] = { 0x6F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
		size_t blob1Len = sizeof(blob1)/sizeof(unsigned char);

		unsigned char blob2[] = { 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
		size_t blob2Len = sizeof(blob2)/sizeof(unsigned char);

		I32Field    field1("I32-FIELD", -1234);
		U32Field    field2("U32-FIELD", 1234);
		F32Field    field3("F32-FIELD", 1234.56f);
		StringField field4("STR-FIELD", "Hello World");
		I64Field    field5("I64-FIELD", 1234);
		BinaryField field6("BIN1-FIELD", blob1, blob1Len);
		BinaryField field7("BIN2-FIELD", blob2, blob2Len);
		F64Field    field8("F64-FIELD", static_cast<GMSEC_F64>((std::numeric_limits<GMSEC_U64>::max)()) + 1);

		test.check("1: Fail getI64Value", -1234 == field1.getI64Value());
		test.check("2: Fail getI64Value", 1234 == field2.getI64Value());
		test.check("3: Fail getI64Value", 1234 == field3.getI64Value());

		try {
			field4.getI64Value();
			test.check("4: Fail getI64Value", false);
		}
		catch (const GmsecException& e) {
			test.check(e.what(), std::string(e.what()).find("Field cannot be converted to a GMSEC_I64") != std::string::npos);
		}

		test.check("5: Fail getI64Value", 1234 == field5.getI64Value());
		test.check("6: Fail getI64Value", static_cast<GMSEC_I64>(8070450532247928832) == field6.getI64Value());

		try {
			field7.getI64Value();
			test.check("7: Fail getI64Value", false);
		}
		catch (const GmsecException& e) {
			test.check(e.what(), std::string(e.what()).find("Field cannot be converted to a GMSEC_I64") != std::string::npos);
		}

		try {
			field8.getI64Value();
			test.check("8: Fail getI64Value", false);
		}
		catch (const GmsecException& e) {
			test.check(e.what(), std::string(e.what()).find("Field cannot be converted to a GMSEC_I64") != std::string::npos);
		}
	}

	//o getU16Value()
	{
		GMSEC_INFO << "Test Field::getU16Field()";

		unsigned char blob1[] = { 0xFF, 0xFF };
		size_t blob1Len = sizeof(blob1)/sizeof(unsigned char);

		unsigned char blob2[] = { 0x01, 0x02, 0x03, 0x04 };
		size_t blob2Len = sizeof(blob2)/sizeof(unsigned char);

		I16Field    field1("I16-FIELD", -10);
		U16Field    field2("U16-FIELD", 1234);
		F32Field    field3("F32-FIELD", 1234.56f);
		StringField field4("STR-FIELD", "Hello World");
		BinaryField field5("BIN1-FIELD", blob1, blob1Len);
		BinaryField field6("BIN2-FIELD", blob2, blob2Len);

		test.check("1: Fail getU16Value", static_cast<GMSEC_U16>(65526) == field1.getU16Value());
		test.check("2: Fail getU16Value", static_cast<GMSEC_U16>(1234) == field2.getU16Value());
		test.check("3: Fail getU16Value", static_cast<GMSEC_U16>(1234) == field3.getU16Value());

		try {
			field4.getU16Value();
			test.check("4: Fail getU16Value", false);
		}
		catch (const GmsecException& e) {
			test.check(e.what(), std::string(e.what()).find("Field cannot be converted to a GMSEC_U16") != std::string::npos);
		}

		test.check("5: Fail getU16Value", static_cast<GMSEC_U16>(65535) == field5.getU16Value());

		try {
			field6.getU16Value();
			test.check("6: Fail getU16Value", false);
		}
		catch (const GmsecException& e) {
			test.check(e.what(), std::string(e.what()).find("Field cannot be converted to a GMSEC_U16") != std::string::npos);
		}
	}

	//o getU32Value()
	{
		GMSEC_INFO << "Test Field::getU32Field()";

		unsigned char blob1[] = { 0xFF, 0xFF, 0xFF, 0xFF };
		size_t blob1Len = sizeof(blob1)/sizeof(unsigned char);

		unsigned char blob2[] = { 0x01, 0x02, 0x03, 0x04, 0x05 };
		size_t blob2Len = sizeof(blob2)/sizeof(unsigned char);

		I32Field    field1("I32-FIELD", -10);
		U32Field    field2("U32-FIELD", 1234);
		F32Field    field3("F32-FIELD", 1234.56f);
		StringField field4("STR-FIELD", "Hello World");
		BinaryField field5("BIN1-FIELD", blob1, blob1Len);
		BinaryField field6("BIN2-FIELD", blob2, blob2Len);

		test.check("1: Fail getU32Value", static_cast<GMSEC_U32>(4294967286) == field1.getU32Value());
		test.check("2: Fail getU32Value", static_cast<GMSEC_U32>(1234) == field2.getU32Value());
		test.check("3: Fail getU32Value", static_cast<GMSEC_U32>(1234) == field3.getU32Value());

		try {
			field4.getU32Value();
			test.check("4: Fail getU32Value", false);
		}
		catch (const GmsecException& e) {
			test.check(e.what(), std::string(e.what()).find("Field cannot be converted to a GMSEC_U32") != std::string::npos);
		}

		test.check("5: Fail getU32Value", static_cast<GMSEC_U32>(4294967295) == field5.getU32Value());

		try {
			field6.getU32Value();
			test.check("6: Fail getU32Value", false);
		}
		catch (const GmsecException& e) {
			test.check(e.what(), std::string(e.what()).find("Field cannot be converted to a GMSEC_U32") != std::string::npos);
		}
	}

	//o getU64Value()
	{
		GMSEC_INFO << "Test Field::getU64Field()";

		unsigned char blob1[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
		size_t blob1Len = sizeof(blob1)/sizeof(unsigned char);

		unsigned char blob2[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
		size_t blob2Len = sizeof(blob2)/sizeof(unsigned char);

		I32Field    field1("I32-FIELD", -10);
		U32Field    field2("U32-FIELD", 1234);
		F32Field    field3("F32-FIELD", 1234.56f);
		StringField field4("STR-FIELD", "Hello World");
		U64Field    field5("U64-FIELD", 1234);
		BinaryField field6("BIN1-FIELD", blob1, blob1Len);
		BinaryField field7("BIN2-FIELD", blob2, blob2Len);

		test.check("1: Fail getU64Value", static_cast<GMSEC_U64>(18446744073709551606UL) == field1.getU64Value());
		test.check("2: Fail getU64Value", static_cast<GMSEC_U64>(1234) == field2.getU64Value());
		test.check("3: Fail getU64Value", static_cast<GMSEC_U64>(1234) == field3.getU64Value());

		try {
			field4.getU64Value();
			test.check("4: Fail getU64Value", false);
		}
		catch (const GmsecException& e) {
			test.check(e.what(), std::string(e.what()).find("Field cannot be converted to a GMSEC_U64") != std::string::npos);
		}

		test.check("5: Fail getU64Value", 1234 == field5.getU64Value());
		test.check("6: Fail getU64Value", static_cast<GMSEC_U64>(18446744073709551615ULL) == field6.getU64Value());

		try {
			field7.getU64Value();
			test.check("7: Fail getU64Value", false);
		}
		catch (const GmsecException& e) {
			test.check(e.what(), std::string(e.what()).find("Field cannot be converted to a GMSEC_U64") != std::string::npos);
		}
	}

	//o getF64Value()
	{
		GMSEC_INFO << "Test Field::getF64Field()";

		unsigned char blob1[] = { 0x40, 0x93, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00 };
		size_t blob1Len = sizeof(blob1)/sizeof(unsigned char);

		unsigned char blob2[] = { 0x40, 0x93, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF };
		size_t blob2Len = sizeof(blob2)/sizeof(unsigned char);

		CharField    field_01("CHAR-FIELD", 'c');
		BooleanField field_02("BOOL-FIELD", true);
		I16Field     field_03("I16-FIELD", static_cast<GMSEC_I16>(-1234));
		I32Field     field_04("I32-FIELD", -1234);
		I64Field     field_05("I64-FIELD", static_cast<GMSEC_I64>(-1234));
		I8Field      field_06("I8-FIELD", static_cast<GMSEC_I8>(-128));
		U16Field     field_07("U16-FIELD", static_cast<GMSEC_U16>(1234));
		U32Field     field_08("U32-FIELD", 1234);
		U64Field     field_09("U64-FIELD", static_cast<GMSEC_U64>(1234));
		U8Field      field_10("U8-FIELD", static_cast<GMSEC_U8>(127));
		F32Field     field_11("F32-FIELD", 1234.56f);
		F64Field     field_12("F64-FIELD", 1234.56);
		StringField  field_13("STR-FIELD", "1234");
		BinaryField  field_14("BIN-FIELD", blob1, blob1Len);
		StringField  field_15("BAD-FIELD", "Hello World");
		BinaryField  field_16("BAD-FIELD", blob2, blob2Len);

		test.check("01: Fail getF64Value()", field_01.getF64Value() == 99);   // from ASCII table, 'c' == 99 (dec)
		test.check("02: Fail getF64Value()", field_02.getF64Value() == 1);
		test.check("03: Fail getF64Value()", field_03.getF64Value() == static_cast<GMSEC_F64>(-1234));
		test.check("04: Fail getF64Value()", field_04.getF64Value() == static_cast<GMSEC_F64>(-1234));
		test.check("05: Fail getF64Value()", field_05.getF64Value() == static_cast<GMSEC_F64>(-1234));
		test.check("06: Fail getF64Value()", field_06.getF64Value() == static_cast<GMSEC_F64>(-128));
		test.check("07: Fail getF64Value()", field_07.getF64Value() == static_cast<GMSEC_F64>(1234));
		test.check("08: Fail getF64Value()", field_08.getF64Value() == static_cast<GMSEC_F64>(1234));
		test.check("09: Fail getF64Value()", field_09.getF64Value() == static_cast<GMSEC_F64>(1234));
		test.check("10: Fail getF64Value()", field_10.getF64Value() == static_cast<GMSEC_F64>(127));
		test.check("11: Fail getF64Value()", field_11.getF64Value() == static_cast<GMSEC_F64>(1234.56));
		test.check("12: Fail getF64Value()", field_12.getF64Value() == static_cast<GMSEC_F64>(1234.56));
		test.check("13: Fail getF64Value()", field_13.getF64Value() == static_cast<GMSEC_F64>(1234));
		test.check("14: Fail getF64Value()", field_14.getF64Value() == static_cast<GMSEC_F64>(1234));

		try {
			field_15.getF64Value();
			test.check("15: Fail getF64Value", false);
		}
		catch (const GmsecException& e) {
			test.check(e.what(), std::string(e.what()).find("Field cannot be converted to a GMSEC_F64") != std::string::npos);
		}

		try {
			field_16.getF64Value();
			test.check("16: Fail getF64Value", false);
		}
		catch (const GmsecException& e) {
			test.check(e.what(), std::string(e.what()).find("Field cannot be converted to a GMSEC_F64") != std::string::npos);
		}
	}

	// isHeader()
	{
		GMSEC_INFO << "Test Field::isHeader()";

		StringField field("MY-FIELD", "MY-VALUE", true);
		test.check("Expected isHeader to report true", field.isHeader());
	}

	// invalid field name
	{
		GMSEC_INFO << "Test Field::isFieldNameCompliant()";

		const char* invalid_name[4] = { NULL, "", "FOO%BAR", "foobar" };

		for (size_t i = 0; i < sizeof(invalid_name)/sizeof(const char*); ++i)
		{
			try {
				StringField(invalid_name[i], "Hello World");
				test.check("An error was expected", false);
			}
			catch (const GmsecException& e) {
				test.check(e.what(), true);
			}
		}
	}
}


void test_IdentifyHeaderFields(Test& test)
{
	GMSEC_INFO << "Test Identify Header Fields...";

	{
		StringField field1("MISSION-ID", "MSSN", true);              // a header field
		U32Field    field2("PUB-RATE", static_cast<GMSEC_U32>(30));  // not a header field

		test.check("Expected no HEAD", std::string(field1.toXML()).find("HEAD") == std::string::npos);
		test.check("Expected no HEAD", std::string(field2.toXML()).find("HEAD") == std::string::npos);

		test.check("Expected no HEAD", std::string(field1.toJSON()).find("HEAD") == std::string::npos);
		test.check("Expected no HEAD", std::string(field2.toJSON()).find("HEAD") == std::string::npos);
	}

	{
		StringField field1("MISSION-ID", "MSSN", true);              // a header field
		U32Field    field2("PUB-RATE", static_cast<GMSEC_U32>(30));  // not a header field

		Config config;
		config.addValue("GMSEC-IDENTIFY-HEADER-FIELDS", "false");

		test.check("Expected no HEAD", std::string(field1.toXML()).find("HEAD") == std::string::npos);
		test.check("Expected no HEAD", std::string(field2.toXML()).find("HEAD") == std::string::npos);

		test.check("Expected no HEAD", std::string(field1.toJSON()).find("HEAD") == std::string::npos);
		test.check("Expected no HEAD", std::string(field2.toJSON()).find("HEAD") == std::string::npos);
	}

	{
		StringField field1("MISSION-ID", "MSSN", true);              // a header field
		U32Field    field2("PUB-RATE", static_cast<GMSEC_U32>(30));  // not a header field

		Config config;
		config.addValue("GMSEC-IDENTIFY-HEADER-FIELDS", "true");

		test.check("Expected HEAD", std::string(field1.toXML()).find("HEAD") != std::string::npos);
		test.check("Expected no HEAD", std::string(field2.toXML()).find("HEAD") == std::string::npos);

		test.check("Expected HEAD", std::string(field1.toJSON()).find("HEAD") != std::string::npos);
		test.check("Expected no HEAD", std::string(field2.toJSON()).find("HEAD") == std::string::npos);
	}
}


int test_Fields(Test& test)
{
	test.setDescription("Test Fields");

	test_BinaryField(test);
	test_BooleanField(test);
	test_CharField(test);
	test_F32Field(test);
	test_F64Field(test);
	test_I16Field(test);
	test_I32Field(test);
	test_I64Field(test);
	test_I8Field(test);
	test_U16Field(test);
	test_U32Field(test);
	test_U64Field(test);
	test_U8Field(test);
	test_StringField(test);
	test_Field(test);
	test_IdentifyHeaderFields(test);

	return 0;
}

TEST_DRIVER(test_Fields)
