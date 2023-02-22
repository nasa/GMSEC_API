namespace T005
{
	using GMSEC.API5;
	using GMSEC.API5.Testing.Common;
	using System;
	using System.Text;


	class T005_Message : TestCase
	{
		static void Main(string[] args)
		{
			T005_Message test = new T005_Message();
			test.SetDescription("Test Message");
			test.Run(args);
		}


		public override void Run()
		{
			try
			{
				Test_Constructor();
				Test_CopyConstructor();
				Test_GetSchemaID();
				Test_GetVersion();
				Test_GetSchemaLevel();
				Test_IsCompliant();
				Test_RegisterMessageValidator();
				Test_SetFieldValue();
				Test_SetConfig();
				Test_SetSubject();
				Test_GetSubject();
				Test_SetKind();
				Test_GetKind();
				Test_AddField();
				Test_AddFields();
				Test_ClearFields();
				Test_ClearField();
				Test_HasField();
				Test_GetStringValue();
				Test_GetBooleanValue();
				Test_GetI16Value();
				Test_GetI32Value();
				Test_GetI64Value();
				Test_GetU16Value();
				Test_GetU32Value();
				Test_GetU64Value();
				Test_GetF64Value();
				Test_GetField();
				Test_GetFieldType();
				Test_GetBinaryField();
				Test_GetBooleanField();
				Test_GetCharField();
				Test_GetF32Field();
				Test_GetF64Field();
				Test_GetI8Field();
				Test_GetI16Field();
				Test_GetI32Field();
				Test_GetI64Field();
				Test_GetU8Field();
				Test_GetU16Field();
				Test_GetU32Field();
				Test_GetU64Field();
				Test_GetStringField();
				Test_GetFieldCount();
				Test_CopyFields();
				Test_ToXML();
				Test_ToJSON();
				Test_GetSize();
				Test_GetFieldIterator();
			}
			catch (GmsecException e)
			{
				Require(e.ToString(), false);
			}
		}


		private void Test_Constructor()
		{
			Log.Info("Test constructor");

			Message msg = new Message();

			Check("Unexpected subject", msg.GetSubject() == "");
			Check("Unexpected kind", msg.GetKind() == Message.Kind.PUBLISH);
			Check("Unexpected schema ID", msg.GetSchemaID() == "");
			Check("Unexpected version", msg.GetVersion() == Gmsec.GMSEC_MSG_SPEC_CURRENT);
			Check("Unexpected schema level", msg.GetSchemaLevel() == Specification.SchemaLevel.LEVEL_0);
			Check("Unexpected compliancy", msg.IsCompliant().HasError() == true);
			Check("Unexpected field count", msg.GetFieldCount() == 0);
			Check("Unexpected XML", msg.ToXML() != null);
			Check("Unexpected JSON", msg.ToXML() != null);
		}


		private void Test_CopyConstructor()
		{
			Log.Info("Test copy-constructor");

			MessageFactory msgFactory = new MessageFactory();

			SetStandardFields(msgFactory);

			Message msg1 = msgFactory.CreateMessage("HB");

			// Nominal test
			Message msg2 = new Message(msg1);

			Check("Unexpected subject", msg1.GetSubject() == msg2.GetSubject());
			Check("Unexpected kind", msg1.GetKind() == msg2.GetKind());
			Check("Unexpected schema ID", msg1.GetSchemaID() == msg2.GetSchemaID());
			Check("Unexpected version", msg1.GetVersion() == msg2.GetVersion());
			Check("Unexpected schema level", msg1.GetSchemaLevel() == msg2.GetSchemaLevel());
			Check("Expected compliant message", msg1.IsCompliant().HasError() == msg2.IsCompliant().HasError());
			Check("Unexpected field count", msg1.GetFieldCount() == msg2.GetFieldCount());
			Check("Unexpected XML", msg1.ToXML() == msg2.ToXML());
			Check("Unexpected JSON", msg1.ToJSON() == msg2.ToJSON());

			// Off-Nominal test
			try
			{
				new Message((Message) null);
				Check("Expected exception", false);
			}
			catch (ArgumentNullException e)
			{
				Check(e.ToString(), e.ToString().Contains("System.ArgumentNullException: gmsec::api5::Message const & type is null"));
			}
		}


		private void Test_GetSchemaID()
		{
			Log.Info("Test GetSchemaID()");

			MessageFactory msgFactory = new MessageFactory();

			SetStandardFields(msgFactory);

			Message msg1 = msgFactory.CreateMessage("HB");
			Message msg2 = msgFactory.CreateMessage("MVAL");
			Message msg3 = msgFactory.CreateMessage("TLMFRAME");

			Check("Message 1 has unexpected schema ID", msg1.GetSchemaID() == "MSG.HB");
			Check("Message 2 has unexpected schema ID", msg2.GetSchemaID() == "MSG.MVAL");
			Check("Message 3 has unexpected schema ID", msg3.GetSchemaID() == "MSG.TLMFRAME");
		}


		private void Test_GetVersion()
		{
			Log.Info("Test GetVersion()");

			MessageFactory msgFactory1 = new MessageFactory();
			MessageFactory msgFactory2 = new MessageFactory(new Config("gmsec-specification-version=201900", DataType.KEY_VALUE_DATA));

			Message msg1 = msgFactory1.CreateMessage("HB");
			Message msg2 = msgFactory2.CreateMessage("HB");

			Check("Message 1 has unexpected version", msg1.GetVersion() == Gmsec.GMSEC_MSG_SPEC_CURRENT);
			Check("Message 2 has unexpected version", msg2.GetVersion() == Gmsec.GMSEC_MSG_SPEC_2019_00);
		}


		private void Test_GetSchemaLevel()
		{
			Log.Info("Test GetSchemaLevel()");

			MessageFactory msgFactory1 = new MessageFactory();
			MessageFactory msgFactory2 = new MessageFactory(new Config("gmsec-schema-level=2", DataType.KEY_VALUE_DATA));

			Message msg1 = msgFactory1.CreateMessage("HB");
			Message msg2 = msgFactory2.CreateMessage("HB");

			Check("Message 1 has unexpected version", msg1.GetSchemaLevel() == Specification.SchemaLevel.LEVEL_0);
			Check("Message 2 has unexpected version", msg2.GetSchemaLevel() == Specification.SchemaLevel.LEVEL_2);
		}


		private void Test_IsCompliant()
		{
			Log.Info("Test IsCompliant()");

			Config config = new Config(GetConfig());   // make a copy
			config.AddValue("gmsec-msg-content-validate", "true");

			MessageFactory msgFactory = new MessageFactory(config);

			SetStandardFields(msgFactory);

			Message msg = msgFactory.CreateMessage("HB");

			// Nominal test
			Check("Expected compliant message", msg.IsCompliant().HasError() == false);

			// Off-Nominal test
			msg.ClearField("DOMAIN1");
			Status status = msg.IsCompliant();
			Check("Expected non-compliant message", status.HasError() == true);
		}


		private void Test_RegisterMessageValidator()
		{
			Log.Info("Test RegisterMessageValidator()");

			T005_MessageValidator validator = new T005_MessageValidator();

			Config config = new Config(GetConfig());   // make a copy
			config.AddValue("gmsec-msg-content-validate", "true");

			MessageFactory msgFactory = new MessageFactory(config);

			SetStandardFields(msgFactory);

			Message msg = msgFactory.CreateMessage("HB");

			msg.RegisterMessageValidator(validator);

			// Expecting no error
			Status status = msg.IsCompliant();
			Check("Expected compliant message", status.HasError() == false);

			// Expecting an error
			status = msg.IsCompliant();
			Check("Expected non-compliant message", status.HasError() == true);
			Check("Expected non-compliant message", status.GetReason() == "Field is missing");

			// Expecting an error
			Message msg2 = new Message(msg);
			status = msg2.IsCompliant();
			Check("Expected non-compliant message", status.HasError() == true);
			Check("Expected non-compliant message", status.GetReason() == "Field is missing");
		}


		private void Test_SetFieldValue()
		{
			Log.Info("Test SetFieldValue()");

			MessageFactory msgFactory = new MessageFactory();

			SetStandardFields(msgFactory);

			Message msg = msgFactory.CreateMessage("HB");

			//o Test with string value
			msg.SetFieldValue("PUB-RATE", "5");
			Check("PUB-RATE has unexpected field type", msg.GetField("PUB-RATE").GetFieldType() == Field.Type.U16);
			Check("PUB-RATE has unexpected field value", msg.GetI32Value("PUB-RATE") == 5);

			//o Test with int value
			msg.SetFieldValue("PUB-RATE", 10);
			Check("PUB-RATE has unexpected field type", msg.GetField("PUB-RATE").GetFieldType() == Field.Type.U16);
			Check("PUB-RATE has unexpected field value", msg.GetI32Value("PUB-RATE") == 10);

			//o Test with long value
			msg.SetFieldValue("PUB-RATE", (long) 20);
			Check("PUB-RATE has unexpected field type", msg.GetField("PUB-RATE").GetFieldType() == Field.Type.U16);
			Check("PUB-RATE has unexpected field value", msg.GetI32Value("PUB-RATE") == 20);

			//o Test with double value
			msg.SetFieldValue("PUB-RATE", 40.123);
			Check("PUB-RATE has unexpected field type", msg.GetField("PUB-RATE").GetFieldType() == Field.Type.U16);
			Check("PUB-RATE has unexpected field value", msg.GetI32Value("PUB-RATE") == 40);

			//o Verify result returned is 'true' when replacing a field
			Check("Expected return value of true", msg.SetFieldValue("PUB-RATE", 10) == true);

			//o Verify result returned is 'false' when adding a new field
			Check("Expected return value of false", msg.SetFieldValue("PUBLISH-RATE", 10) == false);
		}


		private void Test_SetConfig()
		{
			Log.Info("Test SetConfig()");

			Config config = new Config("tracking=false", DataType.KEY_VALUE_DATA);

			MessageFactory msgFactory = new MessageFactory();

			SetStandardFields(msgFactory);

			Message msg = msgFactory.CreateMessage("TLMPROC");

			msg.SetConfig(config);

			Check("Message has unexpected Config content", config.ToXML() == msg.GetConfig().ToXML());
		}


		private void Test_SetSubject()
		{
			Log.Info("Test SetSubject()");

			MessageFactory msgFactory = new MessageFactory();

			SetStandardFields(msgFactory);

			Message msg = msgFactory.CreateMessage("TLMPROC");

			msg.SetSubject("GMSEC.MY-MISSION.MY-SAT-ID.MSG.TLMPROC.FOOEY");

			Check("Message has unexpected subject", msg.GetSubject() == "GMSEC.MY-MISSION.MY-SAT-ID.MSG.TLMPROC.FOOEY");

			//o Off-nominal tests

			if (UsingLinux())
			{
				// Note: This test does not work on Windoze
				try {
					//o null subject
					Message tmp = new Message();
					tmp.SetSubject(null);
					Check("An exception was expected", false);
				}
				catch (GmsecException e) {
					Check(e.ToString(), e.ToString().Contains("Subject cannot be NULL, nor an empty string"));
				}
			}

			try {
				//o zero-length subject
				Message tmp = new Message();
				tmp.SetSubject("");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Subject cannot be NULL, nor an empty string"));
			}

			try {
				//o invalid subject content
				Message tmp = new Message();
				tmp.SetSubject("Subject.Cannot.Contain.Lowercase.Letters");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Invalid message subject"));
			}

			try {
				//o invalid subject content
				Message tmp = new Message();
				tmp.SetSubject("HELLO%WORLD");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Invalid message subject"));
			}
		}


		private void Test_GetSubject()
		{
			Log.Info("Test GetSubject()");

			MessageFactory msgFactory = new MessageFactory();

			SetStandardFields(msgFactory);

			Message msg = msgFactory.CreateMessage("TLMPROC");
			msg.AddField("STREAM-MODE", "RT");  // add ME2 (stream mode)

			string subject = msg.GetSubject();
			subject += ".2";   // add ME2 (virtual channel ID)
			subject += ".1";   // add ME3 (AP ID)

			msg.SetSubject(subject);

			Check("Message has unexpected subject", msg.GetSubject() == subject);
		}


		public void Test_SetKind()
		{
			Log.Info("Test SetKind()");

			MessageFactory msgFactory = new MessageFactory();

			SetStandardFields(msgFactory);

			Message msg = msgFactory.CreateMessage("HB");
			Check("Expected PUBLISH-kind", msg.GetKind() == Message.Kind.PUBLISH);

			// Set to REQUEST-kind, and verify
			msg.SetKind(Message.Kind.REQUEST);
			Check("Expected REQUEST-kind", msg.GetKind() == Message.Kind.REQUEST);

			// Set to REPLY-kind, and verify
			msg.SetKind(Message.Kind.REPLY);
			Check("Expected REPLY-kind", msg.GetKind() == Message.Kind.REPLY);

			// Set to PUBLISH-kind, and verify
			msg.SetKind(Message.Kind.PUBLISH);
			Check("Expected PUBLISH-kind", msg.GetKind() == Message.Kind.PUBLISH);
		}


		public void Test_GetKind()
		{
			Log.Info("Test GetKind()");

			MessageFactory msgFactory = new MessageFactory();

			SetStandardFields(msgFactory);

			Message msg1 = msgFactory.CreateMessage("HB");
			Message msg2 = msgFactory.CreateMessage("REQ.DIR");
			Message msg3 = msgFactory.CreateMessage("RESP.DIR");
			Message msg4 = msgFactory.CreateMessage();
			Message msg5 = new Message();

			Check("Message 1 has unexpected kind", msg1.GetKind() == Message.Kind.PUBLISH);
			Check("Message 2 has unexpected kind", msg2.GetKind() == Message.Kind.REQUEST);
			Check("Message 3 has unexpected kind", msg3.GetKind() == Message.Kind.REPLY);
			Check("Message 4 has unexpected kind", msg4.GetKind() == Message.Kind.PUBLISH);
			Check("Message 5 has unexpected kind", msg5.GetKind() == Message.Kind.PUBLISH);
		}


		private void Test_AddField()
		{
			Log.Info("Test AddField()");

			StringField field = new StringField("SPECIFICATION", "C2MS");

			MessageFactory msgFactory = new MessageFactory();

			Message msg = msgFactory.CreateMessage("LOG");

			//o replace existing field
			Check("Expected field to be replaced", msg.AddField(field) == true);

			//o test other overloaded methods
			Check("Expected new field to be added", msg.AddField("BINARY", new byte[6]) == false);
			Check("Expected new field to be added", msg.AddField("BOOLEAN", true) == false);
			Check("Expected new field to be added", msg.AddField("CHAR", 'c') == false);
			Check("Expected new field to be added", msg.AddField("FLOAT", 1.2f) == false);
			Check("Expected new field to be added", msg.AddField("DOUBLE", 2.4) == false);
			Check("Expected new field to be added", msg.AddField("BYTE", (sbyte) 5) == false);
			Check("Expected new field to be added", msg.AddField("SHORT", (short) 55) == false);
			Check("Expected new field to be added", msg.AddField("INT", 555) == false);
			Check("Expected new field to be added", msg.AddField("LONG", (long) 5555) == false);
			Check("Expected new field to be added", msg.AddField("STRING", "The old fox") == false);
			Check("Expected new field to be added", msg.AddField("UBYTE", (byte) 5) == false);
			Check("Expected new field to be added", msg.AddField("USHORT", (ushort) 55) == false);
			Check("Expected new field to be added", msg.AddField("UINT", (uint) 555) == false);
			Check("Expected new field to be added", msg.AddField("ULONG", (ulong) 5555) == false);

			//o validate field settings
			Check("SPECIFICATION not found", msg.HasField("SPECIFICATION"));
			Check("SPECIFICATION has wrong type", msg.GetField("SPECIFICATION").GetFieldType() == Field.Type.STRING);
			Check("SPECIFICATION has wrong value", msg.GetStringValue("SPECIFICATION") == "C2MS");

			Check("BINARY not found", msg.HasField("BINARY"));
			Check("BINARY has wrong type", msg.GetField("BINARY").GetFieldType() == Field.Type.BINARY);
			Check("BINARY has wrong length", msg.GetBinaryField("BINARY").GetValue().Length == 6);

			Check("BOOLEAN not found", msg.HasField("BOOLEAN"));
			Check("BOOLEAN has wrong type", msg.GetField("BOOLEAN").GetFieldType() == Field.Type.BOOL);
			Check("BOOLEAN has wrong value", msg.GetBooleanField("BOOLEAN").GetValue() == true);

			Check("CHAR not found", msg.HasField("CHAR"));
			Check("CHAR has wrong type", msg.GetField("CHAR").GetFieldType() == Field.Type.CHAR);
			Check("CHAR has wrong value", msg.GetCharField("CHAR").GetValue() == 'c');

			Check("FLOAT not found", msg.HasField("FLOAT"));
			Check("FLOAT has wrong type", msg.GetField("FLOAT").GetFieldType() == Field.Type.F32);
			Check("FLOAT has wrong value", msg.GetF32Field("FLOAT").GetValue() == 1.2f);

			Check("DOUBLE not found", msg.HasField("DOUBLE"));
			Check("DOUBLE has wrong type", msg.GetField("DOUBLE").GetFieldType() == Field.Type.F64);
			Check("DOUBLE has wrong value", msg.GetF64Field("DOUBLE").GetValue() == 2.4);

			Check("BYTE not found", msg.HasField("BYTE"));
			Check("BYTE has wrong type", msg.GetField("BYTE").GetFieldType() == Field.Type.I8);
			Check("BYTE has wrong value", msg.GetI8Field("BYTE").GetValue() == (sbyte) 5);

			Check("SHORT not found", msg.HasField("SHORT"));
			Check("SHORT has wrong type", msg.GetField("SHORT").GetFieldType() == Field.Type.I16);
			Check("SHORT has wrong value", msg.GetI16Field("SHORT").GetValue() == (short) 55);

			Check("INT not found", msg.HasField("INT"));
			Check("INT has wrong type", msg.GetField("INT").GetFieldType() == Field.Type.I32);
			Check("INT has wrong value", msg.GetI32Field("INT").GetValue() == 555);

			Check("LONG not found", msg.HasField("LONG"));
			Check("LONG has wrong type", msg.GetField("LONG").GetFieldType() == Field.Type.I64);
			Check("LONG has wrong value", msg.GetI64Field("LONG").GetValue() == (long) 5555);

			Check("STRING not found", msg.HasField("STRING"));
			Check("STRING has wrong type", msg.GetField("STRING").GetFieldType() == Field.Type.STRING);
			Check("STRING has wrong value", msg.GetStringField("STRING").GetValue() == "The old fox");

			Check("UBYTE not found", msg.HasField("UBYTE"));
			Check("UBYTE has wrong type", msg.GetField("UBYTE").GetFieldType() == Field.Type.U8);
			Check("UBYTE has wrong value", msg.GetU8Field("UBYTE").GetValue() == (byte) 5);

			Check("USHORT not found", msg.HasField("USHORT"));
			Check("USHORT has wrong type", msg.GetField("USHORT").GetFieldType() == Field.Type.U16);
			Check("USHORT has wrong value", msg.GetU16Field("USHORT").GetValue() == (ushort) 55);

			Check("UINT not found", msg.HasField("UINT"));
			Check("UINT has wrong type", msg.GetField("UINT").GetFieldType() == Field.Type.U32);
			Check("UINT has wrong value", msg.GetU32Field("UINT").GetValue() == (uint) 555);

			Check("ULONG not found", msg.HasField("ULONG"));
			Check("ULONG has wrong type", msg.GetField("ULONG").GetFieldType() == Field.Type.U64);
			Check("ULONG has wrong value", msg.GetU64Field("ULONG").GetValue() == (ulong) 5555);
		}


		private void Test_AddFields()
		{
			Log.Info("Test AddFields()");

			FieldList fields = new FieldList()
			{
				new StringField("ONE", "uno"),
				new StringField("TWO", "dos"),
				new StringField("THREE", "tres")
			};

			MessageFactory msgFactory = new MessageFactory();

			Message msg = msgFactory.CreateMessage("LOG");

			Check("Expected only new fields to be added", false == msg.AddFields(fields));

			Check("Expected to have field ONE", true == msg.HasField("ONE"));
			Check("Unexpected field type for ONE", Field.Type.STRING == msg.GetField("ONE").GetFieldType());
			Check("Unexpected field value for ONE", msg.GetStringValue("ONE") == "uno");

			Check("Expected to have field TWO", true == msg.HasField("TWO"));
			Check("Unexpected field type for TWO", Field.Type.STRING == msg.GetField("TWO").GetFieldType());
			Check("Unexpected field value for TWO", msg.GetStringValue("TWO") == "dos");

			Check("Expected to have field THREE", true == msg.HasField("THREE"));
			Check("Unexpected field type for THREE", Field.Type.STRING == msg.GetField("THREE").GetFieldType());
			Check("Unexpected field value for THREE", msg.GetStringValue("THREE") == "tres");
		}


		private void Test_ClearFields()
		{
			Log.Info("Test ClearFields()");

			MessageFactory msgFactory = new MessageFactory();

			Message msg = msgFactory.CreateMessage("HB");

			Check("Message missing CONTENT-VERSION", true == msg.HasField("CONTENT-VERSION"));
			Check("Message missing HEADER-VERSION", true == msg.HasField("HEADER-VERSION"));
			Check("Message missing MESSAGE-SUBTYPE", true == msg.HasField("MESSAGE-SUBTYPE"));
			Check("Message missing MESSAGE-TYPE", true == msg.HasField("MESSAGE-TYPE"));
			Check("Message missing SPECIFICATION", true == msg.HasField("SPECIFICATION"));

			msg.ClearFields();

			Check("Message has CONTENT-VERSION", false == msg.HasField("CONTENT-VERSION"));
			Check("Message has HEADER-VERSION", false == msg.HasField("HEADER-VERSION"));
			Check("Message has MESSAGE-SUBTYPE", false == msg.HasField("MESSAGE-SUBTYPE"));
			Check("Message has MESSAGE-TYPE", false == msg.HasField("MESSAGE-TYPE"));
			Check("Message has SPECIFICATION", false == msg.HasField("SPECIFICATION"));
		}


		private void Test_ClearField()
		{
			Log.Info("Test ClearField()");

			MessageFactory msgFactory = new MessageFactory();

			Message msg = msgFactory.CreateMessage("HB");

			Check("CONTENT-VERSION not found in message", msg.HasField("CONTENT-VERSION"));

			Check("CONTENT-VERSION not cleared from message", msg.ClearField("CONTENT-VERSION") == true);
			Check("FOO-BAR does not exist and cannot be cleared", msg.ClearField("FOO-BAR") == false);
			Check("foo-bar does not exist and cannot be cleared", msg.ClearField("foo-bar") == false);

			Check("CONTENT-VERSION should not exist in message", msg.HasField("CONTENT-VERSION") == false);
		}


		private void Test_HasField()
		{
			Log.Info("Test HasField()");

			MessageFactory msgFactory = new MessageFactory();

			Message msg = msgFactory.CreateMessage("HB");

			msg.SetFieldValue("PUB-RATE", "30");

			Check("Message should have PUB-RATE", msg.HasField("PUB-RATE"));
			Check("Message should not have PUBLISH-RATE", msg.HasField("PUBLISH-RATE") == false);
			Check("Message should not have null field", msg.HasField(null) == false);
			Check("Message should not have empty string field", msg.HasField("") == false);
		}


		private void Test_GetStringValue()
		{
			Log.Info("Test GetStringValue()");

			MessageFactory msgFactory = new MessageFactory();

			Message msg = msgFactory.CreateMessage("HB");

			msg.SetFieldValue("PUB-RATE", "30");

			Check("MESSAGE-TYPE has unexpected value", msg.GetStringValue("MESSAGE-TYPE") == "MSG");
			Check("PUB-RATE has unexpected value", msg.GetStringValue("PUB-RATE") == "30");
		}


		private void Test_GetBooleanValue()
		{
			Log.Info("Test GetBooleanValue()");

			Message msg = new Message();

			msg.AddField("FIELD-1", 0);
			msg.AddField("FIELD-2", 1);
			msg.AddField("FIELD-3", "10000");
			msg.AddField("FIELD-4", 10000);
			msg.AddField("FIELD-5", true);
			msg.AddField("FIELD-6", false);
			msg.AddField("FIELD-7", 2019.0);
			msg.AddField("FIELD-8", "true");
			msg.AddField("FIELD-9", "FALSE");
			msg.AddField("FIELD-0", "foo bar");

			// Nominal tests
			Check("1 Expected boolean value of false", msg.GetBooleanValue("FIELD-1") == false);
			Check("2 Expected boolean value of true",  msg.GetBooleanValue("FIELD-2"));
			Check("3 Expected boolean value of true",  msg.GetBooleanValue("FIELD-3"));
			Check("4 Expected boolean value of true",  msg.GetBooleanValue("FIELD-4"));
			Check("5 Expected boolean value of true",  msg.GetBooleanValue("FIELD-5"));
			Check("6 Expected boolean value of false", msg.GetBooleanValue("FIELD-6") == false);
			Check("7 Expected boolean value of true",  msg.GetBooleanValue("FIELD-7"));
			Check("8 Expected boolean value of true",  msg.GetBooleanValue("FIELD-8"));
			Check("9 Expected boolean value of false", msg.GetBooleanValue("FIELD-9") == false);

			// Off-nominal tests
			// Field cannot be converted
			try {
				msg.GetBooleanValue("FIELD-0");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Field cannot be represented as a boolean"));
			}

			// Unknown field
			try {
				msg.GetBooleanValue("BOGUS");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Message does not contain field with name: BOGUS"));
			}

			// NULL field name
			try {
				msg.GetBooleanValue(null);
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Field name cannot be NULL, nor an empty string"));
			}

			// empty-string field name
			try {
				msg.GetBooleanValue("");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Field name cannot be NULL, nor an empty string"));
			}
		}


		private void Test_GetI16Value()
		{
			Log.Info("Test GetI16Value()");

			MessageFactory msgFactory = new MessageFactory();

			Message msg = msgFactory.CreateMessage("HB");

			msg.SetFieldValue("PUB-RATE", "30");

			Check("PUB-RATE has unexpected value", msg.GetI16Value("PUB-RATE") == (short) 30);

			try {
				msg.GetI16Value("MESSAGE-TYPE");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Field cannot be converted to a GMSEC_I16"));
			}
		}


		private void Test_GetI32Value()
		{
			Log.Info("Test GetI32Value()");

			MessageFactory msgFactory = new MessageFactory();

			Message msg = msgFactory.CreateMessage("HB");

			msg.SetFieldValue("PUB-RATE", "30");

			Check("PUB-RATE has unexpected value", msg.GetI32Value("PUB-RATE") == 30);

			try {
				msg.GetI32Value("MESSAGE-TYPE");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Field cannot be converted to a GMSEC_I32"));
			}
		}


		private void Test_GetI64Value()
		{
			Log.Info("Test GetI64Value()");

			MessageFactory msgFactory = new MessageFactory();

			Message msg = msgFactory.CreateMessage("HB");

			msg.SetFieldValue("PUB-RATE", "30");

			Check("PUB-RATE has unexpected value", msg.GetI64Value("PUB-RATE") == (long) 30);

			try {
				msg.GetI64Value("MESSAGE-TYPE");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Field cannot be converted to a GMSEC_I64"));
			}
		}


		private void Test_GetU16Value()
		{
			Log.Info("Test GetU16Value()");

			MessageFactory msgFactory = new MessageFactory();

			Message msg = msgFactory.CreateMessage("HB");

			msg.SetFieldValue("PUB-RATE", "30");

			Check("PUB-RATE has unexpected value", msg.GetU16Value("PUB-RATE") == (ushort) 30);

			try {
				msg.GetU16Value("MESSAGE-TYPE");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Field cannot be converted to a GMSEC_U16"));
			}
		}


		private void Test_GetU32Value()
		{
			Log.Info("Test GetU32Value()");

			MessageFactory msgFactory = new MessageFactory();

			Message msg = msgFactory.CreateMessage("HB");

			msg.SetFieldValue("PUB-RATE", "30");

			Check("PUB-RATE has unexpected value", msg.GetU32Value("PUB-RATE") == (ulong) 30);

			try {
				msg.GetU32Value("MESSAGE-TYPE");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Field cannot be converted to a GMSEC_U32"));
			}
		}


		private void Test_GetU64Value()
		{
			Log.Info("Test GetU64Value()");

			MessageFactory msgFactory = new MessageFactory();

			Message msg = msgFactory.CreateMessage("HB");

			msg.SetFieldValue("PUB-RATE", "30");

			Check("PUB-RATE has unexpected value", msg.GetU64Value("PUB-RATE") == (ulong) 30);

			try {
				msg.GetU64Value("MESSAGE-TYPE");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Field cannot be converted to a GMSEC_U64"));
			}
		}


		private void Test_GetF64Value()
		{
			Log.Info("Test GetF64Value()");

			MessageFactory msgFactory = new MessageFactory();

			Message msg = msgFactory.CreateMessage("HB");

			msg.SetFieldValue("PUB-RATE", "30");

			Check("PUB-RATE has unexpected value", msg.GetF64Value("PUB-RATE") == (double) 30);

			try {
				msg.GetF64Value("MESSAGE-TYPE");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Field cannot be converted to a GMSEC_F64"));
			}
		}


		private void Test_GetField()
		{
			Log.Info("Test GetField()");

			MessageFactory msgFactory = new MessageFactory();

			Message msg = msgFactory.CreateMessage("HB");

			//o Use valid field name
			Check("Message should have MESSAGE-TYPE", msg.GetField("MESSAGE-TYPE") != null);

			//o Use bogus field name
			Check("Message should have MSG-TYPE", msg.GetField("MSG-TYPE") == null);

			//o Use null field name
			try {
				msg.GetField(null);
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Field name cannot be NULL, nor an empty string"));
			}

			//o Use empty-string field name
			try {
				msg.GetField("");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Field name cannot be NULL, nor an empty string"));
			}
		}


		private void Test_GetFieldType()
		{
			Log.Info("Test GetFieldType()");

			MessageFactory msgFactory = new MessageFactory();

			Message msg = msgFactory.CreateMessage("HB");

			Check("MESSAGE-TYPE has unexpected field type", msg.GetFieldType("MESSAGE-TYPE") == Field.Type.STRING);
			Check("CONTENT-VERSION has unexpected field type", msg.GetFieldType("CONTENT-VERSION") == Field.Type.F32);

			//o Use bogus field name
			try {
				msg.GetFieldType("MSG-TYPE");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Message does not contain field with name: MSG-TYPE"));
			}

			//o Use null field name
			try {
				msg.GetFieldType(null);
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Field name cannot be NULL, nor an empty string"));
			}

			//o Use empty-string field name
			try {
				msg.GetFieldType("");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Field name cannot be NULL, nor an empty string"));
			}
		}


		private void Test_GetBinaryField()
		{
			Log.Info("Test GetBinaryField()");

			byte[] data = new byte[256];
			for (int i = 0; i < data.Length; ++i) {
				data[i] = (byte)(i & 0xFF);
			}

			MessageFactory msgFactory = new MessageFactory();

			Message msg = msgFactory.CreateMessage("MSG.TLMPKT");

			msg.AddField("DATA", data);

			//o Use valid field name
			BinaryField field = msg.GetBinaryField("DATA");
			Check("Unexpected field type", field.GetFieldType() == Field.Type.BINARY);
			Check("Unexpected field name", field.GetName() == "DATA");
			Check("Unexpected data length", field.GetValue().Length == data.Length);

			//o Use non-binary field name
			try {
				msg.GetBinaryField("CONTENT-VERSION");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Message does not contain BinaryField with name: CONTENT-VERSION"));
			}

			//o Use null field name
			try {
				msg.GetBinaryField(null);
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Field name cannot be NULL, nor an empty string"));
			}

			//o Use empty-string field name
			try {
				msg.GetBinaryField("");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Field name cannot be NULL, nor an empty string"));
			}
		}


		private void Test_GetBooleanField()
		{
			Log.Info("Test GetBooleanField()");

			MessageFactory msgFactory = new MessageFactory();

			Message msg = msgFactory.CreateMessage("REQ.DIR");

			msg.AddField("RESPONSE", true);

			//o Use valid field name
			BooleanField field = msg.GetBooleanField("RESPONSE");
			Check("Unexpected field type", field.GetFieldType() == Field.Type.BOOL);
			Check("Unexpected field name", field.GetName() == "RESPONSE");
			Check("Unexpected data value", field.GetValue() == true);

			//o Use non-boolean field name
			try {
				msg.GetBooleanField("CONTENT-VERSION");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Message does not contain BooleanField with name: CONTENT-VERSION"));
			}

			//o Use null field name
			try {
				msg.GetBooleanField(null);
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Field name cannot be NULL, nor an empty string"));
			}

			//o Use empty-string field name
			try {
				msg.GetBooleanField("");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Field name cannot be NULL, nor an empty string"));
			}
		}


		private void Test_GetCharField()
		{
			Log.Info("Test GetCharField()");

			MessageFactory msgFactory = new MessageFactory();

			Message msg = msgFactory.CreateMessage("REQ.DIR");

			msg.AddField("CHAR-FIELD", 'c');

			//o Use valid field name
			CharField field = msg.GetCharField("CHAR-FIELD");
			Check("Unexpected field type", field.GetFieldType() == Field.Type.CHAR);
			Check("Unexpected field name", field.GetName() == "CHAR-FIELD");
			Check("Unexpected data value", field.GetValue() == 'c');

			//o Use non-char field name
			try {
				msg.GetCharField("CONTENT-VERSION");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Message does not contain CharField with name: CONTENT-VERSION"));
			}

			//o Use null field name
			try {
				msg.GetCharField(null);
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Field name cannot be NULL, nor an empty string"));
			}

			//o Use empty-string field name
			try {
				msg.GetCharField("");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Field name cannot be NULL, nor an empty string"));
			}
		}


		private void Test_GetF32Field()
		{
			Log.Info("Test GetF32Field()");

			MessageFactory msgFactory = new MessageFactory();

			Message msg = msgFactory.CreateMessage("REQ.DIR");

			//o Use valid field name
			F32Field field = msg.GetF32Field("CONTENT-VERSION");
			Check("Unexpected field type", field.GetFieldType() == Field.Type.F32);
			Check("Unexpected field name", field.GetName() == "CONTENT-VERSION");

			//o Use non-F32 field name
			try {
				msg.GetF32Field("MESSAGE-TYPE");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Message does not contain F32Field with name: MESSAGE-TYPE"));
			}

			//o Use null field name
			try {
				msg.GetF32Field(null);
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Field name cannot be NULL, nor an empty string"));
			}

			//o Use empty-string field name
			try {
				msg.GetF32Field("");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Field name cannot be NULL, nor an empty string"));
			}
		}


		private void Test_GetF64Field()
		{
			Log.Info("Test GetF64Field()");

			MessageFactory msgFactory = new MessageFactory();

			Message msg = msgFactory.CreateMessage("REQ.DIR");

			msg.AddField("F64-FIELD", 3.14);

			//o Use valid field name
			F64Field field = msg.GetF64Field("F64-FIELD");
			Check("Unexpected field type", field.GetFieldType() == Field.Type.F64);
			Check("Unexpected field name", field.GetName() == "F64-FIELD");
			Check("Unexpected field value", field.GetValue() == 3.14);

			//o Use non-F64 field name
			try {
				msg.GetF64Field("MESSAGE-TYPE");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Message does not contain F64Field with name: MESSAGE-TYPE"));
			}

			//o Use null field name
			try {
				msg.GetF64Field(null);
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Field name cannot be NULL, nor an empty string"));
			}

			//o Use empty-string field name
			try {
				msg.GetF64Field("");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Field name cannot be NULL, nor an empty string"));
			}
		}


		private void Test_GetI8Field()
		{
			Log.Info("Test GetI8Field()");

			MessageFactory msgFactory = new MessageFactory();

			Message msg = msgFactory.CreateMessage("REQ.DIR");

			msg.AddField("I8-FIELD", (sbyte) 8);

			//o Use valid field name
			I8Field field = msg.GetI8Field("I8-FIELD");
			Check("Unexpected field type", field.GetFieldType() == Field.Type.I8);
			Check("Unexpected field name", field.GetName() == "I8-FIELD");
			Check("Unexpected field value", field.GetValue() == (sbyte) 8);

			//o Use non-I8 field name
			try {
				msg.GetI8Field("MESSAGE-TYPE");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Message does not contain I8Field with name: MESSAGE-TYPE"));
			}

			//o Use null field name
			try {
				msg.GetI8Field(null);
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Field name cannot be NULL, nor an empty string"));
			}

			//o Use empty-string field name
			try {
				msg.GetI8Field("");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Field name cannot be NULL, nor an empty string"));
			}
		}


		private void Test_GetI16Field()
		{
			Log.Info("Test GetI16Field()");

			MessageFactory msgFactory = new MessageFactory();

			Message msg = msgFactory.CreateMessage("REQ.DIR");

			msg.AddField("I16-FIELD", (short) 16);

			//o Use valid field name
			I16Field field = msg.GetI16Field("I16-FIELD");
			Check("Unexpected field type", field.GetFieldType() == Field.Type.I16);
			Check("Unexpected field name", field.GetName() == "I16-FIELD");
			Check("Unexpected field value", field.GetValue() == (short) 16);

			//o Use non-I16 field name
			try {
				msg.GetI16Field("MESSAGE-TYPE");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Message does not contain I16Field with name: MESSAGE-TYPE"));
			}

			//o Use null field name
			try {
				msg.GetI16Field(null);
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Field name cannot be NULL, nor an empty string"));
			}

			//o Use empty-string field name
			try {
				msg.GetI16Field("");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Field name cannot be NULL, nor an empty string"));
			}
		}


		private void Test_GetI32Field()
		{
			Log.Info("Test GetI32Field()");

			MessageFactory msgFactory = new MessageFactory();

			Message msg = msgFactory.CreateMessage("REQ.DIR");

			msg.AddField("I32-FIELD", 32);

			//o Use valid field name
			I32Field field = msg.GetI32Field("I32-FIELD");
			Check("Unexpected field type", field.GetFieldType() == Field.Type.I32);
			Check("Unexpected field name", field.GetName() == "I32-FIELD");
			Check("Unexpected field value", field.GetValue() == 32);

			//o Use non-I32 field name
			try {
				msg.GetI32Field("MESSAGE-TYPE");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Message does not contain I32Field with name: MESSAGE-TYPE"));
			}

			//o Use null field name
			try {
				msg.GetI32Field(null);
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Field name cannot be NULL, nor an empty string"));
			}

			//o Use empty-string field name
			try {
				msg.GetI32Field("");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Field name cannot be NULL, nor an empty string"));
			}
		}


		private void Test_GetI64Field()
		{
			Log.Info("Test GetI64Field()");

			MessageFactory msgFactory = new MessageFactory();

			Message msg = msgFactory.CreateMessage("REQ.DIR");

			msg.AddField("I64-FIELD", (long) 64);

			//o Use valid field name
			I64Field field = msg.GetI64Field("I64-FIELD");
			Check("Unexpected field type", field.GetFieldType() == Field.Type.I64);
			Check("Unexpected field name", field.GetName() == "I64-FIELD");
			Check("Unexpected field value", field.GetValue() == (long) 64);

			//o Use non-I64 field name
			try {
				msg.GetI64Field("MESSAGE-TYPE");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Message does not contain I64Field with name: MESSAGE-TYPE"));
			}

			//o Use null field name
			try {
				msg.GetI64Field(null);
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Field name cannot be NULL, nor an empty string"));
			}

			//o Use empty-string field name
			try {
				msg.GetI64Field("");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Field name cannot be NULL, nor an empty string"));
			}
		}


		private void Test_GetU8Field()
		{
			Log.Info("Test GetU8Field()");

			MessageFactory msgFactory = new MessageFactory();

			Message msg = msgFactory.CreateMessage("REQ.DIR");

			msg.AddField("U8-FIELD", (byte) 8);

			//o Use valid field name
			U8Field field = msg.GetU8Field("U8-FIELD");
			Check("Unexpected field type", field.GetFieldType() == Field.Type.U8);
			Check("Unexpected field name", field.GetName() == "U8-FIELD");
			Check("Unexpected field value", field.GetValue() == (byte) 8);

			//o Use non-U8 field name
			try {
				msg.GetU8Field("MESSAGE-TYPE");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Message does not contain U8Field with name: MESSAGE-TYPE"));
			}

			//o Use null field name
			try {
				msg.GetU8Field(null);
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Field name cannot be NULL, nor an empty string"));
			}

			//o Use empty-string field name
			try {
				msg.GetU8Field("");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Field name cannot be NULL, nor an empty string"));
			}
		}


		private void Test_GetU16Field()
		{
			Log.Info("Test GetU16Field()");

			MessageFactory msgFactory = new MessageFactory();

			Message msg = msgFactory.CreateMessage("REQ.DIR");

			msg.AddField("U16-FIELD", (ushort) 16);

			//o Use valid field name
			U16Field field = msg.GetU16Field("U16-FIELD");
			Check("Unexpected field type", field.GetFieldType() == Field.Type.U16);
			Check("Unexpected field name", field.GetName() == "U16-FIELD");
			Check("Unexpected field value", field.GetValue() == (ushort) 16);

			//o Use non-U16 field name
			try {
				msg.GetU16Field("MESSAGE-TYPE");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Message does not contain U16Field with name: MESSAGE-TYPE"));
			}

			//o Use null field name
			try {
				msg.GetU16Field(null);
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Field name cannot be NULL, nor an empty string"));
			}

			//o Use empty-string field name
			try {
				msg.GetU16Field("");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Field name cannot be NULL, nor an empty string"));
			}
		}


		private void Test_GetU32Field()
		{
			Log.Info("Test GetU32Field()");

			MessageFactory msgFactory = new MessageFactory();

			Message msg = msgFactory.CreateMessage("REQ.DIR");

			msg.AddField("U32-FIELD", (uint) 32);

			//o Use valid field name
			U32Field field = msg.GetU32Field("U32-FIELD");
			Check("Unexpected field type", field.GetFieldType() == Field.Type.U32);
			Check("Unexpected field name", field.GetName() == "U32-FIELD");
			Check("Unexpected field value", field.GetValue() == (uint) 32);

			//o Use non-U32 field name
			try {
				msg.GetU32Field("MESSAGE-TYPE");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Message does not contain U32Field with name: MESSAGE-TYPE"));
			}

			//o Use null field name
			try {
				msg.GetU32Field(null);
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Field name cannot be NULL, nor an empty string"));
			}

			//o Use empty-string field name
			try {
				msg.GetU32Field("");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Field name cannot be NULL, nor an empty string"));
			}
		}


		private void Test_GetU64Field()
		{
			Log.Info("Test GetU64Field()");

			MessageFactory msgFactory = new MessageFactory();

			Message msg = msgFactory.CreateMessage("REQ.DIR");

			msg.AddField("U64-FIELD", (ulong) 64);

			//o Use valid field name
			U64Field field = msg.GetU64Field("U64-FIELD");
			Check("Unexpected field type", field.GetFieldType() == Field.Type.U64);
			Check("Unexpected field name", field.GetName() == "U64-FIELD");
			Check("Unexpected field value", field.GetValue() == (ulong) 64);

			//o Use non-U64 field name
			try {
				msg.GetU64Field("MESSAGE-TYPE");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Message does not contain U64Field with name: MESSAGE-TYPE"));
			}

			//o Use null field name
			try {
				msg.GetU64Field(null);
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Field name cannot be NULL, nor an empty string"));
			}

			//o Use empty-string field name
			try {
				msg.GetU64Field("");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Field name cannot be NULL, nor an empty string"));
			}
		}


		private void Test_GetStringField()
		{
			Log.Info("Test GetStringField()");

			MessageFactory msgFactory = new MessageFactory();

			Message msg = msgFactory.CreateMessage("REQ.DIR");

			//o Use valid field name
			StringField field = msg.GetStringField("MESSAGE-TYPE");
			Check("Unexpected field type", field.GetFieldType() == Field.Type.STRING);
			Check("Unexpected field name", field.GetName() == "MESSAGE-TYPE");
			Check("Unexpected field value", field.GetValue() == "REQ");

			//o Use non-string field name
			try {
				msg.GetStringField("CONTENT-VERSION");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Message does not contain StringField with name: CONTENT-VERSION"));
			}

			//o Use null field name
			try {
				msg.GetStringField(null);
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Field name cannot be NULL, nor an empty string"));
			}

			//o Use empty-string field name
			try {
				msg.GetStringField("");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Field name cannot be NULL, nor an empty string"));
			}
		}


		private void Test_GetFieldCount()
		{
			Log.Info("Test GetFieldCount()");

			MessageFactory msgFactory = new MessageFactory();

			SetStandardFields(msgFactory);

			Message msg = msgFactory.CreateMessage("HB");

			Check("Message has unexpected field count", msg.GetFieldCount() == 13);

			msg.SetFieldValue("PUB-RATE", "30");

			Check("Message has unexpected field count", msg.GetFieldCount() == 14);

			msg.ClearField("PUB-RATE");

			Check("Message has unexpected field count", msg.GetFieldCount() == 13);

			msg.ClearFields();

			Check("Message has unexpected field count", msg.GetFieldCount() == 0);
		}


		private void Test_CopyFields()
		{
			Log.Info("Test CopyFields()");

			MessageFactory msgFactory = new MessageFactory();

			Message msg1 = msgFactory.CreateMessage("HB");
			Message msg2 = new Message(msg1);

			msg1.SetFieldValue("PUB-RATE", "30");

			Check("Message 2 should not have PUB-RATE", msg2.HasField("PUB-RATE") == false);

			msg1.CopyFields(msg2);

			Check("Message 2 should have PUB-RATE", msg2.HasField("PUB-RATE"));
		}


		private void Test_ToXML()
		{
			Log.Info("Test ToXML()");

			MessageFactory msgFactory = new MessageFactory();

			SetStandardFields(msgFactory);

			Message msg1 = msgFactory.CreateMessage("HB");
			Message msg2 = new Message(msg1);

			Check("Message 1 has NULL XML content", msg1.ToXML() != null);
			Check("Message 2 has NULL XML content", msg2.ToXML() != null);
			Check("Messages 1 and 2 do not have same XML content", msg1.ToXML() == msg2.ToXML());

			Message msg3 = msgFactory.FromData(msg1.ToXML(), DataType.XML_DATA);
			Check("Messages 1 and 3 do not have same XML content", msg1.ToXML() == msg3.ToXML());
		}


		private void Test_ToJSON()
		{
			Log.Info("Test ToJSON()");

			MessageFactory msgFactory = new MessageFactory();

			SetStandardFields(msgFactory);

			Message msg1 = msgFactory.CreateMessage("HB");
			Message msg2 = new Message(msg1);

			Check("Message 1 has NULL JSON content", msg1.ToJSON() != null);
			Check("Message 2 has NULL JSON content", msg2.ToJSON() != null);
			Check("Messages 1 and 2 do not have same JSON content", msg1.ToJSON() == msg2.ToJSON());

			Message msg3 = msgFactory.FromData(msg1.ToJSON(), DataType.JSON_DATA);
			Check("Messages 1 and 3 do not have same JSON content", msg1.ToJSON() == msg3.ToJSON());
		}


		private void Test_GetSize()
		{
			Log.Info("Test GetSize()");

			MessageFactory msgFactory = new MessageFactory();

			Message msg1 = msgFactory.CreateMessage("HB");
			Message msg2 = new Message(msg1);

			Check("Message 1 has unexpected size", msg1.GetSize() > 0);
			Check("Message 2 has unexpected size", msg2.GetSize() > 0);
			Check("Messages should have the same size", msg1.GetSize() == msg2.GetSize());
		}


		private void Test_GetFieldIterator()
		{
			Log.Info("Test GetFieldIterator()");

			MessageFactory msgFactory = new MessageFactory();

			Message msg1 = msgFactory.CreateMessage("HB");
			Message msg2 = new Message();

			MessageFieldIterator iter1 = msg1.GetFieldIterator();
			MessageFieldIterator iter2 = msg2.GetFieldIterator();

			Check("Have no fields to iterate", iter1.HasNext() == true);
			Check("Have fields to iterate", iter2.HasNext() == false);
		}
	}


	class T005_MessageValidator : MessageValidator
	{
		private int iter = 0;

		public T005_MessageValidator() {}

		public override Status ValidateMessage(Message msg)
		{
			Status status = new Status();

			if (iter > 0)
			{
				status = new Status((int) StatusClass.MSG_ERROR, (int) StatusCode.MESSAGE_FAILED_VALIDATION, "Field is missing");
			}

			++iter;

			return status;
		}

		public void reset()
		{
			iter = 0;
		}
	}
}
