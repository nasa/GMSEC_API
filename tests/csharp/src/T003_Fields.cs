namespace T003
{
	using GMSEC.API5;
	using GMSEC.API5.Testing.Common;
	using System;
	using System.Text;


	class T003_Fields : TestCase
	{
		static void Main(string[] args)
		{
			T003_Fields test = new T003_Fields();
			test.SetDescription("Test Fields");
			test.Run(args);
		}


		public override void Run()
		{
			try
			{
				Test_BinaryField();
				Test_BooleanField();
				Test_CharField();
				Test_StringField();

				Test_F32Field();
				Test_F64Field();

				Test_I8Field();
				Test_I16Field();
				Test_I32Field();
				Test_I64Field();

				Test_U8Field();
				Test_U16Field();
				Test_U32Field();
				Test_U64Field();

				Test_Field();
			}
			catch (GmsecException e)
			{
				Require(e.ToString(), false);
			}
		}


		private void Test_BinaryField()
		{
			Log.Info("Test BinaryField");

			// Nominal test
			string name = "MY-FIELD";
			byte[] data = new byte[4];
			for (int i = 0; i < data.Length; ++i) {
				data[i] = (byte)(i & 0xFF);
			}

			// Constructor 1
			BinaryField field1 = new BinaryField(name, data);
			Check("Unexpected header type", field1.IsHeader() == false);
			Check("Unexpected field type", field1.GetFieldType() == Field.Type.BINARY);
			Check("Unexpected field name", field1.GetName() == name);
			Check("Unexpected data length", field1.GetValue().Length == data.Length);

			byte[] bytes = field1.GetValue();
			for (int i = 0; i < data.Length; ++i) {
				Check("Unexpected data at position " + i.ToString(), bytes[i] == data[i]);
			}

			Check("Unexpected XML", field1.ToXML() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"BIN\">00010203</FIELD>");
			Check("Unexpected JSON", field1.ToJSON() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"BIN\",\"VALUE\":\"00010203\"}");

			// Constructor 2
			BinaryField field2 = new BinaryField(name, data, true);
			Check("Unexpected header type", field2.IsHeader() == true);
			Check("Unexpected field type", field2.GetFieldType() == Field.Type.BINARY);
			Check("Unexpected field name", field2.GetName() == name);
			Check("Unexpected data length", field2.GetValue().Length == data.Length);

			bytes = field2.GetValue();
			for (int i = 0; i < data.Length; ++i) {
				Check("Unexpected data at position " + i.ToString(), bytes[i] == data[i]);
			}

			Check("Unexpected XML", field2.ToXML() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"BIN\">00010203</FIELD>");
			Check("Unexpected JSON", field2.ToJSON() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"BIN\",\"VALUE\":\"00010203\"}");

			// Copy constructor
			BinaryField field3 = new BinaryField(field2);
			Check("Unexpected header type", field3.IsHeader() == field2.IsHeader());
			Check("Unexpected field type", field3.GetFieldType() == field2.GetFieldType());
			Check("Unexpected field name", field3.GetName() == field2.GetName());
			Check("Unexpected data length", field3.GetValue().Length == field2.GetValue().Length);

			bytes = field3.GetValue();
			for (int i = 0; i < data.Length; ++i) {
				Check("Unexpected data at position " + i.ToString(), bytes[i] == data[i]);
			}

			Check("Unexpected XML", field3.ToXML() == field2.ToXML());
			Check("Unexpected JSON", field3.ToJSON() == field2.ToJSON());

			// Off-Nominal test
			try
			{
				// null field name
				new BinaryField(null, data);
				Check("IllegalArgumentException was expected", false);
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), e.ToString().Contains("Field name is not compliant"));
			}

			try
			{
				// empty-string field name
				new BinaryField("", data);
				Check("IllegalArgumentException was expected", false);
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), e.ToString().Contains("Field name is not compliant"));
			}
		}


		private void Test_BooleanField()
		{
			Log.Info("Test BooleanField");

			// Nominal test
			string  name  = "MY-FIELD";
			bool    value = true;

			// Constructor
			BooleanField field1 = new BooleanField(name, value);
			Check("Unexpected field type", field1.GetFieldType() == Field.Type.BOOL);
			Check("Unexpected field name", field1.GetName() == name);
			Check("Unexpected field value", field1.GetValue() == value);
			Check("Unexpected XML", field1.ToXML() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"BOOL\">TRUE</FIELD>");
			Check("Unexpected JSON", field1.ToJSON() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"BOOL\",\"VALUE\":\"TRUE\"}");


			// Copy-constructor
			BooleanField field2 = new BooleanField(field1);
			Check("Unexpected field type", field2.GetFieldType() == field1.GetFieldType());
			Check("Unexpected field name", field2.GetName() == field1.GetName());
			Check("Unexpected field value", field2.GetValue() == field1.GetValue());
			Check("Unexpected XML", field2.ToXML() == field1.ToXML());
			Check("Unexpected JSON", field2.ToJSON() == field1.ToJSON());

			// Off-Nominal test
			try
			{
				// null field name
				new BooleanField(null, value);
				Check("IllegalArgumentException was expected", false);
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), e.ToString().Contains("Field name is not compliant"));
			}

			try
			{
				// empty-string field name
				new BooleanField("", value);
				Check("IllegalArgumentException was expected", false);
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), e.ToString().Contains("Field name is not compliant"));
			}
		}


		private void Test_CharField()
		{
			Log.Info("Test CharField");

			// Nominal test
			string name  = "MY-FIELD";
			char   value = 'f';

			// Constructor
			CharField field1 = new CharField(name, value);
			Check("Unexpected field type", field1.GetFieldType() == Field.Type.CHAR);
			Check("Unexpected field name", field1.GetName() == name);
			Check("Unexpected field value", field1.GetValue() == value);
			Check("Unexpected XML", field1.ToXML() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"CHAR\">f</FIELD>");
			Check("Unexpected JSON", field1.ToJSON() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"CHAR\",\"VALUE\":\"f\"}");


			// Copy-constructor
			CharField field2 = new CharField(field1);
			Check("Unexpected field type", field2.GetFieldType() == field1.GetFieldType());
			Check("Unexpected field name", field2.GetName() == field1.GetName());
			Check("Unexpected field value", field2.GetValue() == field1.GetValue());
			Check("Unexpected XML", field2.ToXML() == field1.ToXML());
			Check("Unexpected JSON", field2.ToJSON() == field1.ToJSON());

			// Off-Nominal test
			try
			{
				// null field name
				new CharField(null, value);
				Check("IllegalArgumentException was expected", false);
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), e.ToString().Contains("Field name is not compliant"));
			}

			try
			{
				// empty-string field name
				new CharField("", value);
				Check("IllegalArgumentException was expected", false);
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), e.ToString().Contains("Field name is not compliant"));
			}
		}


		private void Test_StringField()
		{
			Log.Info("Test StringField");

			// Nominal test
			string name  = "MY-FIELD";
			string value = "Test String";

			// Constructor
			StringField field1 = new StringField(name, value);
			Check("Unexpected field type", field1.GetFieldType() == Field.Type.STRING);
			Check("Unexpected field name", field1.GetName() == name);
			Check("Unexpected field value", field1.GetValue() == value);
			Check("Unexpected XML", field1.ToXML() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"STRING\">Test String</FIELD>");
			Check("Unexpected JSON", field1.ToJSON() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"STRING\",\"VALUE\":\"Test String\"}");


			// Copy-constructor
			StringField field2 = new StringField(field1);
			Check("Unexpected field type", field2.GetFieldType() == field1.GetFieldType());
			Check("Unexpected field name", field2.GetName() == field1.GetName());
			Check("Unexpected field value", field2.GetValue() == field1.GetValue());
			Check("Unexpected XML", field2.ToXML() == field1.ToXML());
			Check("Unexpected JSON", field2.ToJSON() == field1.ToJSON());

			// Off-Nominal test
			try
			{
				// null field name
				new StringField(null, value);
				Check("GmsecException was expected", false);
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), e.ToString().Contains("Field name is not compliant"));
			}

			try
			{
				// empty-string field name
				new StringField("", value);
				Check("IllegalArgumentException was expected", false);
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), e.ToString().Contains("Field name is not compliant"));
			}
		}


		private void Test_F32Field()
		{
			Log.Info("Test F32Field");

			// Nominal test
			string name  = "MY-FIELD";
			float  value = 10;

			// Constructor
			F32Field field1 = new F32Field(name, value);
			Check("Unexpected field type", field1.GetFieldType() == Field.Type.F32);
			Check("Unexpected field name", field1.GetName() == name);
			Check("Unexpected field value", field1.GetValue() == value);
			Check("Unexpected XML", field1.ToXML() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"F32\" BITS=\"41200000\">10</FIELD>");
			Check("Unexpected JSON", field1.ToJSON() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"F32\",\"BITS\":\"41200000\",\"VALUE\":\"10\"}");


			// Copy-constructor
			F32Field field2 = new F32Field(field1);
			Check("Unexpected field type", field2.GetFieldType() == field1.GetFieldType());
			Check("Unexpected field name", field2.GetName() == field1.GetName());
			Check("Unexpected field value", field2.GetValue() == field1.GetValue());
			Check("Unexpected XML", field2.ToXML() == field1.ToXML());
			Check("Unexpected JSON", field2.ToJSON() == field1.ToJSON());

			// Off-Nominal test
			try
			{
				// null field name
				new F32Field(null, value);
				Check("GmsecException was expected", false);
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), e.ToString().Contains("Field name is not compliant"));
			}

			try
			{
				// empty-string field name
				new F32Field("", value);
				Check("GmsecException was expected", false);
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), e.ToString().Contains("Field name is not compliant"));
			}
		}


		private void Test_F64Field()
		{
			Log.Info("Test F64Field");

			// Nominal test
			string name  = "MY-FIELD";
			double value = 10;

			// Constructor
			F64Field field1 = new F64Field(name, value);
			Check("Unexpected field type", field1.GetFieldType() == Field.Type.F64);
			Check("Unexpected field name", field1.GetName() == name);
			Check("Unexpected field value", field1.GetValue() == value);
			Check("Unexpected XML", field1.ToXML() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"F64\" BITS=\"4024000000000000\">10</FIELD>");
			Check("Unexpected JSON", field1.ToJSON() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"F64\",\"BITS\":\"4024000000000000\",\"VALUE\":\"10\"}");


			// Copy-constructor
			F64Field field2 = new F64Field(field1);
			Check("Unexpected field type", field2.GetFieldType() == field1.GetFieldType());
			Check("Unexpected field name", field2.GetName() == field1.GetName());
			Check("Unexpected field value", field2.GetValue() == field1.GetValue());
			Check("Unexpected XML", field2.ToXML() == field1.ToXML());
			Check("Unexpected JSON", field2.ToJSON() == field1.ToJSON());

			// Off-Nominal test
			try
			{
				// null field name
				new F64Field(null, value);
				Check("GmsecException was expected", false);
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), e.ToString().Contains("Field name is not compliant"));
			}

			try
			{
				// empty-string field name
				new F64Field("", value);
				Check("GmsecException was expected", false);
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), e.ToString().Contains("Field name is not compliant"));
			}
		}


		private void Test_I8Field()
		{
			Log.Info("Test I8Field");

			// Nominal test
			string name  = "MY-FIELD";
			sbyte  value = 10;

			// Constructor
			I8Field field1 = new I8Field(name, value);
			Check("Unexpected field type", field1.GetFieldType() == Field.Type.I8);
			Check("Unexpected field name", field1.GetName() == name);
			Check("Unexpected field value", field1.GetValue() == value);
			Check("Unexpected XML", field1.ToXML() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"I8\">10</FIELD>");
			Check("Unexpected JSON", field1.ToJSON() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"I8\",\"VALUE\":\"10\"}");


			// Copy-constructor
			I8Field field2 = new I8Field(field1);
			Check("Unexpected field type", field2.GetFieldType() == field1.GetFieldType());
			Check("Unexpected field name", field2.GetName() == field1.GetName());
			Check("Unexpected field value", field2.GetValue() == field1.GetValue());
			Check("Unexpected XML", field2.ToXML() == field1.ToXML());
			Check("Unexpected JSON", field2.ToJSON() == field1.ToJSON());

			// Off-Nominal test
			try
			{
				// null field name
				new I8Field(null, value);
				Check("GmsecException was expected", false);
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), e.ToString().Contains("Field name is not compliant"));
			}

			try
			{
				// empty-string field name
				new I8Field("", value);
				Check("GmsecException was expected", false);
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), e.ToString().Contains("Field name is not compliant"));
			}
		}


		private void Test_I16Field()
		{
			Log.Info("Test I16Field");

			// Nominal test
			string name  = "MY-FIELD";
			short  value = 10;

			// Constructor
			I16Field field1 = new I16Field(name, value);
			Check("Unexpected field type", field1.GetFieldType() == Field.Type.I16);
			Check("Unexpected field name", field1.GetName() == name);
			Check("Unexpected field value", field1.GetValue() == value);
			Check("Unexpected XML", field1.ToXML() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"I16\">10</FIELD>");
			Check("Unexpected JSON", field1.ToJSON() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"I16\",\"VALUE\":\"10\"}");


			// Copy-constructor
			I16Field field2 = new I16Field(field1);
			Check("Unexpected field type", field2.GetFieldType() == field1.GetFieldType());
			Check("Unexpected field name", field2.GetName() == field1.GetName());
			Check("Unexpected field value", field2.GetValue() == field1.GetValue());
			Check("Unexpected XML", field2.ToXML() == field1.ToXML());
			Check("Unexpected JSON", field2.ToJSON() == field1.ToJSON());

			// Off-Nominal test
			try
			{
				// null field name
				new I16Field(null, value);
				Check("GmsecException was expected", false);
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), e.ToString().Contains("Field name is not compliant"));
			}

			try
			{
				// empty-string field name
				new I16Field("", value);
				Check("GmsecException was expected", false);
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), e.ToString().Contains("Field name is not compliant"));
			}
		}


		private void Test_I32Field()
		{
			Log.Info("Test I32Field");

			// Nominal test
			string name  = "MY-FIELD";
			int    value = 10;

			// Constructor
			I32Field field1 = new I32Field(name, value);
			Check("Unexpected field type", field1.GetFieldType() == Field.Type.I32);
			Check("Unexpected field name", field1.GetName() == name);
			Check("Unexpected field value", field1.GetValue() == value);
			Check("Unexpected XML", field1.ToXML() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"I32\">10</FIELD>");
			Check("Unexpected JSON", field1.ToJSON() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"I32\",\"VALUE\":\"10\"}");


			// Copy-constructor
			I32Field field2 = new I32Field(field1);
			Check("Unexpected field type", field2.GetFieldType() == field1.GetFieldType());
			Check("Unexpected field name", field2.GetName() == field1.GetName());
			Check("Unexpected field value", field2.GetValue() == field1.GetValue());
			Check("Unexpected XML", field2.ToXML() == field1.ToXML());
			Check("Unexpected JSON", field2.ToJSON() == field1.ToJSON());

			// Off-Nominal test
			try
			{
				// null field name
				new I32Field(null, value);
				Check("GmsecException was expected", false);
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), e.ToString().Contains("Field name is not compliant"));
			}

			try
			{
				// empty-string field name
				new I32Field("", value);
				Check("GmsecException was expected", false);
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), e.ToString().Contains("Field name is not compliant"));
			}
		}


		private void Test_I64Field()
		{
			Log.Info("Test I64Field");

			// Nominal test
			string name  = "MY-FIELD";
			long   value = 10;

			// Constructor
			I64Field field1 = new I64Field(name, value);
			Check("Unexpected field type", field1.GetFieldType() == Field.Type.I64);
			Check("Unexpected field name", field1.GetName() == name);
			Check("Unexpected field value", field1.GetValue() == value);
			Check("Unexpected XML", field1.ToXML() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"I64\">10</FIELD>");
			Check("Unexpected JSON", field1.ToJSON() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"I64\",\"VALUE\":\"10\"}");


			// Copy-constructor
			I64Field field2 = new I64Field(field1);
			Check("Unexpected field type", field2.GetFieldType() == field1.GetFieldType());
			Check("Unexpected field name", field2.GetName() == field1.GetName());
			Check("Unexpected field value", field2.GetValue() == field1.GetValue());
			Check("Unexpected XML", field2.ToXML() == field1.ToXML());
			Check("Unexpected JSON", field2.ToJSON() == field1.ToJSON());

			// Off-Nominal test
			try
			{
				// null field name
				new I64Field(null, value);
				Check("GmsecException was expected", false);
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), e.ToString().Contains("Field name is not compliant"));
			}

			try
			{
				// empty-string field name
				new I64Field("", value);
				Check("GmsecException was expected", false);
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), e.ToString().Contains("Field name is not compliant"));
			}
		}


		private void Test_U8Field()
		{
			Log.Info("Test U8Field");

			// Nominal test
			string name  = "MY-FIELD";
			byte   value = 10;

			// Constructor
			U8Field field1 = new U8Field(name, value);
			Check("Unexpected field type", field1.GetFieldType() == Field.Type.U8);
			Check("Unexpected field name", field1.GetName() == name);
			Check("Unexpected field value", field1.GetValue() == value);
			Check("Unexpected XML", field1.ToXML() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"U8\">10</FIELD>");
			Check("Unexpected JSON", field1.ToJSON() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"U8\",\"VALUE\":\"10\"}");


			// Copy-constructor
			U8Field field2 = new U8Field(field1);
			Check("Unexpected field type", field2.GetFieldType() == field1.GetFieldType());
			Check("Unexpected field name", field2.GetName() == field1.GetName());
			Check("Unexpected field value", field2.GetValue() == field1.GetValue());
			Check("Unexpected XML", field2.ToXML() == field1.ToXML());
			Check("Unexpected JSON", field2.ToJSON() == field1.ToJSON());

			// Off-Nominal test
			try
			{
				// null field name
				new U8Field(null, value);
				Check("GmsecException was expected", false);
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), e.ToString().Contains("Field name is not compliant"));
			}

			try
			{
				// empty-string field name
				new U8Field("", value);
				Check("GmsecException was expected", false);
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), e.ToString().Contains("Field name is not compliant"));
			}
		}


		private void Test_U16Field()
		{
			Log.Info("Test U16Field");

			// Nominal test
			string name  = "MY-FIELD";
			ushort value = 10;

			// Constructor
			U16Field field1 = new U16Field(name, value);
			Check("Unexpected field type", field1.GetFieldType() == Field.Type.U16);
			Check("Unexpected field name", field1.GetName() == name);
			Check("Unexpected field value", field1.GetValue() == value);
			Check("Unexpected XML", field1.ToXML() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"U16\">10</FIELD>");
			Check("Unexpected JSON", field1.ToJSON() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"U16\",\"VALUE\":\"10\"}");


			// Copy-constructor
			U16Field field2 = new U16Field(field1);
			Check("Unexpected field type", field2.GetFieldType() == field1.GetFieldType());
			Check("Unexpected field name", field2.GetName() == field1.GetName());
			Check("Unexpected field value", field2.GetValue() == field1.GetValue());
			Check("Unexpected XML", field2.ToXML() == field1.ToXML());
			Check("Unexpected JSON", field2.ToJSON() == field1.ToJSON());

			// Off-Nominal test
			try
			{
				// null field name
				new U16Field(null, value);
				Check("GmsecException was expected", false);
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), e.ToString().Contains("Field name is not compliant"));
			}

			try
			{
				// empty-string field name
				new U16Field("", value);
				Check("GmsecException was expected", false);
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), e.ToString().Contains("Field name is not compliant"));
			}
		}


		private void Test_U32Field()
		{
			Log.Info("Test U32Field");

			// Nominal test
			string name  = "MY-FIELD";
			uint   value = 10;

			// Constructor
			U32Field field1 = new U32Field(name, value);
			Check("Unexpected field type", field1.GetFieldType() == Field.Type.U32);
			Check("Unexpected field name", field1.GetName() == name);
			Check("Unexpected field value", field1.GetValue() == value);
			Check("Unexpected XML", field1.ToXML() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"U32\">10</FIELD>");
			Check("Unexpected JSON", field1.ToJSON() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"U32\",\"VALUE\":\"10\"}");


			// Copy-constructor
			U32Field field2 = new U32Field(field1);
			Check("Unexpected field type", field2.GetFieldType() == field1.GetFieldType());
			Check("Unexpected field name", field2.GetName() == field1.GetName());
			Check("Unexpected field value", field2.GetValue() == field1.GetValue());
			Check("Unexpected XML", field2.ToXML() == field1.ToXML());
			Check("Unexpected JSON", field2.ToJSON() == field1.ToJSON());

			// Off-Nominal test
			try
			{
				// null field name
				new U32Field(null, value);
				Check("GmsecException was expected", false);
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), e.ToString().Contains("Field name is not compliant"));
			}

			try
			{
				// empty-string field name
				new U32Field("", value);
				Check("GmsecException was expected", false);
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), e.ToString().Contains("Field name is not compliant"));
			}
		}


		private void Test_U64Field()
		{
			Log.Info("Test U64Field");

			// Nominal test
			string name  = "MY-FIELD";
			ulong  value = 10;

			// Constructor
			U64Field field1 = new U64Field(name, value);
			Check("Unexpected field type", field1.GetFieldType() == Field.Type.U64);
			Check("Unexpected field name", field1.GetName() == name);
			Check("Unexpected field value", field1.GetValue() == value);
			Check("Unexpected XML", field1.ToXML() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"U64\">10</FIELD>");
			Check("Unexpected JSON", field1.ToJSON() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"U64\",\"VALUE\":\"10\"}");


			// Copy-constructor
			U64Field field2 = new U64Field(field1);
			Check("Unexpected field type", field2.GetFieldType() == field1.GetFieldType());
			Check("Unexpected field name", field2.GetName() == field1.GetName());
			Check("Unexpected field value", field2.GetValue() == field1.GetValue());
			Check("Unexpected XML", field2.ToXML() == field1.ToXML());
			Check("Unexpected JSON", field2.ToJSON() == field1.ToJSON());

			// Off-Nominal test
			try
			{
				// null field name
				new U64Field(null, value);
				Check("GmsecException was expected", false);
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), e.ToString().Contains("Field name is not compliant"));
			}

			try
			{
				// empty-string field name
				new U64Field("", value);
				Check("GmsecException was expected", false);
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), e.ToString().Contains("Field name is not compliant"));
			}
		}


		private void Test_Field()
		{
			//o GetStringValue()
			{
				Log.Info("Test Field.GetStringField()");

				I32Field    field1 = new I32Field("I32-FIELD", (int) -1234);
				U32Field    field2 = new U32Field("U32-FIELD", (uint) 1234);
				F32Field    field3 = new F32Field("F32-FIELD", 1234.56f);
				StringField field4 = new StringField("STR-FIELD", "Hello World");

				Check("Fail GetStringValue 1", field1.GetStringValue() == "-1234");
				Check("Fail GetStringValue 2", field2.GetStringValue() == "1234");
				Check("Fail GetStringValue 3", field3.GetStringValue() == "1234.56");
				Check("Fail GetStringValue 4", field4.GetStringValue() == "Hello World");
			}

			//o getBooleanValue()
			{
				Log.Info("Test Field.GetBooleanField()");

				BooleanField field1 = new BooleanField("BOOL-FIELD", true);
				U32Field     field2 = new U32Field("U32-FIELD1", (uint) 1234);
				I32Field     field3 = new I32Field("I32-FIELD2", (int) -1234);
				F32Field     field4 = new F32Field("F32-FIELD", 0.0f);
				StringField  field5 = new StringField("STR-FIELD1", "FALSE");
				StringField  field6 = new StringField("STR-FIELD2", "true");
				StringField  field7 = new StringField("STR-FIELD3", "Hello World");

				Check("Fail getBooleanValue 1", field1.GetBooleanValue() == true);
				Check("Fail getBooleanValue 2", field2.GetBooleanValue() == true);
				Check("Fail getBooleanValue 3", field3.GetBooleanValue() == true);
				Check("Fail getBooleanValue 4", field4.GetBooleanValue() == false);
				Check("Fail getBooleanValue 5", field5.GetBooleanValue() == false);
				Check("Fail getBooleanValue 6", field6.GetBooleanValue() == true);

				try {
					field7.GetBooleanValue();
					Check("Expected boolean conversion exception", false);
				}
				catch (GmsecException e) {
					Check(e.ToString(), e.ToString().Contains("Field cannot be represented as a boolean"));
				}
			}

			//o GetI16Value()
			{
				Log.Info("Test Field.GetI16Field()");

				I16Field    field1 = new I16Field("I32-FIELD", (short) -1234);
				U16Field    field2 = new U16Field("U32-FIELD", (ushort) 1234);
				F32Field    field3 = new F32Field("F32-FIELD", 1234.56f);
				StringField field4 = new StringField("STR-FIELD", "Hello World");

				Check("Fail GetI16Value 1", field1.GetI16Value() == (short) -1234);
				Check("Fail GetI16Value 2", field2.GetI16Value() == (short) 1234);
				Check("Fail GetI16Value 3", field3.GetI16Value() == (short) 1234);

				try {
					field4.GetI16Value();
					Check("Fail GetI16Value 4", false);
				}
				catch (GmsecException e) {
					Check(e.ToString(), e.ToString().Contains("Field cannot be converted to a GMSEC_I16"));
				}
			}

			//o GetI32Value()
			{
				Log.Info("Test Field.GetI32Field()");

				I32Field    field1 = new I32Field("I32-FIELD", (int) -1234);
				U32Field    field2 = new U32Field("U32-FIELD", (uint) 1234);
				F32Field    field3 = new F32Field("F32-FIELD", 1234.56f);
				StringField field4 = new StringField("STR-FIELD", "Hello World");

				Check("Fail GetI32Value 1", field1.GetI32Value() == (int) -1234);
				Check("Fail GetI32Value 2", field2.GetI32Value() == (uint) 1234);
				Check("Fail GetI32Value 3", field3.GetI32Value() == (int) 1234);

				try {
					field4.GetI32Value();
					Check("Fail GetI32Value 4", false);
				}
				catch (GmsecException e) {
					Check(e.ToString(), e.ToString().Contains("Field cannot be converted to a GMSEC_I32"));
				}
			}

			//o GetI64Value()
			{
				Log.Info("Test Field.GetI64Field()");

				I32Field    field1 = new I32Field("I32-FIELD", (int) -1234);
				U32Field    field2 = new U32Field("U32-FIELD", (uint) 1234);
				F32Field    field3 = new F32Field("F32-FIELD", 1234.56f);
				StringField field4 = new StringField("STR-FIELD", "Hello World");

				Check("Fail GetI64Value 1", field1.GetI64Value() == (long) -1234);
				Check("Fail GetI64Value 2", field2.GetI64Value() == (long) 1234);
				Check("Fail GetI64Value 3", field3.GetI64Value() == (long) 1234);

				try {
					field4.GetI64Value();
					Check("Fail GetI64Value 4", false);
				}
				catch (GmsecException e) {
					Check(e.ToString(), e.ToString().Contains("Field cannot be converted to a GMSEC_I64"));
				}
			}

			//o GetU16Value()
			{
				Log.Info("Test Field.GetU16Field()");

				I16Field    field1 = new I16Field("I32-FIELD", (short) 1234);
				U16Field    field2 = new U16Field("U32-FIELD", (ushort) 1234);
				F32Field    field3 = new F32Field("F32-FIELD", 1234.56f);
				StringField field4 = new StringField("STR-FIELD", "Hello World");

				Check("Fail GetU16Value 1", field1.GetU16Value() == (ushort) 1234);
				Check("Fail GetU16Value 2", field2.GetU16Value() == (ushort) 1234);
				Check("Fail GetU16Value 3", field3.GetU16Value() == (ushort) 1234);

				try {
					field4.GetU16Value();
					Check("Fail GetU16Value 4", false);
				}
				catch (GmsecException e) {
					Check(e.ToString(), e.ToString().Contains("Field cannot be converted to a GMSEC_U16"));
				}
			}

			//o GetU32Value()
			{
				Log.Info("Test Field.GetU32Field()");

				I32Field    field1 = new I32Field("I32-FIELD", (int) 1234);
				U32Field    field2 = new U32Field("U32-FIELD", (uint) 1234);
				F32Field    field3 = new F32Field("F32-FIELD", 1234.56f);
				StringField field4 = new StringField("STR-FIELD", "Hello World");

				Check("Fail GetU32Value 1", field1.GetU32Value() == (uint) 1234);
				Check("Fail GetU32Value 2", field2.GetU32Value() == (uint) 1234);
				Check("Fail GetU32Value 3", field3.GetU32Value() == (uint) 1234);

				try {
					field4.GetU32Value();
					Check("Fail GetU32Value 4", false);
				}
				catch (GmsecException e) {
					Check(e.ToString(), e.ToString().Contains("Field cannot be converted to a GMSEC_U32"));
				}
			}

			//o GetU64Value()
			{
				Log.Info("Test Field.GetU64Field()");

				I32Field    field1 = new I32Field("I32-FIELD", (int) 1234);
				U32Field    field2 = new U32Field("U32-FIELD", (uint) 1234);
				F32Field    field3 = new F32Field("F32-FIELD", 1234.56f);
				StringField field4 = new StringField("STR-FIELD", "Hello World");

				Check("Fail GetU64Value 1", field1.GetU64Value() == (ulong) 1234);
				Check("Fail GetU64Value 2", field2.GetU64Value() == (ulong) 1234);
				Check("Fail GetU64Value 3", field3.GetU64Value() == (ulong) 1234);

				try {
					field4.GetU64Value();
					Check("Fail GetU64Value 4", false);
				}
				catch (GmsecException e) {
					Check(e.ToString(), e.ToString().Contains("Field cannot be converted to a GMSEC_U64"));
				}
			}

			//o GetF64Value()
			{
				Log.Info("Test Field.GetF64Field()");

				I32Field    field1 = new I32Field("I32-FIELD", (int) -1234);
				U32Field    field2 = new U32Field("U32-FIELD", (uint) 1234);
				F32Field    field3 = new F32Field("F32-FIELD", 1234.56f);
				StringField field4 = new StringField("STR-FIELD", "Hello World");

				Check("Fail GetF64Value 1", field1.GetF64Value() == -1234.0);
				Check("Fail GetF64Value 2", field2.GetF64Value() == 1234.0);
				Check("Fail GetF64Value 3", field3.GetF64Value() == 1234.56);

				try {
					field4.GetF64Value();
					Check("Fail GetF64Value 4", false);
				}
				catch (GmsecException  e) {
					Check(e.ToString(), e.ToString().Contains("Field cannot be converted to a GMSEC_F64"));
				}
			}
		}
	}
}
