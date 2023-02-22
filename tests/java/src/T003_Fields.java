import gov.nasa.gsfc.gmsec.api5.field.Field;

import gov.nasa.gsfc.gmsec.api5.field.BinaryField;
import gov.nasa.gsfc.gmsec.api5.field.BooleanField;
import gov.nasa.gsfc.gmsec.api5.field.CharField;
import gov.nasa.gsfc.gmsec.api5.field.F32Field;
import gov.nasa.gsfc.gmsec.api5.field.F64Field;
import gov.nasa.gsfc.gmsec.api5.field.StringField;

import gov.nasa.gsfc.gmsec.api5.field.I8Field;
import gov.nasa.gsfc.gmsec.api5.field.I16Field;
import gov.nasa.gsfc.gmsec.api5.field.I32Field;
import gov.nasa.gsfc.gmsec.api5.field.I64Field;

import gov.nasa.gsfc.gmsec.api5.field.U8Field;
import gov.nasa.gsfc.gmsec.api5.field.U16Field;
import gov.nasa.gsfc.gmsec.api5.field.U32Field;
import gov.nasa.gsfc.gmsec.api5.field.U64Field;

import gov.nasa.gsfc.gmsec.api5.GmsecException;
import gov.nasa.gsfc.gmsec.api5.U8;
import gov.nasa.gsfc.gmsec.api5.U16;
import gov.nasa.gsfc.gmsec.api5.U32;
import gov.nasa.gsfc.gmsec.api5.U64;

import gov.nasa.gsfc.gmsec.api5.util.Log;

import java.math.BigInteger;


public class T003_Fields extends TestCase
{
	public static void main(String[] args)
	{
		T003_Fields test = new T003_Fields();
		test.setDescription("Test Fields");
		test.run(args);
	}


	public void testCase()
		throws Exception
	{
		try
		{
			test_BinaryField();
			test_BooleanField();
			test_CharField();
			test_StringField();

			test_F32Field();
			test_F64Field();

			test_I8Field();
			test_I16Field();
			test_I32Field();
			test_I64Field();

			test_U8Field();
			test_U16Field();
			test_U32Field();
			test_U64Field();

			test_Field();
		}
		catch (Exception e)
		{
			require(e.getMessage(), false);
		}
	}


	private void test_BinaryField()
		throws GmsecException
	{
		Log.info("Test BinaryField");

		// Nominal test
		String name  = "MY-FIELD";
		byte[] value = new byte[4];
		for (int i = 0; i < 4; ++i) {
			value[i] = (byte) i;
		}

		// Constructor
		BinaryField field1 = new BinaryField(name, value);
		check("Unexpected field type", field1.getType() == Field.Type.BINARY);
		check("Unexpected field name", field1.getName().equals(name));
		check("Unexpected XML", field1.toXML().equals("<FIELD NAME=\"MY-FIELD\" TYPE=\"BIN\">00010203</FIELD>"));
		check("Unexpected JSON", field1.toJSON().equals("{\"NAME\":\"MY-FIELD\",\"TYPE\":\"BIN\",\"VALUE\":\"00010203\"}"));

		byte[] blob = field1.getValue();
		for (int i = 0; i < value.length; ++i) {
			if (blob[i] != value[i]) {
				Log.debug("BinaryField value at position " + i + " is wrong");
				check("BinaryField value is wrong", false);
			}
		}

		// Copy constructor
		BinaryField field2 = new BinaryField(field1);
		check("Unexpected field type", field2.getType() == field1.getType());
		check("Unexpected field name", field2.getName().equals(field1.getName()));
		check("Unexpected XML", field2.toXML().equals(field1.toXML()));
		check("Unexpected JSON", field2.toJSON().equals(field1.toJSON()));

		for (int i = 0; i < value.length; ++i) {
			if (blob[i] != value[i]) {
				Log.debug("BinaryField value at position " + i + " is wrong");
				check("BinaryField value is wrong", false);
			}
		}

		// Off-Nominal test
		try {
			// null field name
			new BinaryField(null, value);
			check("Exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}

		try {
			// empty-string field name
			new BinaryField("", value);
			check("Exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}
	}


	private void test_BooleanField()
		throws GmsecException
	{
		Log.info("Test BooleanField");

		// Nominal test
		String  name  = "MY-FIELD";
		boolean value = true;

		// Constructor
		BooleanField field1 = new BooleanField(name, value);
		check("Unexpected field type", field1.getType() == Field.Type.BOOL);
		check("Unexpected field name", field1.getName().equals(name));
		check("Unexpected field value", field1.getValue() == value);
		check("Unexpected XML", field1.toXML().equals("<FIELD NAME=\"MY-FIELD\" TYPE=\"BOOL\">TRUE</FIELD>"));
		check("Unexpected JSON", field1.toJSON().equals("{\"NAME\":\"MY-FIELD\",\"TYPE\":\"BOOL\",\"VALUE\":\"TRUE\"}"));


		// Copy-constructor
		BooleanField field2 = new BooleanField(field1);
		check("Unexpected field type", field2.getType() == field1.getType());
		check("Unexpected field name", field2.getName().equals(field1.getName()));
		check("Unexpected field value", field2.getValue() == field1.getValue());
		check("Unexpected XML", field2.toXML().equals(field1.toXML()));
		check("Unexpected JSON", field2.toJSON().equals(field1.toJSON()));

		// Off-Nominal test
		try
		{
			// null field name
			new BooleanField(null, value);
			check("IllegalArgumentException was expected", false);
		}
		catch (IllegalArgumentException e)
		{
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}

		try
		{
			// empty-string field name
			new BooleanField("", value);
			check("IllegalArgumentException was expected", false);
		}
		catch (IllegalArgumentException e)
		{
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}
	}


	private void test_CharField()
		throws GmsecException
	{
		Log.info("Test CharField");

		// Nominal test
		String name  = "MY-FIELD";
		char   value = 'f';

		// Constructor
		CharField field1 = new CharField(name, value);
		check("Unexpected field type", field1.getType() == Field.Type.CHAR);
		check("Unexpected field name", field1.getName().equals(name));
		check("Unexpected field value", field1.getValue() == value);
		check("Unexpected XML", field1.toXML().equals("<FIELD NAME=\"MY-FIELD\" TYPE=\"CHAR\">f</FIELD>"));
		check("Unexpected JSON", field1.toJSON().equals("{\"NAME\":\"MY-FIELD\",\"TYPE\":\"CHAR\",\"VALUE\":\"f\"}"));


		// Copy-constructor
		CharField field2 = new CharField(field1);
		check("Unexpected field type", field2.getType() == field1.getType());
		check("Unexpected field name", field2.getName().equals(field1.getName()));
		check("Unexpected field value", field2.getValue() == field1.getValue());
		check("Unexpected XML", field2.toXML().equals(field1.toXML()));
		check("Unexpected JSON", field2.toJSON().equals(field1.toJSON()));

		// Off-Nominal test
		try
		{
			// null field name
			new CharField(null, value);
			check("IllegalArgumentException was expected", false);
		}
		catch (IllegalArgumentException e)
		{
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}

		try
		{
			// empty-string field name
			new CharField("", value);
			check("IllegalArgumentException was expected", false);
		}
		catch (IllegalArgumentException e)
		{
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}
	}


	private void test_StringField()
		throws GmsecException
	{
		Log.info("Test StringField");

		// Nominal test
		String name  = "MY-FIELD";
		String value = "Test String";

		// Constructor
		StringField field1 = new StringField(name, value);
		check("Unexpected field type", field1.getType() == Field.Type.STRING);
		check("Unexpected field name", field1.getName().equals(name));
		check("Unexpected field value", field1.getValue().equals(value));
		check("Unexpected XML", field1.toXML().equals("<FIELD NAME=\"MY-FIELD\" TYPE=\"STRING\">Test String</FIELD>"));
		check("Unexpected JSON", field1.toJSON().equals("{\"NAME\":\"MY-FIELD\",\"TYPE\":\"STRING\",\"VALUE\":\"Test String\"}"));


		// Copy-constructor
		StringField field2 = new StringField(field1);
		check("Unexpected field type", field2.getType() == field1.getType());
		check("Unexpected field name", field2.getName().equals(field1.getName()));
		check("Unexpected field value", field2.getValue().equals(field1.getValue()));
		check("Unexpected XML", field2.toXML().equals(field1.toXML()));
		check("Unexpected JSON", field2.toJSON().equals(field1.toJSON()));

		// Off-Nominal test
		try
		{
			// null field name
			new StringField(null, value);
			check("GmsecException was expected", false);
			check("IllegalArgumentException was expected", false);
		}
		catch (IllegalArgumentException e)
		{
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}

		try
		{
			// empty-string field name
			new StringField("", value);
			check("IllegalArgumentException was expected", false);
		}
		catch (IllegalArgumentException e)
		{
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}
	}


	private void test_F32Field()
		throws GmsecException
	{
		Log.info("Test F32Field");

		// Nominal test
		String name  = "MY-FIELD";
		float  value = 10;

		// Constructor
		F32Field field1 = new F32Field(name, value);
		check("Unexpected field type", field1.getType() == Field.Type.F32);
		check("Unexpected field name", field1.getName().equals(name));
		check("Unexpected field value", field1.getValue() == value);
		check("Unexpected XML", field1.toXML().equals("<FIELD NAME=\"MY-FIELD\" TYPE=\"F32\" BITS=\"41200000\">10</FIELD>"));
		check("Unexpected JSON", field1.toJSON().equals("{\"NAME\":\"MY-FIELD\",\"TYPE\":\"F32\",\"BITS\":\"41200000\",\"VALUE\":\"10\"}"));


		// Copy-constructor
		F32Field field2 = new F32Field(field1);
		check("Unexpected field type", field2.getType() == field1.getType());
		check("Unexpected field name", field2.getName().equals(field1.getName()));
		check("Unexpected field value", field2.getValue() == field1.getValue());
		check("Unexpected XML", field2.toXML().equals(field1.toXML()));
		check("Unexpected JSON", field2.toJSON().equals(field1.toJSON()));

		// Off-Nominal test
		try
		{
			// null field name
			new F32Field(null, value);
			check("GmsecException was expected", false);
			check("IllegalArgumentException was expected", false);
		}
		catch (IllegalArgumentException e)
		{
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}

		try
		{
			// empty-string field name
			new F32Field("", value);
			check("GmsecException was expected", false);
			check("IllegalArgumentException was expected", false);
		}
		catch (IllegalArgumentException e)
		{
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}
	}


	private void test_F64Field()
		throws GmsecException
	{
		Log.info("Test F64Field");

		// Nominal test
		String name  = "MY-FIELD";
		double value = 10;

		// Constructor
		F64Field field1 = new F64Field(name, value);
		check("Unexpected field type", field1.getType() == Field.Type.F64);
		check("Unexpected field name", field1.getName().equals(name));
		check("Unexpected field value", field1.getValue() == value);
		check("Unexpected XML", field1.toXML().equals("<FIELD NAME=\"MY-FIELD\" TYPE=\"F64\" BITS=\"4024000000000000\">10</FIELD>"));
		check("Unexpected JSON", field1.toJSON().equals("{\"NAME\":\"MY-FIELD\",\"TYPE\":\"F64\",\"BITS\":\"4024000000000000\",\"VALUE\":\"10\"}"));


		// Copy-constructor
		F64Field field2 = new F64Field(field1);
		check("Unexpected field type", field2.getType() == field1.getType());
		check("Unexpected field name", field2.getName().equals(field1.getName()));
		check("Unexpected field value", field2.getValue() == field1.getValue());
		check("Unexpected XML", field2.toXML().equals(field1.toXML()));
		check("Unexpected JSON", field2.toJSON().equals(field1.toJSON()));

		// Off-Nominal test
		try
		{
			// null field name
			new F64Field(null, value);
			check("IllegalArgumentException was expected", false);
		}
		catch (IllegalArgumentException e)
		{
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}

		try
		{
			// empty-string field name
			new F64Field("", value);
			check("IllegalArgumentException was expected", false);
		}
		catch (IllegalArgumentException e)
		{
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}
	}


	private void test_I8Field()
		throws GmsecException
	{
		Log.info("Test I8Field");

		// Nominal test
		String name  = "MY-FIELD";
		byte   value = 10;

		// Constructor
		I8Field field1 = new I8Field(name, value);
		check("Unexpected field type", field1.getType() == Field.Type.I8);
		check("Unexpected field name", field1.getName().equals(name));
		check("Unexpected field value", field1.getValue() == value);
		check("Unexpected XML", field1.toXML().equals("<FIELD NAME=\"MY-FIELD\" TYPE=\"I8\">10</FIELD>"));
		check("Unexpected JSON", field1.toJSON().equals("{\"NAME\":\"MY-FIELD\",\"TYPE\":\"I8\",\"VALUE\":\"10\"}"));


		// Copy-constructor
		I8Field field2 = new I8Field(field1);
		check("Unexpected field type", field2.getType() == field1.getType());
		check("Unexpected field name", field2.getName().equals(field1.getName()));
		check("Unexpected field value", field2.getValue() == field1.getValue());
		check("Unexpected XML", field2.toXML().equals(field1.toXML()));
		check("Unexpected JSON", field2.toJSON().equals(field1.toJSON()));

		// Off-Nominal test
		try
		{
			// null field name
			new I8Field(null, value);
			check("IllegalArgumentException was expected", false);
		}
		catch (IllegalArgumentException e)
		{
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}

		try
		{
			// empty-string field name
			new I8Field("", value);
			check("IllegalArgumentException was expected", false);
		}
		catch (IllegalArgumentException e)
		{
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}
	}


	private void test_I16Field()
		throws GmsecException
	{
		Log.info("Test I16Field");

		// Nominal test
		String name  = "MY-FIELD";
		short  value = 10;

		// Constructor
		I16Field field1 = new I16Field(name, value);
		check("Unexpected field type", field1.getType() == Field.Type.I16);
		check("Unexpected field name", field1.getName().equals(name));
		check("Unexpected field value", field1.getValue() == value);
		check("Unexpected XML", field1.toXML().equals("<FIELD NAME=\"MY-FIELD\" TYPE=\"I16\">10</FIELD>"));
		check("Unexpected JSON", field1.toJSON().equals("{\"NAME\":\"MY-FIELD\",\"TYPE\":\"I16\",\"VALUE\":\"10\"}"));

		// Copy-constructor
		I16Field field2 = new I16Field(field1);
		check("Unexpected field type", field2.getType() == field1.getType());
		check("Unexpected field name", field2.getName().equals(field1.getName()));
		check("Unexpected field value", field2.getValue() == field1.getValue());
		check("Unexpected XML", field2.toXML().equals(field1.toXML()));
		check("Unexpected JSON", field2.toJSON().equals(field1.toJSON()));

		// Off-Nominal test
		try
		{
			// null field name
			new I16Field(null, value);
			check("IllegalArgumentException was expected", false);
		}
		catch (IllegalArgumentException e)
		{
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}

		try
		{
			// empty-string field name
			new I16Field("", value);
			check("IllegalArgumentException was expected", false);
		}
		catch (IllegalArgumentException e)
		{
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}
	}


	private void test_I32Field()
		throws GmsecException
	{
		Log.info("Test I32Field");

		// Nominal test
		String name  = "MY-FIELD";
		int    value = 10;

		// Constructor
		I32Field field1 = new I32Field(name, value);
		check("Unexpected field type", field1.getType() == Field.Type.I32);
		check("Unexpected field name", field1.getName().equals(name));
		check("Unexpected field value", field1.getValue() == value);
		check("Unexpected XML", field1.toXML().equals("<FIELD NAME=\"MY-FIELD\" TYPE=\"I32\">10</FIELD>"));
		check("Unexpected JSON", field1.toJSON().equals("{\"NAME\":\"MY-FIELD\",\"TYPE\":\"I32\",\"VALUE\":\"10\"}"));

		// Copy-constructor
		I32Field field2 = new I32Field(field1);
		check("Unexpected field type", field2.getType() == field1.getType());
		check("Unexpected field name", field2.getName().equals(field1.getName()));
		check("Unexpected field value", field2.getValue() == field1.getValue());
		check("Unexpected XML", field2.toXML().equals(field1.toXML()));
		check("Unexpected JSON", field2.toJSON().equals(field1.toJSON()));

		// Off-Nominal test
		try
		{
			// null field name
			new I32Field(null, value);
			check("IllegalArgumentException was expected", false);
		}
		catch (IllegalArgumentException e)
		{
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}

		try
		{
			// empty-string field name
			new I32Field("", value);
			check("IllegalArgumentException was expected", false);
		}
		catch (IllegalArgumentException e)
		{
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}
	}


	private void test_I64Field()
		throws GmsecException
	{
		Log.info("Test I64Field");

		// Nominal test
		String name  = "MY-FIELD";
		long   value = 10;

		// Constructor
		I64Field field1 = new I64Field(name, value);
		check("Unexpected field type", field1.getType() == Field.Type.I64);
		check("Unexpected field name", field1.getName().equals(name));
		check("Unexpected field value", field1.getValue() == value);
		check("Unexpected XML", field1.toXML().equals("<FIELD NAME=\"MY-FIELD\" TYPE=\"I64\">10</FIELD>"));
		check("Unexpected JSON", field1.toJSON().equals("{\"NAME\":\"MY-FIELD\",\"TYPE\":\"I64\",\"VALUE\":\"10\"}"));

		// Copy-constructor
		I64Field field2 = new I64Field(field1);
		check("Unexpected field type", field2.getType() == field1.getType());
		check("Unexpected field name", field2.getName().equals(field1.getName()));
		check("Unexpected field value", field2.getValue() == field1.getValue());
		check("Unexpected XML", field2.toXML().equals(field1.toXML()));
		check("Unexpected JSON", field2.toJSON().equals(field1.toJSON()));

		// Off-Nominal test
		try
		{
			// null field name
			new I64Field(null, value);
			check("IllegalArgumentException was expected", false);
		}
		catch (IllegalArgumentException e)
		{
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}

		try
		{
			// empty-string field name
			new I64Field("", value);
			check("IllegalArgumentException was expected", false);
		}
		catch (IllegalArgumentException e)
		{
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}
	}


	private void test_U8Field()
		throws GmsecException
	{
		Log.info("Test U8Field");

		// Nominal test
		String name  = "MY-FIELD";
		int    value = 10;

		// Constructor
		U8Field field1 = new U8Field(name, new U8(value));
		check("Unexpected field type", field1.getType() == Field.Type.U8);
		check("Unexpected field name", field1.getName().equals(name));
		check("Unexpected field value", field1.getValue().get() == value);
		check("Unexpected XML", field1.toXML().equals("<FIELD NAME=\"MY-FIELD\" TYPE=\"U8\">10</FIELD>"));
		check("Unexpected JSON", field1.toJSON().equals("{\"NAME\":\"MY-FIELD\",\"TYPE\":\"U8\",\"VALUE\":\"10\"}"));


		// Copy-constructor
		U8Field field2 = new U8Field(field1);
		check("Unexpected field type", field2.getType() == field1.getType());
		check("Unexpected field name", field2.getName().equals(field1.getName()));
		check("Unexpected field value", field2.getValue().get() == field1.getValue().get());
		check("Unexpected XML", field2.toXML().equals(field1.toXML()));
		check("Unexpected JSON", field2.toJSON().equals(field1.toJSON()));

		// Off-Nominal test
		try
		{
			// null field name
			new U8Field(null, new U8(value));
			check("IllegalArgumentException was expected", false);
		}
		catch (IllegalArgumentException e)
		{
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}

		try
		{
			// empty-string field name
			new U8Field("", new U8(value));
			check("IllegalArgumentException was expected", false);
		}
		catch (IllegalArgumentException e)
		{
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}

		try
		{
			// null field value
			new U8Field(name, null);
			check("Exception was expected", false);
		}
		catch (IllegalArgumentException e)
		{
			check(e.getMessage(), e.getMessage().contains("U8 object is null"));
		}
	}


	private void test_U16Field()
		throws GmsecException
	{
		Log.info("Test U16Field");

		// Nominal test
		String name  = "MY-FIELD";
		int    value = 10;

		// Constructor
		U16Field field1 = new U16Field(name, new U16(value));
		check("Unexpected field type", field1.getType() == Field.Type.U16);
		check("Unexpected field name", field1.getName().equals(name));
		check("Unexpected field value", field1.getValue().get() == value);
		check("Unexpected XML", field1.toXML().equals("<FIELD NAME=\"MY-FIELD\" TYPE=\"U16\">10</FIELD>"));
		check("Unexpected JSON", field1.toJSON().equals("{\"NAME\":\"MY-FIELD\",\"TYPE\":\"U16\",\"VALUE\":\"10\"}"));

		// Copy-constructor
		U16Field field2 = new U16Field(field1);
		check("Unexpected field type", field2.getType() == field1.getType());
		check("Unexpected field name", field2.getName().equals(field1.getName()));
		check("Unexpected field value", field2.getValue().get() == field1.getValue().get());
		check("Unexpected XML", field2.toXML().equals(field1.toXML()));
		check("Unexpected JSON", field2.toJSON().equals(field1.toJSON()));

		// Off-Nominal test
		try
		{
			// null field name
			new U16Field(null, new U16(value));
			check("IllegalArgumentException was expected", false);
		}
		catch (IllegalArgumentException e)
		{
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}

		try
		{
			// empty-string field name
			new U16Field("", new U16(value));
			check("IllegalArgumentException was expected", false);
		}
		catch (IllegalArgumentException e)
		{
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}

		try
		{
			// null field value
			new U16Field(name, null);
			check("Exception was expected", false);
		}
		catch (IllegalArgumentException e)
		{
			check(e.getMessage(), e.getMessage().contains("U16 object is null"));
		}
	}


	private void test_U32Field()
		throws GmsecException
	{
		Log.info("Test U32Field");

		// Nominal test
		String name  = "MY-FIELD";
		long   value = 10;

		// Constructor
		U32Field field1 = new U32Field(name, new U32(value));
		check("Unexpected field type", field1.getType() == Field.Type.U32);
		check("Unexpected field name", field1.getName().equals(name));
		check("Unexpected field value", field1.getValue().get() == value);
		check("Unexpected XML", field1.toXML().equals("<FIELD NAME=\"MY-FIELD\" TYPE=\"U32\">10</FIELD>"));
		check("Unexpected JSON", field1.toJSON().equals("{\"NAME\":\"MY-FIELD\",\"TYPE\":\"U32\",\"VALUE\":\"10\"}"));

		// Copy-constructor
		U32Field field2 = new U32Field(field1);
		check("Unexpected field type", field2.getType() == field1.getType());
		check("Unexpected field name", field2.getName().equals(field1.getName()));
		check("Unexpected field value", field2.getValue().get() == field1.getValue().get());
		check("Unexpected XML", field2.toXML().equals(field1.toXML()));
		check("Unexpected JSON", field2.toJSON().equals(field1.toJSON()));

		// Off-Nominal test
		try
		{
			// null field name
			new U32Field(null, new U32(value));
			check("IllegalArgumentException was expected", false);
		}
		catch (IllegalArgumentException e)
		{
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}

		try
		{
			// empty-string field name
			new U32Field("", new U32(value));
			check("IllegalArgumentException was expected", false);
		}
		catch (IllegalArgumentException e)
		{
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}

		try
		{
			// null field value
			new U32Field(name, null);
			check("Exception was expected", false);
		}
		catch (IllegalArgumentException e)
		{
			check(e.getMessage(), e.getMessage().contains("U32 object is null"));
		}
	}


	private void test_U64Field()
		throws GmsecException
	{
		Log.info("Test U64Field");

		// Nominal test
		String     name  = "MY-FIELD";
		BigInteger value = new BigInteger("10");

		// Constructor
		U64Field field1 = new U64Field(name, new U64(value));
		check("Unexpected field type", field1.getType() == Field.Type.U64);
		check("Unexpected field name", field1.getName().equals(name));
		check("Unexpected field value", field1.getValue().get().equals(value));
		check("Unexpected XML", field1.toXML().equals("<FIELD NAME=\"MY-FIELD\" TYPE=\"U64\">10</FIELD>"));
		check("Unexpected JSON", field1.toJSON().equals("{\"NAME\":\"MY-FIELD\",\"TYPE\":\"U64\",\"VALUE\":\"10\"}"));

		// Copy-constructor
		U64Field field2 = new U64Field(field1);
		check("Unexpected field type", field2.getType() == field1.getType());
		check("Unexpected field name", field2.getName().equals(field1.getName()));
		check("Unexpected field value", field2.getValue().get().equals(field1.getValue().get()));
		check("Unexpected XML", field2.toXML().equals(field1.toXML()));
		check("Unexpected JSON", field2.toJSON().equals(field1.toJSON()));

		// Off-Nominal test
		try
		{
			// null field name
			new U64Field(null, new U64(value.toString()));
			check("IllegalArgumentException was expected", false);
		}
		catch (IllegalArgumentException e)
		{
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}

		try
		{
			// empty-string field name
			new U64Field("", new U64(value.toString()));
			check("IllegalArgumentException was expected", false);
		}
		catch (IllegalArgumentException e)
		{
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}

		try
		{
			// null field value
			new U64Field(name, null);
			check("Exception was expected", false);
		}
		catch (IllegalArgumentException e)
		{
			check(e.getMessage(), e.getMessage().contains("U64 object is null"));
		}
	}


	private void test_Field()
		throws GmsecException, TestException
	{
		//o getStringValue()
		{
			Log.info("Test Field.getStringValue()");

			I32Field    field1 = new I32Field("I32-FIELD", -1234);
			U32Field    field2 = new U32Field("U32-FIELD", new U32(1234));
			F32Field    field3 = new F32Field("F32-FIELD", 1234.56f);
			StringField field4 = new StringField("STR-FIELD", "Hello World");

			check("Fail getStringValue 1", field1.getStringValue().equals("-1234"));
			check("Fail getStringValue 2", field2.getStringValue().equals("1234"));
			check("Fail getStringValue 3", field3.getStringValue().equals("1234.56"));
			check("Fail getStringValue 4", field4.getStringValue().equals("Hello World"));
		}

		//o getBooleanValue()
		{
			Log.info("Test Field::getBooleanField()");

			BooleanField field1 = new BooleanField("BOOL-FIELD", true);
			U32Field     field2 = new U32Field("U32-FIELD1", new U32(1234));
			I32Field     field3 = new I32Field("I32-FIELD2", -1234);
			F32Field     field4 = new F32Field("F32-FIELD", 0.0f);
			StringField  field5 = new StringField("STR-FIELD1", "FALSE");
			StringField  field6 = new StringField("STR-FIELD2", "true");
			StringField  field7 = new StringField("STR-FIELD3", "Hello World");

			check("Fail getBooleanValue 1", field1.getBooleanValue() == true);
			check("Fail getBooleanValue 2", field2.getBooleanValue() == true);
			check("Fail getBooleanValue 3", field3.getBooleanValue() == true);
			check("Fail getBooleanValue 4", field4.getBooleanValue() == false);
			check("Fail getBooleanValue 5", field5.getBooleanValue() == false);
			check("Fail getBooleanValue 6", field6.getBooleanValue() == true);

			try {
				field7.getBooleanValue();
				check("Expected boolean conversion exception", false);
			}
			catch (GmsecException e) {
				check(e.getMessage(), e.getMessage().contains("Field cannot be represented as a boolean"));
			}
		}

		//o getI16Value()
		{
			Log.info("Test Field.getI16Value()");

			I16Field    field1 = new I16Field("I32-FIELD", (short) -1234);
			U16Field    field2 = new U16Field("U32-FIELD", new U16(1234));
			F32Field    field3 = new F32Field("F32-FIELD", 1234.56f);
			StringField field4 = new StringField("STR-FIELD", "Hello World");

			check("Fail getI16Value 1", field1.getI16Value() == (short) -1234);
			check("Fail getI16Value 2", field2.getI16Value() == (short) 1234);
			check("Fail getI16Value 3", field3.getI16Value() == (short) 1234);

			try {
				field4.getI16Value();
				check("Fail getI16Value 4", false);
			}
			catch (GmsecException e) {
				check(e.getMessage(), e.getMessage().contains("Field cannot be converted to a GMSEC_I16"));
			}
		}

		//o getI32Value()
		{
			Log.info("Test Field.getI32Value()");

			I32Field    field1 = new I32Field("I32-FIELD", -1234);
			U32Field    field2 = new U32Field("U32-FIELD", new U32(1234));
			F32Field    field3 = new F32Field("F32-FIELD", 1234.56f);
			StringField field4 = new StringField("STR-FIELD", "Hello World");

			check("Fail getI32Value 1", field1.getI32Value() == -1234);
			check("Fail getI32Value 2", field2.getI32Value() == 1234);
			check("Fail getI32Value 3", field3.getI32Value() == 1234);

			try {
				field4.getI32Value();
				check("Fail getI32Value 4", false);
			}
			catch (GmsecException e) {
				check(e.getMessage(), e.getMessage().contains("Field cannot be converted to a GMSEC_I32"));
			}
		}

		//o getI64Value()
		{
			Log.info("Test Field.getI64Value()");

			I32Field    field1 = new I32Field("I32-FIELD", -1234);
			U32Field    field2 = new U32Field("U32-FIELD", new U32(1234));
			F32Field    field3 = new F32Field("F32-FIELD", 1234.56f);
			StringField field4 = new StringField("STR-FIELD", "Hello World");

			check("Fail getI64Value 1", field1.getI64Value() == -1234);
			check("Fail getI64Value 2", field2.getI64Value() == 1234);
			check("Fail getI64Value 3", field3.getI64Value() == 1234);

			try {
				field4.getI64Value();
				check("Fail getI64Value 4", false);
			}
			catch (GmsecException e) {
				check(e.getMessage(), e.getMessage().contains("Field cannot be converted to a GMSEC_I64"));
			}
		}

		//o getU16Value()
		{
			Log.info("Test Field.getU16Value()");

			I16Field    field1 = new I16Field("I32-FIELD", (short) 1234);
			U16Field    field2 = new U16Field("U32-FIELD", new U16(1234));
			F32Field    field3 = new F32Field("F32-FIELD", 1234.56f);
			StringField field4 = new StringField("STR-FIELD", "Hello World");

			check("Fail getU16Value 1", field1.getU16Value().toString().equals("1234"));
			check("Fail getU16Value 2", field2.getU16Value().toString().equals("1234"));
			check("Fail getU16Value 3", field3.getU16Value().toString().equals("1234"));

			try {
				field4.getU16Value();
				check("Fail getU16Value 4", false);
			}
			catch (GmsecException e) {
				check(e.getMessage(), e.getMessage().contains("Field cannot be converted to a GMSEC_U16"));
			}
		}

		//o getU32Value()
		{
			Log.info("Test Field.getU32Value()");

			I32Field    field1 = new I32Field("I32-FIELD", 1234);
			U32Field    field2 = new U32Field("U32-FIELD", new U32(1234));
			F32Field    field3 = new F32Field("F32-FIELD", 1234.56f);
			StringField field4 = new StringField("STR-FIELD", "Hello World");

			check("Fail getU32Value 1", field1.getU32Value().toString().equals("1234"));
			check("Fail getU32Value 2", field2.getU32Value().toString().equals("1234"));
			check("Fail getU32Value 3", field3.getU32Value().toString().equals("1234"));

			try {
				field4.getU32Value();
				check("Fail getU32Value 4", false);
			}
			catch (GmsecException e) {
				check(e.getMessage(), e.getMessage().contains("Field cannot be converted to a GMSEC_U32"));
			}
		}

		//o getU64Value()
		{
			Log.info("Test Field.getU64Value()");

			I32Field    field1 = new I32Field("I32-FIELD", 1234);
			U32Field    field2 = new U32Field("U32-FIELD", new U32(1234));
			F32Field    field3 = new F32Field("F32-FIELD", 1234.56f);
			StringField field4 = new StringField("STR-FIELD", "Hello World");

			check("Fail getU64Value 1", field1.getU64Value().toString().equals("1234"));
			check("Fail getU64Value 2", field2.getU64Value().toString().equals("1234"));
			check("Fail getU64Value 3", field3.getU64Value().toString().equals("1234"));

			try {
				field4.getU64Value();
				check("Fail getU64Value 4", false);
			}
			catch (GmsecException e) {
				check(e.getMessage(), e.getMessage().contains("Field cannot be converted to a GMSEC_U64"));
			}
		}

		//o getF64Value()
		{
			Log.info("Test Field.getF64Value()");

			I32Field    field1 = new I32Field("I32-FIELD", -1234);
			U32Field    field2 = new U32Field("U32-FIELD", new U32(1234));
			F32Field    field3 = new F32Field("F32-FIELD", 1234.56f);
			StringField field4 = new StringField("STR-FIELD", "Hello World");

			check("Fail getF64Value 1", field1.getF64Value() == -1234.0);
			check("Fail getF64Value 2", field2.getF64Value() == 1234.0);
			check("Fail getF64Value 3", field3.getF64Value() == 1234.56);

			try {
				field4.getF64Value();
				check("Fail getF64Value 4", false);
			}
			catch (GmsecException  e) {
				check(e.getMessage(), e.getMessage().contains("Field cannot be converted to a GMSEC_F64"));
			}
		}
	}
}
