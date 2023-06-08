#!/usr/bin/env node

const TestCase = require('./TestCase.js');
const gmsec = require('../../../bin/gmsec_nodejs.node');


class T003_Fields extends TestCase
{
	constructor()
	{
		super();
	}

	testCase()
	{
		this.testBinaryField();
		this.testBooleanField();
		this.testCharField();
		this.testStringField();
		this.testF32Field();
		this.testF64Field();
		this.testI8Field();
		this.testI16Field();
		this.testI32Field();
		this.testI64Field();
		this.testU8Field();
		this.testU16Field();
		this.testU32Field();
		this.testU64Field();
		this.testField();
	}

	testBinaryField()
	{
		gmsec.Log.info("Test BinaryField");

		let name  = "MY-FIELD";
		let value = new gmsec.BlobArray();
		for (let i = 0; i < 4; ++i) {
			value.add(i);
		}

		// Constructor
		let field1 = new gmsec.BinaryField(name, value);
		this.check("Unexpected field type", field1.getType() == gmsec.Field.Type_BINARY);
		this.check("Unexpected field name", field1.getName() == name);
		this.check("Unexpected field length", field1.getLength() == value.size());
		this.check("Unexpected XML", field1.toXML() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"BIN\">00010203</FIELD>");
		this.check("Unexpected JSON", field1.toJSON() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"BIN\",\"VALUE\":\"00010203\"}");
		this.check("Unexpected isHeader value", field1.isHeader() == false);

		let data1 = field1.getValue();
		for (let i = 0; i < data1.size(); i++) {
			if (value.get(i) != data1.get(i)) {
				this.check("BinaryField value at position " + i.toString() + " is wrong", false);
			}
		}

		// Set as header field
		let field2 = new gmsec.BinaryField(name, value, true);
		this.check("Unexpected field type", field2.getType() == gmsec.Field.Type_BINARY);
		this.check("Unexpected field name", field2.getName() == name);
		this.check("Unexpected field length", field2.getLength() == value.size());
		this.check("Unexpected isHeader value", field2.isHeader());

		let data2 = field2.getValue();
		for (let i = 0; i < data2.size(); i++) {
			this.check("BinaryField value at position " + i.toString() + " is wrong", value.get(i) == data2.get(i));
		}

		// Copy-constructor
		let field3 = new gmsec.BinaryField(field1);
		this.check("Unexpected field type", field1.getType() == field3.getType());
		this.check("Unexpected field name", field1.getName() == field3.getName());
		this.check("Unexpected field length", field1.getLength() == field3.getLength());
		this.check("Unexpected XML", field1.toXML() == field3.toXML());
		this.check("Unexpected JSON", field1.toJSON() == field3.toJSON());
		this.check("Unexpected isHeader value", field1.isHeader() == field3.isHeader());

		let data3 = field3.getValue();
		for (let i = 0; i < data3.size(); i++) {
			this.check("BinaryField value at position " + i.toString() + " is wrong", value.get(i) == data3.get(i));
		}
	}

	testBooleanField()
	{
		gmsec.Log.info("Test BooleanField");

		let name  = "MY-FIELD";
		let value = true;

		// Constructor
		let field1 = new gmsec.BooleanField(name, value);
		this.check("Unexpected field type", field1.getType() == gmsec.Field.Type_BOOL);
		this.check("Unexpected field name", field1.getName() == name);
		this.check("Unexpected field value", field1.getValue() == value);
		this.check("Unexpected XML", field1.toXML() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"BOOL\">TRUE</FIELD>");
		this.check("Unexpected JSON", field1.toJSON() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"BOOL\",\"VALUE\":\"TRUE\"}");
		this.check("Unexpected isHeader value", field1.isHeader() == false);

		// Set as header field
		let field2 = new gmsec.BooleanField(name, value, true);
		this.check("Unexpected field type", field2.getType() == gmsec.Field.Type_BOOL);
		this.check("Unexpected field name", field2.getName() == name);
		this.check("Unexpected field value", field2.getValue() == value);
		this.check("Unexpected isHeader value", field2.isHeader());

		// Copy-constructor
		let field3 = new gmsec.BooleanField(field1);
		this.check("Unexpected field type", field1.getType() == field3.getType());
		this.check("Unexpected field name", field1.getName() == field3.getName());
		this.check("Unexpected field value", field1.getValue() == field3.getValue());
		this.check("Unexpected XML", field1.toXML() == field3.toXML());
		this.check("Unexpected JSON", field1.toJSON() == field3.toJSON());
		this.check("Unexpected isHeader value", field1.isHeader() == field3.isHeader());
	}

	testCharField()
	{
		gmsec.Log.info("Test CharField");

		let name  = "MY-FIELD";
		let value = 'f';

		// Constructor
		let field1 = new gmsec.CharField(name, value);
		this.check("Unexpected field type", field1.getType() == gmsec.Field.Type_CHAR);
		this.check("Unexpected field name", field1.getName() == name);
		this.check("Unexpected field value", field1.getValue() == value);
		this.check("Unexpected XML", field1.toXML() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"CHAR\">f</FIELD>");
		this.check("Unexpected JSON", field1.toJSON() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"CHAR\",\"VALUE\":\"f\"}");
		this.check("Unexpected isHeader value", field1.isHeader() == false);

		// Set as header field
		let field2 = new gmsec.CharField(name, value, true);
		this.check("Unexpected field type", field2.getType() == gmsec.Field.Type_CHAR);
		this.check("Unexpected field name", field2.getName() == name);
		this.check("Unexpected field value", field2.getValue() == value);
		this.check("Unexpected isHeader value", field2.isHeader());

		// Copy-constructor
		let field3 = new gmsec.CharField(field1);
		this.check("Unexpected field type", field1.getType() == field3.getType());
		this.check("Unexpected field name", field1.getName() == field3.getName());
		this.check("Unexpected field value", field1.getValue() == field3.getValue());
		this.check("Unexpected XML", field1.toXML() == field3.toXML());
		this.check("Unexpected JSON", field1.toJSON() == field3.toJSON());
		this.check("Unexpected isHeader value", field1.isHeader() == field3.isHeader());
	}

	testStringField()
	{
		gmsec.Log.info("Test StringField");

		let name  = "MY-FIELD";
		let value = 'Test String';

		// Constructor
		let field1 = new gmsec.StringField(name, value);
		this.check("Unexpected field type", field1.getType() == gmsec.Field.Type_STRING);
		this.check("Unexpected field name", field1.getName() == name);
		this.check("Unexpected field value", field1.getValue() == value);
		this.check("Unexpected XML", field1.toXML() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"STRING\">Test String</FIELD>");
		this.check("Unexpected JSON", field1.toJSON() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"STRING\",\"VALUE\":\"Test String\"}");
		this.check("Unexpected isHeader value", field1.isHeader() == false);

		// Set as header field
		let field2 = new gmsec.StringField(name, value, true);
		this.check("Unexpected field type", field2.getType() == gmsec.Field.Type_STRING);
		this.check("Unexpected field name", field2.getName() == name);
		this.check("Unexpected field value", field2.getValue() == value);
		this.check("Unexpected isHeader value", field2.isHeader());

		// Copy-constructor
		let field3 = new gmsec.StringField(field1);
		this.check("Unexpected field type", field1.getType() == field3.getType());
		this.check("Unexpected field name", field1.getName() == field3.getName());
		this.check("Unexpected field value", field1.getValue() == field3.getValue());
		this.check("Unexpected XML", field1.toXML() == field3.toXML());
		this.check("Unexpected JSON", field1.toJSON() == field3.toJSON());
		this.check("Unexpected isHeader value", field1.isHeader() == field3.isHeader());
	}

	testF32Field()
	{
		gmsec.Log.info("Test F32Field");

		let name  = "MY-FIELD";
		let value = 10;

		// Constructor
		let field1 = new gmsec.F32Field(name, value);
		this.check("Unexpected field type", field1.getType() == gmsec.Field.Type_F32);
		this.check("Unexpected field name", field1.getName() == name);
		this.check("Unexpected field value", field1.getValue() == value);
		this.check("Unexpected XML", field1.toXML() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"F32\" BITS=\"41200000\">10</FIELD>")
		this.check("Unexpected JSON", field1.toJSON() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"F32\",\"BITS\":\"41200000\",\"VALUE\":\"10\"}")
		this.check("Unexpected isHeader value", field1.isHeader() == false);

		// Set as header field
		let field2 = new gmsec.F32Field(name, value, true);
		this.check("Unexpected field type", field2.getType() == gmsec.Field.Type_F32);
		this.check("Unexpected field name", field2.getName() == name);
		this.check("Unexpected field value", field2.getValue() == value);
		this.check("Unexpected isHeader value", field2.isHeader());

		// Copy-constructor
		let field3 = new gmsec.F32Field(field1);
		this.check("Unexpected field type", field1.getType() == field3.getType());
		this.check("Unexpected field name", field1.getName() == field3.getName());
		this.check("Unexpected field value", field1.getValue() == field3.getValue());
		this.check("Unexpected XML", field1.toXML() == field3.toXML());
		this.check("Unexpected JSON", field1.toJSON() == field3.toJSON());
		this.check("Unexpected isHeader value", field1.isHeader() == field3.isHeader());
	}

	testF64Field()
	{
		gmsec.Log.info("Test F64Field");

		let name  = "MY-FIELD";
		let value = 10;

		// Constructor
		let field1 = new gmsec.F64Field(name, value);
		this.check("Unexpected field type", field1.getType() == gmsec.Field.Type_F64);
		this.check("Unexpected field name", field1.getName() == name);
		this.check("Unexpected field value", field1.getValue() == value);
		this.check("Unexpected XML", field1.toXML() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"F64\" BITS=\"4024000000000000\">10</FIELD>")
		this.check("Unexpected JSON", field1.toJSON() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"F64\",\"BITS\":\"4024000000000000\",\"VALUE\":\"10\"}")
		this.check("Unexpected isHeader value", field1.isHeader() == false);

		// Set as header field
		let field2 = new gmsec.F64Field(name, value, true);
		this.check("Unexpected field type", field2.getType() == gmsec.Field.Type_F64);
		this.check("Unexpected field name", field2.getName() == name);
		this.check("Unexpected field value", field2.getValue() == value);
		this.check("Unexpected isHeader value", field2.isHeader());

		// Copy-constructor
		let field3 = new gmsec.F64Field(field1);
		this.check("Unexpected field type", field1.getType() == field3.getType());
		this.check("Unexpected field name", field1.getName() == field3.getName());
		this.check("Unexpected field value", field1.getValue() == field3.getValue());
		this.check("Unexpected XML", field1.toXML() == field3.toXML());
		this.check("Unexpected JSON", field1.toJSON() == field3.toJSON());
		this.check("Unexpected isHeader value", field1.isHeader() == field3.isHeader());
	}

	testI8Field()
	{
		gmsec.Log.info("Test I8Field");

		let name  = "MY-FIELD";
		let value = 10;

		// Constructor
		let field1 = new gmsec.I8Field(name, value);
		this.check("Unexpected field type", field1.getType() == gmsec.Field.Type_I8);
		this.check("Unexpected field name", field1.getName() == name);
		this.check("Unexpected field value", field1.getValue() == value);
		this.check("Unexpected XML", field1.toXML() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"I8\">10</FIELD>")
		this.check("Unexpected JSON", field1.toJSON() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"I8\",\"VALUE\":\"10\"}")
		this.check("Unexpected isHeader value", field1.isHeader() == false);

		// Set as header field
		let field2 = new gmsec.I8Field(name, value, true);
		this.check("Unexpected field type", field2.getType() == gmsec.Field.Type_I8);
		this.check("Unexpected field name", field2.getName() == name);
		this.check("Unexpected field value", field2.getValue() == value);
		this.check("Unexpected field value", field2.isHeader());
		this.check("Unexpected isHeader value", field2.isHeader());

		// Copy-constructor
		let field3 = new gmsec.I8Field(field1);
		this.check("Unexpected field type", field1.getType() == field3.getType());
		this.check("Unexpected field name", field1.getName() == field3.getName());
		this.check("Unexpected field value", field1.getValue() == field3.getValue());
		this.check("Unexpected XML", field1.toXML() == field3.toXML());
		this.check("Unexpected JSON", field1.toJSON() == field3.toJSON());
		this.check("Unexpected isHeader value", field1.isHeader() == field3.isHeader());
	}

	testI16Field()
	{
		gmsec.Log.info("Test I16Field");

		let name  = "MY-FIELD";
		let value = 10;

		// Constructor
		let field1 = new gmsec.I16Field(name, value);
		this.check("Unexpected field type", field1.getType() == gmsec.Field.Type_I16);
		this.check("Unexpected field name", field1.getName() == name);
		this.check("Unexpected field value", field1.getValue() == value);
		this.check("Unexpected XML", field1.toXML() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"I16\">10</FIELD>")
		this.check("Unexpected JSON", field1.toJSON() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"I16\",\"VALUE\":\"10\"}")
		this.check("Unexpected isHeader value", field1.isHeader() == false);

		// Set as header field
		let field2 = new gmsec.I16Field(name, value, true);
		this.check("Unexpected field type", field2.getType() == gmsec.Field.Type_I16);
		this.check("Unexpected field name", field2.getName() == name);
		this.check("Unexpected field value", field2.getValue() == value);
		this.check("Unexpected field value", field2.isHeader());
		this.check("Unexpected isHeader value", field2.isHeader());

		// Copy-constructor
		let field3 = new gmsec.I16Field(field1);
		this.check("Unexpected field type", field1.getType() == field3.getType());
		this.check("Unexpected field name", field1.getName() == field3.getName());
		this.check("Unexpected field value", field1.getValue() == field3.getValue());
		this.check("Unexpected XML", field1.toXML() == field3.toXML());
		this.check("Unexpected JSON", field1.toJSON() == field3.toJSON());
		this.check("Unexpected isHeader value", field1.isHeader() == field3.isHeader());
	}

	testI32Field()
	{
		gmsec.Log.info("Test I32Field");

		let name  = "MY-FIELD";
		let value = 10;

		// Constructor
		let field1 = new gmsec.I32Field(name, value);
		this.check("Unexpected field type", field1.getType() == gmsec.Field.Type_I32);
		this.check("Unexpected field name", field1.getName() == name);
		this.check("Unexpected field value", field1.getValue() == value);
		this.check("Unexpected XML", field1.toXML() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"I32\">10</FIELD>")
		this.check("Unexpected JSON", field1.toJSON() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"I32\",\"VALUE\":\"10\"}")
		this.check("Unexpected isHeader value", field1.isHeader() == false);

		// Set as header field
		let field2 = new gmsec.I32Field(name, value, true);
		this.check("Unexpected field type", field2.getType() == gmsec.Field.Type_I32);
		this.check("Unexpected field name", field2.getName() == name);
		this.check("Unexpected field value", field2.getValue() == value);
		this.check("Unexpected field value", field2.isHeader());
		this.check("Unexpected isHeader value", field2.isHeader());

		// Copy-constructor
		let field3 = new gmsec.I32Field(field1);
		this.check("Unexpected field type", field1.getType() == field3.getType());
		this.check("Unexpected field name", field1.getName() == field3.getName());
		this.check("Unexpected field value", field1.getValue() == field3.getValue());
		this.check("Unexpected XML", field1.toXML() == field3.toXML());
		this.check("Unexpected JSON", field1.toJSON() == field3.toJSON());
		this.check("Unexpected isHeader value", field1.isHeader() == field3.isHeader());
	}

	testI64Field()
	{
		gmsec.Log.info("Test I64Field");

		let name  = "MY-FIELD";
		let value = 10;

		// Constructor
		let field1 = new gmsec.I64Field(name, value);
		this.check("Unexpected field type", field1.getType() == gmsec.Field.Type_I64);
		this.check("Unexpected field name", field1.getName() == name);
		this.check("Unexpected field value", field1.getValue() == value);
		this.check("Unexpected XML", field1.toXML() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"I64\">10</FIELD>")
		this.check("Unexpected JSON", field1.toJSON() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"I64\",\"VALUE\":\"10\"}")
		this.check("Unexpected isHeader value", field1.isHeader() == false);

		// Set as header field
		let field2 = new gmsec.I64Field(name, value, true);
		this.check("Unexpected field type", field2.getType() == gmsec.Field.Type_I64);
		this.check("Unexpected field name", field2.getName() == name);
		this.check("Unexpected field value", field2.getValue() == value);
		this.check("Unexpected isHeader value", field2.isHeader());

		// Copy-constructor
		let field3 = new gmsec.I64Field(field1);
		this.check("Unexpected field type", field1.getType() == field3.getType());
		this.check("Unexpected field name", field1.getName() == field3.getName());
		this.check("Unexpected field value", field1.getValue() == field3.getValue());
		this.check("Unexpected XML", field1.toXML() == field3.toXML());
		this.check("Unexpected JSON", field1.toJSON() == field3.toJSON());
		this.check("Unexpected isHeader value", field1.isHeader() == field3.isHeader());
	}

	testU8Field()
	{
		gmsec.Log.info("Test U8Field");

		let name  = "MY-FIELD";
		let value = 10;

		// Constructor
		let field1 = new gmsec.U8Field(name, value);
		this.check("Unexpected field type", field1.getType() == gmsec.Field.Type_U8);
		this.check("Unexpected field name", field1.getName() == name);
		this.check("Unexpected field value", field1.getValue() == value);
		this.check("Unexpected XML", field1.toXML() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"U8\">10</FIELD>")
		this.check("Unexpected JSON", field1.toJSON() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"U8\",\"VALUE\":\"10\"}")
		this.check("Unexpected isHeader value", field1.isHeader() == false);

		// Set as header field
		let field2 = new gmsec.U8Field(name, value, true);
		this.check("Unexpected field type", field2.getType() == gmsec.Field.Type_U8);
		this.check("Unexpected field name", field2.getName() == name);
		this.check("Unexpected field value", field2.getValue() == value);
		this.check("Unexpected field value", field2.isHeader());
		this.check("Unexpected isHeader value", field2.isHeader());

		// Copy-constructor
		let field3 = new gmsec.U8Field(field1);
		this.check("Unexpected field type", field1.getType() == field3.getType());
		this.check("Unexpected field name", field1.getName() == field3.getName());
		this.check("Unexpected field value", field1.getValue() == field3.getValue());
		this.check("Unexpected XML", field1.toXML() == field3.toXML());
		this.check("Unexpected JSON", field1.toJSON() == field3.toJSON());
		this.check("Unexpected isHeader value", field1.isHeader() == field3.isHeader());
	}

	testU16Field()
	{
		gmsec.Log.info("Test U16Field");

		let name  = "MY-FIELD";
		let value = 10;

		// Constructor
		let field1 = new gmsec.U16Field(name, value);
		this.check("Unexpected field type", field1.getType() == gmsec.Field.Type_U16);
		this.check("Unexpected field name", field1.getName() == name);
		this.check("Unexpected field value", field1.getValue() == value);
		this.check("Unexpected XML", field1.toXML() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"U16\">10</FIELD>")
		this.check("Unexpected JSON", field1.toJSON() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"U16\",\"VALUE\":\"10\"}")
		this.check("Unexpected isHeader value", field1.isHeader() == false);

		// Set as header field
		let field2 = new gmsec.U16Field(name, value, true);
		this.check("Unexpected field type", field2.getType() == gmsec.Field.Type_U16);
		this.check("Unexpected field name", field2.getName() == name);
		this.check("Unexpected field value", field2.getValue() == value);
		this.check("Unexpected field value", field2.isHeader());
		this.check("Unexpected isHeader value", field2.isHeader());

		// Copy-constructor
		let field3 = new gmsec.U16Field(field1);
		this.check("Unexpected field type", field1.getType() == field3.getType());
		this.check("Unexpected field name", field1.getName() == field3.getName());
		this.check("Unexpected field value", field1.getValue() == field3.getValue());
		this.check("Unexpected XML", field1.toXML() == field3.toXML());
		this.check("Unexpected JSON", field1.toJSON() == field3.toJSON());
		this.check("Unexpected isHeader value", field1.isHeader() == field3.isHeader());
	}

	testU32Field()
	{
		gmsec.Log.info("Test U32Field");

		let name  = "MY-FIELD";
		let value = 10;

		// Constructor
		let field1 = new gmsec.U32Field(name, value);
		this.check("Unexpected field type", field1.getType() == gmsec.Field.Type_U32);
		this.check("Unexpected field name", field1.getName() == name);
		this.check("Unexpected field value", field1.getValue() == value);
		this.check("Unexpected XML", field1.toXML() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"U32\">10</FIELD>")
		this.check("Unexpected JSON", field1.toJSON() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"U32\",\"VALUE\":\"10\"}")
		this.check("Unexpected isHeader value", field1.isHeader() == false);

		// Set as header field
		let field2 = new gmsec.U32Field(name, value, true);
		this.check("Unexpected field type", field2.getType() == gmsec.Field.Type_U32);
		this.check("Unexpected field name", field2.getName() == name);
		this.check("Unexpected field value", field2.getValue() == value);
		this.check("Unexpected field value", field2.isHeader());
		this.check("Unexpected isHeader value", field2.isHeader());

		// Copy-constructor
		let field3 = new gmsec.U32Field(field1);
		this.check("Unexpected field type", field1.getType() == field3.getType());
		this.check("Unexpected field name", field1.getName() == field3.getName());
		this.check("Unexpected field value", field1.getValue() == field3.getValue());
		this.check("Unexpected XML", field1.toXML() == field3.toXML());
		this.check("Unexpected JSON", field1.toJSON() == field3.toJSON());
		this.check("Unexpected isHeader value", field1.isHeader() == field3.isHeader());
	}

	testU64Field()
	{
		gmsec.Log.info("Test U64Field");

		let name  = "MY-FIELD";
		let value = 10;

		// Constructor
		let field1 = new gmsec.U64Field(name, value);
		this.check("Unexpected field type", field1.getType() == gmsec.Field.Type_U64);
		this.check("Unexpected field name", field1.getName() == name);
		this.check("Unexpected field value", field1.getValue() == value);
		this.check("Unexpected XML", field1.toXML() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"U64\">10</FIELD>")
		this.check("Unexpected JSON", field1.toJSON() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"U64\",\"VALUE\":\"10\"}")
		this.check("Unexpected isHeader value", field1.isHeader() == false);

		// Set as header field
		let field2 = new gmsec.U64Field(name, value, true);
		this.check("Unexpected field type", field2.getType() == gmsec.Field.Type_U64);
		this.check("Unexpected field name", field2.getName() == name);
		this.check("Unexpected field value", field2.getValue() == value);
		this.check("Unexpected isHeader value", field2.isHeader());

		// Copy-constructor
		let field3 = new gmsec.U64Field(field1);
		this.check("Unexpected field type", field1.getType() == field3.getType());
		this.check("Unexpected field name", field1.getName() == field3.getName());
		this.check("Unexpected field value", field1.getValue() == field3.getValue());
		this.check("Unexpected XML", field1.toXML() == field3.toXML());
		this.check("Unexpected JSON", field1.toJSON() == field3.toJSON());
		this.check("Unexpected isHeader value", field1.isHeader() == field3.isHeader());
	}

	testField()
	{
		gmsec.Log.info("Test Field");

		let field1 = new gmsec.I32Field("I32-FIELD", -1234);
		let field2 = new gmsec.U32Field("U32-FIELD", 1234);
		let field3 = new gmsec.F32Field("U32-FIELD", 1234.56);
		let field4 = new gmsec.StringField("STR-FIELD", "Hello World");
		let field5 = new gmsec.StringField("STR-FIELD1", "FALSE");
		let field6 = new gmsec.StringField("STR-FIELD2", "true");
		let field7 = new gmsec.BooleanField("BOOL-FIELD", true);

		// Call getStringValue()
		this.check("Fail getStringValue 1", field1.getStringValue() == "-1234");
		this.check("Fail getStringValue 2", field2.getStringValue() == "1234");
		this.check("Fail getStringValue 3", field3.getStringValue() == "1234.56");
		this.check("Fail getStringValue 4", field4.getStringValue() == "Hello World");

		// Call getBooleanValue()
		this.check("Fail getBooleanValue 1", field1.getBooleanValue() == true);
		this.check("Fail getBooleanValue 2", field2.getBooleanValue() == true);
		this.check("Fail getBooleanValue 3", field3.getBooleanValue() == true);
		this.check("Fail getBooleanValue 5", field5.getBooleanValue() == false);
		this.check("Fail getBooleanValue 6", field6.getBooleanValue() == true);
		this.check("Fail getBooleanValue 7", field7.getBooleanValue() == true);

		try {
			field4.getBooleanValue();
			this.check("Expected boolean conversion exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Field cannot be represented as a boolean"));
		}

		// Call getI32Value()
		this.check("Fail getI32Value 1", field1.getI32Value() == -1234);
		this.check("Fail getI32Value 2", field2.getI32Value() == 1234);
		this.check("Fail getI32Value 3", field3.getI32Value() == 1234);

		try {
			field4.getI32Value();
			this.check("Expected I32 conversion exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Field cannot be converted to a GMSEC_I32"));
		}

		// Call getDoubleValue()
		this.check("Fail getDoubleValue 1", field1.getDoubleValue() == -1234.0);
		this.check("Fail getDoubleValue 2", field2.getDoubleValue() == 1234.0);
		this.check("Fail getDoubleValue 3", field3.getDoubleValue() == 1234.56);

		try {
			field4.getDoubleValue();
			this.check("Expected F64 conversion exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Field cannot be converted to a GMSEC_F64"));
		}

		// Call on down-cast methods
		let value = new gmsec.BlobArray();
		for (let i = 0; i < 4; ++i) {
			value.add(i);
		}

		let msg = new gmsec.Message();
		msg.addField(new gmsec.BinaryField("FIELD-1", value));
		msg.addField(new gmsec.BooleanField("FIELD-2", true));
		msg.addField(new gmsec.CharField("FIELD-3", 'c'));
		msg.addField(new gmsec.F32Field("FIELD-4", 2));
		msg.addField(new gmsec.F64Field("FIELD-5", 2.1));
		msg.addField(new gmsec.I16Field("FIELD-6", 1));
		msg.addField(new gmsec.I32Field("FIELD-7", 2));
		msg.addField(new gmsec.I64Field("FIELD-8", 3));
		msg.addField(new gmsec.I8Field("FIELD-9", 4));
		msg.addField(new gmsec.StringField("FIELD-10", "Hello World"));
		msg.addField(new gmsec.U16Field("FIELD-11", 1));
		msg.addField(new gmsec.U32Field("FIELD-12", 2));
		msg.addField(new gmsec.U64Field("FIELD-13", 3));
		msg.addField(new gmsec.U8Field("FIELD-14", 4));

		let field = gmsec.Field.toBinaryField(msg.getField("FIELD-1"));
		this.require("Unexpected field reference", field != null);
		this.check("Unexpected field type", field.getType() == gmsec.Field.Type_BINARY);
		this.check("Unexpected field name", field.getName() == "FIELD-1");
		let data1 = field.getValue();
		for (let i = 0; i < data1.size(); i++) {
			if (value.get(i) != data1.get(i)) {
				this.check("BinaryField value at position " + i.toString() + " is wrong", false);
			}
		}

		field = gmsec.Field.toBooleanField(msg.getField("FIELD-2"));
		this.require("Unexpected field reference", field != null);
		this.check("Unexpected field type", field.getType() == gmsec.Field.Type_BOOL);
		this.check("Unexpected field name", field.getName() == "FIELD-2");
		this.check("Unexpected field value", field.getValue() == true);

		field = gmsec.Field.toCharField(msg.getField("FIELD-3"));
		this.require("Unexpected field reference", field != null);
		this.check("Unexpected field type", field.getType() == gmsec.Field.Type_CHAR);
		this.check("Unexpected field name", field.getName() == "FIELD-3");
		this.check("Unexpected field value", field.getValue() == 'c');

		field = gmsec.Field.toF32Field(msg.getField("FIELD-4"));
		this.require("Unexpected field reference", field != null);
		this.check("Unexpected field type", field.getType() == gmsec.Field.Type_F32);
		this.check("Unexpected field name", field.getName() == "FIELD-4");
		this.check("Unexpected field value", field.getValue() == 2);

		field = gmsec.Field.toF64Field(msg.getField("FIELD-5"));
		this.require("Unexpected field reference", field != null);
		this.check("Unexpected field type", field.getType() == gmsec.Field.Type_F64);
		this.check("Unexpected field name", field.getName() == "FIELD-5");
		this.check("Unexpected field value", field.getValue() == 2.1);

		field = gmsec.Field.toI16Field(msg.getField("FIELD-6"));
		this.require("Unexpected field reference", field != null);
		this.check("Unexpected field type", field.getType() == gmsec.Field.Type_I16);
		this.check("Unexpected field name", field.getName() == "FIELD-6");
		this.check("Unexpected field value", field.getValue() == 1);

		field = gmsec.Field.toI32Field(msg.getField("FIELD-7"));
		this.require("Unexpected field reference", field != null);
		this.check("Unexpected field type", field.getType() == gmsec.Field.Type_I32);
		this.check("Unexpected field name", field.getName() == "FIELD-7");
		this.check("Unexpected field value", field.getValue() == 2);

		field = gmsec.Field.toI64Field(msg.getField("FIELD-8"));
		this.require("Unexpected field reference", field != null);
		this.check("Unexpected field type", field.getType() == gmsec.Field.Type_I64);
		this.check("Unexpected field name", field.getName() == "FIELD-8");
		this.check("Unexpected field value", field.getValue() == 3);

		field = gmsec.Field.toI8Field(msg.getField("FIELD-9"));
		this.require("Unexpected field reference", field != null);
		this.check("Unexpected field type", field.getType() == gmsec.Field.Type_I8);
		this.check("Unexpected field name", field.getName() == "FIELD-9");
		this.check("Unexpected field value", field.getValue() == 4);

		field = gmsec.Field.toStringField(msg.getField("FIELD-10"));
		this.require("Unexpected field reference", field != null);
		this.check("Unexpected field type", field.getType() == gmsec.Field.Type_STRING);
		this.check("Unexpected field name", field.getName() == "FIELD-10");
		this.check("Unexpected field value", field.getValue() == "Hello World");

		field = gmsec.Field.toU16Field(msg.getField("FIELD-11"));
		this.require("Unexpected field reference", field != null);
		this.check("Unexpected field type", field.getType() == gmsec.Field.Type_U16);
		this.check("Unexpected field name", field.getName() == "FIELD-11");
		this.check("Unexpected field value", field.getValue() == 1);

		field = gmsec.Field.toU32Field(msg.getField("FIELD-12"));
		this.require("Unexpected field reference", field != null);
		this.check("Unexpected field type", field.getType() == gmsec.Field.Type_U32);
		this.check("Unexpected field name", field.getName() == "FIELD-12");
		this.check("Unexpected field value", field.getValue() == 2);

		field = gmsec.Field.toU64Field(msg.getField("FIELD-13"));
		this.require("Unexpected field reference", field != null);
		this.check("Unexpected field type", field.getType() == gmsec.Field.Type_U64);
		this.check("Unexpected field name", field.getName() == "FIELD-13");
		this.check("Unexpected field value", field.getValue() == 3);

		field = gmsec.Field.toU8Field(msg.getField("FIELD-14"));
		this.require("Unexpected field reference", field != null);
		this.check("Unexpected field type", field.getType() == gmsec.Field.Type_U8);
		this.check("Unexpected field name", field.getName() == "FIELD-14");
		this.check("Unexpected field value", field.getValue() == 4);
	}
}

test = new T003_Fields();
test.setDescription("Test Fields");
test.run(process.argv);
