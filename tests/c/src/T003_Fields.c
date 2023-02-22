#include "TestCase.h"


void test_BinaryField()
{
	GMSEC_INFO("Test BinaryField");

	const char* name = "BINARY-FIELD";
	GMSEC_U8    data[128];
	size_t      i;

	for (i = 0; i < sizeof(data); ++i)
	{
		data[i] = (unsigned char)(i % 256);
	}

	GMSEC_Status status = statusCreate();
	GMSEC_Field  field  = binaryFieldCreate(name, data, sizeof(data), GMSEC_TRUE, status);

	GMSEC_U8 const* f1_blob = binaryFieldGetValue(field, NULL);
	size_t          f1_blobLen = binaryFieldGetLength(field, NULL);

	testCheckBool("Failed to create field", statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Unexpected field name", strcompare(fieldGetName(field, NULL), name) == 0);
	testCheckBool("Unexpected field type", fieldGetType(field, NULL) == GMSEC_BINARY_TYPE);
	testCheckBool("Unexpected blob length", f1_blobLen == 128);
	for (i = 0; i < f1_blobLen; ++i)
	{
		GMSEC_BOOL same = (data[i] == f1_blob[i] ? GMSEC_TRUE : GMSEC_FALSE);

		if (same == GMSEC_FALSE)
		{
			char msg[256];
			printToString(msg, "Blob data not the same at position: %d", i);
			testCheckBool(msg, GMSEC_FALSE);
		}
	}
	testCheckBool("Expected header field", fieldIsHeader(field, NULL) == GMSEC_TRUE);

	fieldDestroy(&field);

	// Off-nominal test(s)
	(void) binaryFieldCreate(NULL, data, sizeof(data), GMSEC_FALSE, status);
	testCheckBool("Expected an error creating BinaryField", statusHasError(status) == GMSEC_TRUE);

	(void) binaryFieldGetValue(NULL, status);
	testCheckBool("Expected an error attempting to get BinaryField value", statusHasError(status) == GMSEC_TRUE);
	(void) binaryFieldGetLength(NULL, status);
	testCheckBool("Expected an error attempting to get BinaryField length", statusHasError(status) == GMSEC_TRUE);

	statusDestroy(&status);
}


void test_BooleanField()
{
	GMSEC_INFO("Test BooleanField");

	const char* name = "BOOLEAN-FIELD";
	GMSEC_BOOL value = GMSEC_TRUE;

	GMSEC_Status status = statusCreate();
	GMSEC_Field  field  = booleanFieldCreate(name, value, GMSEC_TRUE, status);

	testCheckBool("Failed to create field", statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Unexpected field name", strcompare(fieldGetName(field, NULL), name) == 0);
	testCheckBool("Unexpected field type", fieldGetType(field, NULL) == GMSEC_BOOL_TYPE);
	testCheckBool("Unexpected field value", booleanFieldGetValue(field, NULL) == value);
	testCheckBool("Expected header field", fieldIsHeader(field, NULL) == GMSEC_TRUE);

	fieldDestroy(&field);

	// Off-nominal test(s)
	(void) booleanFieldCreate(NULL, value, GMSEC_FALSE, status);
	testCheckBool("Expected an error creating BooleanField", statusHasError(status) == GMSEC_TRUE);

	(void) booleanFieldGetValue(NULL, status);
	testCheckBool("Expected an error attempting to get BooleanField value", statusHasError(status) == GMSEC_TRUE);

	statusDestroy(&status);
}


void test_CharField()
{
	GMSEC_INFO("Test CharField");

	const char* name = "CHAR-FIELD";
	char        value = 'c';

	GMSEC_Status status = statusCreate();
	GMSEC_Field  field  = charFieldCreate(name, value, GMSEC_TRUE, status);

	testCheckBool("Failed to create field", statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Unexpected field name", strcompare(fieldGetName(field, NULL), name) == 0);
	testCheckBool("Unexpected field type", fieldGetType(field, NULL) == GMSEC_CHAR_TYPE);
	testCheckBool("Unexpected field value", charFieldGetValue(field, NULL) == value);
	testCheckBool("Expected header field", fieldIsHeader(field, NULL) == GMSEC_TRUE);

	fieldDestroy(&field);

	// Off-nominal test(s)
	(void) charFieldCreate(NULL, value, GMSEC_FALSE, status);
	testCheckBool("Expected an error creating CharField", statusHasError(status) == GMSEC_TRUE);

	(void) charFieldGetValue(NULL, status);
	testCheckBool("Expected an error attempting to get CharField value", statusHasError(status) == GMSEC_TRUE);

	statusDestroy(&status);
}


void test_F32Field()
{
	GMSEC_INFO("Test F32Field");

	const char* name  = "F32-FIELD";
	GMSEC_F32   value = 3.14f;

	GMSEC_Status status = statusCreate();
	GMSEC_Field  field  = f32FieldCreate(name, value, GMSEC_FALSE, status);

	testCheckBool("Failed to create field", statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Unexpected field name", strcompare(fieldGetName(field, NULL), name) == 0);
	testCheckBool("Unexpected field type", fieldGetType(field, NULL) == GMSEC_F32_TYPE);
	testCheckBool("Unexpected field value", f32FieldGetValue(field, NULL) == value);
	testCheckBool("Expected header field", fieldIsHeader(field, NULL) == GMSEC_FALSE);

	fieldDestroy(&field);

	// Off-nominal test(s)
	(void) f32FieldCreate(NULL, value, GMSEC_FALSE, status);
	testCheckBool("Expected an error creating F32Field", statusHasError(status) == GMSEC_TRUE);

	(void) f32FieldGetValue(NULL, status);
	testCheckBool("Expected an error attempting to get F32Field value", statusHasError(status) == GMSEC_TRUE);

	statusDestroy(&status);
}


void test_F64Field()
{
	GMSEC_INFO("Test F64Field");

	const char* name  = "F64-FIELD";
	GMSEC_F64   value = 3.14;

	GMSEC_Status status = statusCreate();
	GMSEC_Field  field  = f64FieldCreate(name, value, GMSEC_FALSE, status);

	testCheckBool("Failed to create field", statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Unexpected field name", strcompare(fieldGetName(field, NULL), name) == 0);
	testCheckBool("Unexpected field type", fieldGetType(field, NULL) == GMSEC_F64_TYPE);
	testCheckBool("Unexpected field value", f64FieldGetValue(field, NULL) == value);
	testCheckBool("Expected header field", fieldIsHeader(field, NULL) == GMSEC_FALSE);

	fieldDestroy(&field);

	// Off-nominal test(s)
	(void) f64FieldCreate(NULL, value, GMSEC_FALSE, status);
	testCheckBool("Expected an error creating F64Field", statusHasError(status) == GMSEC_TRUE);

	(void) f64FieldGetValue(NULL, status);
	testCheckBool("Expected an error attempting to get F64Field value", statusHasError(status) == GMSEC_TRUE);

	statusDestroy(&status);
}


void test_I16Field()
{
	GMSEC_INFO("Test I16Field");

	const char* name  = "I16-FIELD";
	GMSEC_I16   value = 16;

	GMSEC_Status status = statusCreate();
	GMSEC_Field  field  = i16FieldCreate(name, value, GMSEC_FALSE, status);

	testCheckBool("Failed to create field", statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Unexpected field name", strcompare(fieldGetName(field, NULL), name) == 0);
	testCheckBool("Unexpected field type", fieldGetType(field, NULL) == GMSEC_I16_TYPE);
	testCheckBool("Unexpected field value", i16FieldGetValue(field, NULL) == value);
	testCheckBool("Expected header field", fieldIsHeader(field, NULL) == GMSEC_FALSE);

	fieldDestroy(&field);

	// Off-nominal test(s)
	(void) i16FieldCreate(NULL, value, GMSEC_FALSE, status);
	testCheckBool("Expected an error creating I16Field", statusHasError(status) == GMSEC_TRUE);

	(void) i16FieldGetValue(NULL, status);
	testCheckBool("Expected an error attempting to get I16Field value", statusHasError(status) == GMSEC_TRUE);

	statusDestroy(&status);
}


void test_I32Field()
{
	GMSEC_INFO("Test I32Field");

	const char* name  = "I32-FIELD";
	GMSEC_I32   value = 32;

	GMSEC_Status status = statusCreate();
	GMSEC_Field  field  = i32FieldCreate(name, value, GMSEC_FALSE, status);

	testCheckBool("Failed to create field", statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Unexpected field name", strcompare(fieldGetName(field, NULL), name) == 0);
	testCheckBool("Unexpected field type", fieldGetType(field, NULL) == GMSEC_I32_TYPE);
	testCheckBool("Unexpected field value", i32FieldGetValue(field, NULL) == value);
	testCheckBool("Expected header field", fieldIsHeader(field, NULL) == GMSEC_FALSE);

	fieldDestroy(&field);

	// Off-nominal test(s)
	(void) i32FieldCreate(NULL, value, GMSEC_FALSE, status);
	testCheckBool("Expected an error creating I32Field", statusHasError(status) == GMSEC_TRUE);

	(void) i32FieldGetValue(NULL, status);
	testCheckBool("Expected an error attempting to get I32Field value", statusHasError(status) == GMSEC_TRUE);

	statusDestroy(&status);
}


void test_I64Field()
{
	GMSEC_INFO("Test I64Field");

	const char* name  = "I64-FIELD";
	GMSEC_I64   value = 64;

	GMSEC_Status status = statusCreate();
	GMSEC_Field  field  = i64FieldCreate(name, value, GMSEC_FALSE, status);

	testCheckBool("Failed to create field", statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Unexpected field name", strcompare(fieldGetName(field, NULL), name) == 0);
	testCheckBool("Unexpected field type", fieldGetType(field, NULL) == GMSEC_I64_TYPE);
	testCheckBool("Unexpected field value", i64FieldGetValue(field, NULL) == value);
	testCheckBool("Expected header field", fieldIsHeader(field, NULL) == GMSEC_FALSE);

	fieldDestroy(&field);

	// Off-nominal test(s)
	(void) i64FieldCreate(NULL, value, GMSEC_FALSE, status);
	testCheckBool("Expected an error creating I64Field", statusHasError(status) == GMSEC_TRUE);

	(void) i64FieldGetValue(NULL, status);
	testCheckBool("Expected an error attempting to get I64Field value", statusHasError(status) == GMSEC_TRUE);

	statusDestroy(&status);
}


void test_I8Field()
{
	GMSEC_INFO("Test I8Field");

	const char* name  = "I8-FIELD";
	GMSEC_I8    value = 8;

	GMSEC_Status status = statusCreate();
	GMSEC_Field  field  = i8FieldCreate(name, value, GMSEC_FALSE, status);

	testCheckBool("Failed to create field", statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Unexpected field name", strcompare(fieldGetName(field, NULL), name) == 0);
	testCheckBool("Unexpected field type", fieldGetType(field, NULL) == GMSEC_I8_TYPE);
	testCheckBool("Unexpected field value", i8FieldGetValue(field, NULL) == value);
	testCheckBool("Expected header field", fieldIsHeader(field, NULL) == GMSEC_FALSE);

	fieldDestroy(&field);

	// Off-nominal test(s)
	(void) i8FieldCreate(NULL, value, GMSEC_FALSE, status);
	testCheckBool("Expected an error creating I8Field", statusHasError(status) == GMSEC_TRUE);

	(void) i8FieldGetValue(NULL, status);
	testCheckBool("Expected an error attempting to get I8Field value", statusHasError(status) == GMSEC_TRUE);

	statusDestroy(&status);
}


void test_U16Field()
{
	GMSEC_INFO("Test U16Field");

	const char* name  = "U16-FIELD";
	GMSEC_U16   value = 16;

	GMSEC_Status status = statusCreate();
	GMSEC_Field  field  = u16FieldCreate(name, value, GMSEC_FALSE, status);

	testCheckBool("Failed to create field", statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Unexpected field name", strcompare(fieldGetName(field, NULL), name) == 0);
	testCheckBool("Unexpected field type", fieldGetType(field, NULL) == GMSEC_U16_TYPE);
	testCheckBool("Unexpected field value", u16FieldGetValue(field, NULL) == value);
	testCheckBool("Expected header field", fieldIsHeader(field, NULL) == GMSEC_FALSE);

	fieldDestroy(&field);

	// Off-nominal test(s)
	(void) u16FieldCreate(NULL, value, GMSEC_FALSE, status);
	testCheckBool("Expected an error creating U16Field", statusHasError(status) == GMSEC_TRUE);

	(void) u16FieldGetValue(NULL, status);
	testCheckBool("Expected an error attempting to get U16Field value", statusHasError(status) == GMSEC_TRUE);

	statusDestroy(&status);
}


void test_U32Field()
{
	GMSEC_INFO("Test U32Field");

	const char* name  = "U32-FIELD";
	GMSEC_U32   value = 32;

	GMSEC_Status status = statusCreate();
	GMSEC_Field  field  = u32FieldCreate(name, value, GMSEC_FALSE, status);

	testCheckBool("Failed to create field", statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Unexpected field name", strcompare(fieldGetName(field, NULL), name) == 0);
	testCheckBool("Unexpected field type", fieldGetType(field, NULL) == GMSEC_U32_TYPE);
	testCheckBool("Unexpected field value", u32FieldGetValue(field, NULL) == value);
	testCheckBool("Expected header field", fieldIsHeader(field, NULL) == GMSEC_FALSE);

	fieldDestroy(&field);

	// Off-nominal test(s)
	(void) u32FieldCreate(NULL, value, GMSEC_FALSE, status);
	testCheckBool("Expected an error creating U32Field", statusHasError(status) == GMSEC_TRUE);

	(void) u32FieldGetValue(NULL, status);
	testCheckBool("Expected an error attempting to get U32Field value", statusHasError(status) == GMSEC_TRUE);

	statusDestroy(&status);
}


void test_U64Field()
{
	GMSEC_INFO("Test U64Field");

	const char* name  = "U64-FIELD";
	GMSEC_U64   value = 64;

	GMSEC_Status status = statusCreate();
	GMSEC_Field  field  = u64FieldCreate(name, value, GMSEC_FALSE, status);

	testCheckBool("Failed to create field", statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Unexpected field name", strcompare(fieldGetName(field, NULL), name) == 0);
	testCheckBool("Unexpected field type", fieldGetType(field, NULL) == GMSEC_U64_TYPE);
	testCheckBool("Unexpected field value", u64FieldGetValue(field, NULL) == value);
	testCheckBool("Expected header field", fieldIsHeader(field, NULL) == GMSEC_FALSE);

	fieldDestroy(&field);

	// Off-nominal test(s)
	(void) u64FieldCreate(NULL, value, GMSEC_FALSE, status);
	testCheckBool("Expected an error creating U64Field", statusHasError(status) == GMSEC_TRUE);

	(void) u64FieldGetValue(NULL, status);
	testCheckBool("Expected an error attempting to get U64Field value", statusHasError(status) == GMSEC_TRUE);

	statusDestroy(&status);
}


void test_U8Field()
{
	GMSEC_INFO("Test U8Field");

	const char* name  = "U8-FIELD";
	GMSEC_U8    value = 8;

	GMSEC_Status status = statusCreate();
	GMSEC_Field  field  = u8FieldCreate(name, value, GMSEC_FALSE, status);

	testCheckBool("Failed to create field", statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Unexpected field name", strcompare(fieldGetName(field, NULL), name) == 0);
	testCheckBool("Unexpected field type", fieldGetType(field, NULL) == GMSEC_U8_TYPE);
	testCheckBool("Unexpected field value", u8FieldGetValue(field, NULL) == value);
	testCheckBool("Expected header field", fieldIsHeader(field, NULL) == GMSEC_FALSE);

	fieldDestroy(&field);

	// Off-nominal test(s)
	(void) u8FieldCreate(NULL, value, GMSEC_FALSE, status);
	testCheckBool("Expected an error creating U8Field", statusHasError(status) == GMSEC_TRUE);

	(void) u8FieldGetValue(NULL, status);
	testCheckBool("Expected an error attempting to get U8Field value", statusHasError(status) == GMSEC_TRUE);

	statusDestroy(&status);
}


void test_StringField()
{
	GMSEC_INFO("Test StringField");

	const char* name  = "STRING-FIELD";
	const char* value = "Hello World";

	GMSEC_Status status = statusCreate();
	GMSEC_Field  field  = stringFieldCreate(name, value, GMSEC_TRUE, status);

	testCheckBool("Failed to create field", statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Unexpected field name", strcompare(fieldGetName(field, NULL), name) == 0);
	testCheckBool("Unexpected field type", fieldGetType(field, NULL) == GMSEC_STRING_TYPE);
	testCheckBool("Unexpected field value", strcompare(stringFieldGetValue(field, NULL), value) == 0);
	testCheckBool("Expected header field", fieldIsHeader(field, NULL) == GMSEC_TRUE);

	fieldDestroy(&field);

	// Off-nominal test(s)
	(void) stringFieldCreate(NULL, value, GMSEC_FALSE, status);
	testCheckBool("Expected an error creating StringField", statusHasError(status) == GMSEC_TRUE);

	(void) stringFieldGetValue(NULL, status);
	testCheckBool("Expected an error attempting to get StringField value", statusHasError(status) == GMSEC_TRUE);

	statusDestroy(&status);
}


void test_Field()
{
	GMSEC_Status status = statusCreate();
	GMSEC_Field  field1 = stringFieldCreate("STRING-FIELD", "Hello World", GMSEC_FALSE, NULL);
	GMSEC_Field  field2 = i32FieldCreate("I32-FIELD", 10, GMSEC_FALSE, NULL);
	GMSEC_Field  field3 = booleanFieldCreate("BOOL-FIELD", GMSEC_FALSE, GMSEC_FALSE, NULL);

	GMSEC_INFO("Test fieldToXML()");
	const char* xml = "<FIELD NAME=\"STRING-FIELD\" TYPE=\"STRING\">Hello World</FIELD>";
	testCheckBool("Unexpected XML content", strcompare(fieldToXML(field1, NULL), xml) == 0);

	GMSEC_INFO("Test fieldToJSON()");
	const char* json = "{\"NAME\":\"STRING-FIELD\",\"TYPE\":\"STRING\",\"VALUE\":\"Hello World\"}";
	testCheckBool("Unexpected JSON content", strcompare(fieldToJSON(field1, NULL), json) == 0);

	GMSEC_INFO("Test fieldGetStringValue()");
	testCheckBool("Unexpected string value", strcompare(fieldGetStringValue(field2, NULL), "10") == 0);

	GMSEC_INFO("Test fieldGetBooleanValue()");
	testCheckBool("Unexpected boolean value", fieldGetBooleanValue(field2, NULL) == GMSEC_TRUE);
	testCheckBool("Unexpected boolean value", fieldGetBooleanValue(field3, NULL) == GMSEC_FALSE);
	fieldGetBooleanValue(field1, status);
	testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

	GMSEC_INFO("Test fieldGetI16Value()");
	testCheckBool("Unexpected I16 value", fieldGetI16Value(field2, NULL) == (GMSEC_I16) 10);
	fieldGetI16Value(field1, status);
	testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

	GMSEC_INFO("Test fieldGetI32Value()");
	testCheckBool("Unexpected I32 value", fieldGetI32Value(field2, NULL) == (GMSEC_I32) 10);
	fieldGetI32Value(field1, status);
	testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

	GMSEC_INFO("Test fieldGetU16Value()");
	testCheckBool("Unexpected U16 value", fieldGetU16Value(field2, NULL) == (GMSEC_U16) 10);
	fieldGetU16Value(field1, status);
	testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

	GMSEC_INFO("Test fieldGetU32Value()");
	testCheckBool("Unexpected U32 value", fieldGetU32Value(field2, NULL) == (GMSEC_U32) 10);
	fieldGetU32Value(field1, status);
	testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

	GMSEC_INFO("Test fieldGetF64Value()");
	testCheckBool("Unexpected F64 value", fieldGetF64Value(field2, NULL) == (GMSEC_F64) 10);
	fieldGetF64Value(field1, status);
	testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

	GMSEC_INFO("Test fieldDestroy()");
	fieldDestroy(&field1);
	testCheckBool("Destroyed field should be NULL", field1 == NULL);

	fieldDestroy(&field2);
	fieldDestroy(&field3);

	// Off-nominal test(s)
	(void) stringFieldCreate("BAD%NAME", "FOO-BAR", GMSEC_FALSE, status);
	testCheckBool("Expected error due to illegal field name", statusHasError(status) == GMSEC_TRUE);

	(void) fieldGetName(NULL, status);
	testCheckBool("Expected error when attempting to get field name", statusHasError(status) == GMSEC_TRUE);

	(void) fieldGetType(NULL, status);
	testCheckBool("Expected error when attempting to get field type", statusHasError(status) == GMSEC_TRUE);

	(void) fieldToXML(NULL, status);
	testCheckBool("Expected error when attempting to get field XML", statusHasError(status) == GMSEC_TRUE);

	(void) fieldToJSON(NULL, status);
	testCheckBool("Expected error when attempting to get field JSON", statusHasError(status) == GMSEC_TRUE);

	(void) fieldGetStringValue(NULL, status);
	testCheckBool("Expected error when attempting to get field string value", statusHasError(status) == GMSEC_TRUE);

	(void) fieldGetBooleanValue(NULL, status);
	testCheckBool("Expected error when attempting to get field boolean value", statusHasError(status) == GMSEC_TRUE);

	(void) fieldGetI16Value(NULL, status);
	testCheckBool("Expected error when attempting to get field I16 value", statusHasError(status) == GMSEC_TRUE);

	(void) fieldGetI32Value(NULL, status);
	testCheckBool("Expected error when attempting to get field I32 value", statusHasError(status) == GMSEC_TRUE);

	(void) fieldGetU16Value(NULL, status);
	testCheckBool("Expected error when attempting to get field U16 value", statusHasError(status) == GMSEC_TRUE);

	(void) fieldGetU32Value(NULL, status);
	testCheckBool("Expected error when attempting to get field U32 value", statusHasError(status) == GMSEC_TRUE);

	(void) fieldGetF64Value(NULL, status);
	testCheckBool("Expected error when attempting to get field F64 value", statusHasError(status) == GMSEC_TRUE);

	statusDestroy(&status);
}


int test_Fields()
{
	testSetDescription("Test Fields");

	test_BinaryField();
	test_BooleanField();
	test_CharField();
	test_F32Field();
	test_F64Field();
	test_I16Field();
	test_I32Field();
	test_I64Field();
	test_I8Field();
	test_U16Field();
	test_U32Field();
	test_U64Field();
	test_U8Field();
	test_StringField();
	test_Field();

	return 0;
}
TEST_DRIVER(test_Fields)
