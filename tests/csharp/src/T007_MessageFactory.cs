namespace T007
{
	using GMSEC.API5;
	using GMSEC.API5.Testing.Common;
	using System;
	using System.Text;


	class T007_MessageFactory : TestCase
	{
		static void Main(string[] args)
		{
			T007_MessageFactory test = new T007_MessageFactory();
			test.SetDescription("Test MessageFactory");
			test.Run(args);
		}


		public override void Run()
		{
			Test_Constructor_1();
			Test_Constructor_2();
			Test_SetStandardFields();
			Test_ClearStandardFields();
			Test_SetMessageConfig();
			Test_CreateMessage_1();
			Test_CreateMessage_2();
			Test_FromData();
			Test_GetSpecification();
			Test_RegisterMessageValidator();
		}


		private void Test_Constructor_1()
		{
			Log.Info("Test constructor (no args)");

			MessageFactory msgFactory = new MessageFactory();

			Check("Unexpected specification version", msgFactory.GetSpecification().GetVersion() == Gmsec.GMSEC_MSG_SPEC_CURRENT);
			Check("Unexpected specification schema level", msgFactory.GetSpecification().GetSchemaLevel() == Specification.SchemaLevel.LEVEL_0);
		}


		private void Test_Constructor_2()
		{
			Log.Info("Test constructor w/ Config");

			Config config = new Config("gmsec-schema-level=2", DataType.KEY_VALUE_DATA);

			MessageFactory msgFactory = new MessageFactory(config);

			Check("Unexpected specification version", msgFactory.GetSpecification().GetVersion() == Gmsec.GMSEC_MSG_SPEC_CURRENT);
			Check("Unexpected specification schema level", msgFactory.GetSpecification().GetSchemaLevel() == Specification.SchemaLevel.LEVEL_2);
		}


		private void Test_SetStandardFields()
		{
			Log.Info("Test SetStandardFields()");

			MessageFactory msgFactory = new MessageFactory();

			SetStandardFields(msgFactory);

			Message msg = msgFactory.CreateMessage();

			VerifyStandardFields(msg);
		}


		private void Test_ClearStandardFields()
		{
			Log.Info("Test ClearStandardFields()");

			MessageFactory msgFactory = new MessageFactory();

			SetStandardFields(msgFactory);

			Message msg1 = msgFactory.CreateMessage();

			VerifyStandardFields(msg1);

			msgFactory.ClearStandardFields();

			Message msg2 = msgFactory.CreateMessage();

			Check("Expected message field count to be 0 (zero)", msg2.GetFieldCount() == 0);
		}


		private void Test_SetMessageConfig()
		{
			Log.Info("Test SetMessageConfig()");

			MessageFactory msgFactory = new MessageFactory();

			//o Nominal case
			{
				Config msgConfig = new Config("tracking=false", DataType.KEY_VALUE_DATA);

				msgFactory.SetMessageConfig(msgConfig);

				Message msg = msgFactory.CreateMessage();

				Check("Unexpected message config", msg.GetConfig().ToXML() == msgConfig.ToXML());
			}

			//o Off-nominal case
			try {
				msgFactory.SetMessageConfig(null);
				Check("Expected an exception", false);
			}
			catch (ArgumentNullException e) {
				Check(e.ToString(), e.ToString().Contains("System.ArgumentNullException: gmsec::api5::Config const & is null"));
			}
		}


		private void Test_CreateMessage_1()
		{
			Log.Info("Test CreateMessage() w/ no args");

			MessageFactory msgFactory = new MessageFactory();

			Message msg = msgFactory.CreateMessage();

			Check("Expected no fields in the message", msg.GetFieldCount() == 0);
		}


		private void Test_CreateMessage_2()
		{
			Log.Info("Test CreateMessage()");

			MessageFactory msgFactory = new MessageFactory();

			//o Nominal case (valid schema ID)
			{
				Message msg = msgFactory.CreateMessage("HB");

				Check("Unexpected MESSAGE-TYPE", msg.GetStringValue("MESSAGE-TYPE") == "MSG");
				Check("Unexpected MESSAGE-SUBTYPE", msg.GetStringValue("MESSAGE-SUBTYPE") == "HB");
			}

			// Off-nominal case (invalid schema ID)
			{
				try {
					msgFactory.CreateMessage("BOGUS");
					Check("Expected an exception", false);
				}
				catch (GmsecException e) {
					Check(e.ToString(), e.ToString().Contains("SchemaID \"BOGUS\" could not be found in list of available schema"));
				}
			}

			// Off-nominal case (empty-string schema ID)
			{
				try {
					msgFactory.CreateMessage("");
					Check("Expected an exception", false);
				}
				catch (GmsecException e) {
					Check(e.ToString(), e.ToString().Contains("SchemaID cannot be NULL, nor be an empty string"));
				}
			}

			// Off-nominal case (null schema ID)
			{
				try {
					msgFactory.CreateMessage(null);
					Check("Expected an exception", false);
				}
				catch (GmsecException e) {
					Check(e.ToString(), e.ToString().Contains("SchemaID cannot be NULL, nor be an empty string"));
				}
			}
		}


		private void Test_FromData()
		{
			Log.Info("Test FromData()");

			//o Nominal tests
			{
				string xml = ""
					+ "<MESSAGE SUBJECT=\"GMSEC.MSSN.SAT1.MSG.HB.GMPUB\" KIND=\"PUBLISH\">\n"
					+ 	"\t<FIELD NAME=\"COMPONENT\" TYPE=\"STRING\">GMPUB</FIELD>\n"
					+ 	"\t<FIELD NAME=\"CONSTELLATION-ID\" TYPE=\"STRING\">CNS1</FIELD>\n"
					+ 	"\t<FIELD NAME=\"CONTENT-VERSION\" TYPE=\"F32\" BITS=\"44FC6000\">2019</FIELD>\n"
					+ 	"\t<FIELD NAME=\"COUNTER\" TYPE=\"U16\">1</FIELD>\n"
					+ 	"\t<FIELD NAME=\"DOMAIN1\" TYPE=\"STRING\">DOM1</FIELD>\n"
					+ 	"\t<FIELD NAME=\"DOMAIN2\" TYPE=\"STRING\">DOM2</FIELD>\n"
					+ 	"\t<FIELD NAME=\"FACILITY\" TYPE=\"STRING\">MY-FACILITY</FIELD>\n"
					+ 	"\t<FIELD NAME=\"HEADER-VERSION\" TYPE=\"F32\" BITS=\"44FC6000\">2019</FIELD>\n"
					+ 	"\t<FIELD NAME=\"MESSAGE-SUBTYPE\" TYPE=\"STRING\">HB</FIELD>\n"
					+ 	"\t<FIELD NAME=\"MESSAGE-TYPE\" TYPE=\"STRING\">MSG</FIELD>\n"
					+ 	"\t<FIELD NAME=\"MISSION-ID\" TYPE=\"STRING\">MSSN</FIELD>\n"
					+ 	"\t<FIELD NAME=\"PUB-RATE\" TYPE=\"U16\">1</FIELD>\n"
					+ 	"\t<FIELD NAME=\"SAT-ID-LOGICAL\" TYPE=\"STRING\">SAT1</FIELD>\n"
					+ 	"\t<FIELD NAME=\"SAT-ID-PHYSICAL\" TYPE=\"STRING\">SAT1</FIELD>\n"
					+ 	"\t<FIELD NAME=\"SPECIFICATION\" TYPE=\"STRING\">GMSEC</FIELD>\n"
					+ "</MESSAGE>";

				string xml_lower = ""
					+ "<message subject=\"GMSEC.MSSN.SAT1.MSG.HB.GMPUB\" kind=\"publish\">\n"
					+ 	"\t<field name=\"COMPONENT\" type=\"STRING\">GMPUB</field>\n"
					+ 	"\t<field name=\"CONSTELLATION-ID\" type=\"STRING\">CNS1</field>\n"
					+ 	"\t<field name=\"CONTENT-VERSION\" type=\"F32\" bits=\"44FC6000\">2019</field>\n"
					+ 	"\t<field name=\"COUNTER\" type=\"U16\">1</field>\n"
					+ 	"\t<field name=\"DOMAIN1\" type=\"STRING\">DOM1</field>\n"
					+ 	"\t<field name=\"DOMAIN2\" type=\"STRING\">DOM2</field>\n"
					+ 	"\t<field name=\"FACILITY\" type=\"STRING\">MY-FACILITY</field>\n"
					+ 	"\t<field name=\"HEADER-VERSION\" type=\"F32\" bits=\"44FC6000\">2019</field>\n"
					+ 	"\t<field name=\"MESSAGE-SUBTYPE\" type=\"STRING\">HB</field>\n"
					+ 	"\t<field name=\"MESSAGE-TYPE\" type=\"STRING\">MSG</field>\n"
					+ 	"\t<field name=\"MISSION-ID\" type=\"STRING\">MSSN</field>\n"
					+ 	"\t<field name=\"PUB-RATE\" type=\"U16\">1</field>\n"
					+ 	"\t<field name=\"SAT-ID-LOGICAL\" type=\"STRING\">SAT1</field>\n"
					+ 	"\t<field name=\"SAT-ID-PHYSICAL\" type=\"STRING\">SAT1</field>\n"
					+ 	"\t<field name=\"SPECIFICATION\" type=\"STRING\">GMSEC</field>\n"
					+ "</message>";

				string json = ""
					+ "{\"MESSAGE\":{\"SUBJECT\":\"GMSEC.MSSN.SAT1.MSG.HB.GMPUB\",\"KIND\":\"PUBLISH\","
					+ "\"FIELD\":["
					+ "{\"NAME\":\"COMPONENT\",\"TYPE\":\"STRING\",\"VALUE\":\"GMPUB\"},"
					+ "{\"NAME\":\"CONSTELLATION-ID\",\"TYPE\":\"STRING\",\"VALUE\":\"CNS1\"},"
					+ "{\"NAME\":\"CONTENT-VERSION\",\"TYPE\":\"F32\",\"BITS\":\"44FC6000\",\"VALUE\":\"2019\"},"
					+ "{\"NAME\":\"COUNTER\",\"TYPE\":\"U16\",\"VALUE\":\"1\"},"
					+ "{\"NAME\":\"DOMAIN1\",\"TYPE\":\"STRING\",\"VALUE\":\"DOM1\"},"
					+ "{\"NAME\":\"DOMAIN2\",\"TYPE\":\"STRING\",\"VALUE\":\"DOM2\"},"
					+ "{\"NAME\":\"FACILITY\",\"TYPE\":\"STRING\",\"VALUE\":\"MY-FACILITY\"},"
					+ "{\"NAME\":\"HEADER-VERSION\",\"TYPE\":\"F32\",\"BITS\":\"44FC6000\",\"VALUE\":\"2019\"},"
					+ "{\"NAME\":\"MESSAGE-SUBTYPE\",\"TYPE\":\"STRING\",\"VALUE\":\"HB\"},"
					+ "{\"NAME\":\"MESSAGE-TYPE\",\"TYPE\":\"STRING\",\"VALUE\":\"MSG\"},"
					+ "{\"NAME\":\"MISSION-ID\",\"TYPE\":\"STRING\",\"VALUE\":\"MSSN\"},"
					+ "{\"NAME\":\"PUB-RATE\",\"TYPE\":\"U16\",\"VALUE\":\"1\"},"
					+ "{\"NAME\":\"SAT-ID-LOGICAL\",\"TYPE\":\"STRING\",\"VALUE\":\"SAT1\"},"
					+ "{\"NAME\":\"SAT-ID-PHYSICAL\",\"TYPE\":\"STRING\",\"VALUE\":\"SAT1\"},"
					+ "{\"NAME\":\"SPECIFICATION\",\"TYPE\":\"STRING\",\"VALUE\":\"GMSEC\"}"
					+ "]}}";

				string json_lower = ""
					+ "{\"message\":{\"subject\":\"GMSEC.MSSN.SAT1.MSG.HB.GMPUB\",\"kind\":\"publish\","
					+ "\"field\":["
					+ "{\"name\":\"COMPONENT\",\"type\":\"string\",\"value\":\"GMPUB\"},"
					+ "{\"name\":\"CONSTELLATION-ID\",\"type\":\"string\",\"value\":\"CNS1\"},"
					+ "{\"name\":\"CONTENT-VERSION\",\"type\":\"f32\",\"bits\":\"44FC6000\",\"value\":\"2019\"},"
					+ "{\"name\":\"COUNTER\",\"type\":\"u16\",\"value\":\"1\"},"
					+ "{\"name\":\"DOMAIN1\",\"type\":\"string\",\"value\":\"DOM1\"},"
					+ "{\"name\":\"DOMAIN2\",\"type\":\"string\",\"value\":\"DOM2\"},"
					+ "{\"name\":\"FACILITY\",\"type\":\"string\",\"value\":\"MY-FACILITY\"},"
					+ "{\"name\":\"HEADER-VERSION\",\"type\":\"f32\",\"bits\":\"44FC6000\",\"value\":\"2019\"},"
					+ "{\"name\":\"MESSAGE-SUBTYPE\",\"type\":\"string\",\"value\":\"HB\"},"
					+ "{\"name\":\"MESSAGE-TYPE\",\"type\":\"string\",\"value\":\"MSG\"},"
					+ "{\"name\":\"MISSION-ID\",\"type\":\"string\",\"value\":\"MSSN\"},"
					+ "{\"name\":\"PUB-RATE\",\"type\":\"u16\",\"value\":\"1\"},"
					+ "{\"name\":\"SAT-ID-LOGICAL\",\"type\":\"string\",\"value\":\"SAT1\"},"
					+ "{\"name\":\"SAT-ID-PHYSICAL\",\"type\":\"string\",\"value\":\"SAT1\"},"
					+ "{\"name\":\"SPECIFICATION\",\"type\":\"string\",\"value\":\"GMSEC\"}"
					+ "]}}";

				MessageFactory msgFactory = new MessageFactory( new Config("gmsec-schema-level=2", DataType.KEY_VALUE_DATA) );

				Message msg1 = FromData(msgFactory, xml, DataType.XML_DATA, null);
				Message msg2 = FromData(msgFactory, json, DataType.JSON_DATA, null);
				Message msg3 = FromData(msgFactory, xml_lower, DataType.XML_DATA, null);
				Message msg4 = FromData(msgFactory, json_lower, DataType.JSON_DATA, null);

				Check("Unexpected XML",  msg1.ToXML() == xml);
				Check("Unexpected JSON", msg2.ToJSON() == json);
				Check("Unexpected XML",  msg3.ToXML() == xml);
				Check("Unexpected JSON", msg4.ToJSON() == json);

				Check("Message 1 missing CONTENT-VERSION", msg1.HasField("CONTENT-VERSION"));
				Check("Message 1 missing HEADER-VERSION", msg1.HasField("HEADER-VERSION"));
				Check("Message 1 missing MESSAGE-SUBTYPE", msg1.HasField("MESSAGE-SUBTYPE"));
				Check("Message 1 missing MESSAGE-TYPE", msg1.HasField("MESSAGE-TYPE"));
				Check("Message 1 missing PUB-RATE", msg1.HasField("PUB-RATE"));

				Check("Message 2 missing CONTENT-VERSION", msg2.HasField("CONTENT-VERSION"));
				Check("Message 2 missing HEADER-VERSION", msg2.HasField("HEADER-VERSION"));
				Check("Message 2 missing MESSAGE-SUBTYPE", msg2.HasField("MESSAGE-SUBTYPE"));
				Check("Message 2 missing MESSAGE-TYPE", msg2.HasField("MESSAGE-TYPE"));
				Check("Message 2 missing PUB-RATE", msg2.HasField("PUB-RATE"));

				Check("Message 3 missing CONTENT-VERSION", msg3.HasField("CONTENT-VERSION"));
				Check("Message 3 missing HEADER-VERSION", msg3.HasField("HEADER-VERSION"));
				Check("Message 3 missing MESSAGE-SUBTYPE", msg3.HasField("MESSAGE-SUBTYPE"));
				Check("Message 3 missing MESSAGE-TYPE", msg3.HasField("MESSAGE-TYPE"));
				Check("Message 3 missing PUB-RATE", msg3.HasField("PUB-RATE"));

				Check("Message 4 missing CONTENT-VERSION", msg4.HasField("CONTENT-VERSION"));
				Check("Message 4 missing HEADER-VERSION", msg4.HasField("HEADER-VERSION"));
				Check("Message 4 missing MESSAGE-SUBTYPE", msg4.HasField("MESSAGE-SUBTYPE"));
				Check("Message 4 missing MESSAGE-TYPE", msg4.HasField("MESSAGE-TYPE"));
				Check("Message 4 missing PUB-RATE", msg4.HasField("PUB-RATE"));
			}

			// Off-nominal tests
			{
				MessageFactory msgFactory = new MessageFactory();

				string invalid_msg_subject = "<MESSAGE SUBJECT=\"GMSEC FOO BAR\"/>";
				string invalid_xml         = "<MESSAGE SUBJECT=\"GMSEC.FOO.BAR\">";
				string invalid_json        = "{\"MESSAGE\":{\"SUBJECT\":\"GMSEC.FOO.BAR\"";
				string empty_str           = "";
				string null_str            = null;

				FromData(msgFactory, invalid_msg_subject, DataType.XML_DATA, "Invalid message subject");
				FromData(msgFactory, invalid_xml, DataType.XML_DATA, "Mismatched element");
				FromData(msgFactory, invalid_json, DataType.JSON_DATA, "Invalid JSON message format -- invalid JSON string data");
				FromData(msgFactory, empty_str, DataType.XML_DATA, "Data string cannot be NULL, nor empty");
				FromData(msgFactory, null_str, DataType.JSON_DATA, "Data string cannot be NULL, nor empty");
			}
		}


		private void Test_GetSpecification()
		{
			Log.Info("Test GetSpecification()");

			MessageFactory msgFactory1 = new MessageFactory();
			MessageFactory msgFactory2 = new MessageFactory( new Config("gmsec-specification-version=201900", DataType.KEY_VALUE_DATA) );

			Check("Unexpected specification version", msgFactory1.GetSpecification().GetVersion() == Gmsec.GMSEC_MSG_SPEC_CURRENT);
			Check("Unexpected specification version", msgFactory2.GetSpecification().GetVersion() == Gmsec.GMSEC_MSG_SPEC_2019_00);
		}


		private void Test_RegisterMessageValidator()
		{
			Log.Info("Test RegisterMessageValidator()");

			MyValidator val1 = new MyValidator(true);
			MyValidator val2 = new MyValidator(false);

			//o Nominal case
			{
				MessageFactory msgFactory1 = new MessageFactory();
				SetStandardFields(msgFactory1);
				msgFactory1.RegisterMessageValidator(val1);
				Message msg1 = msgFactory1.CreateMessage("HB");
				Status status1 = msg1.IsCompliant();
				Check("Expected message to be compliant", status1.HasError() == false);

				MessageFactory msgFactory2 = new MessageFactory();
				msgFactory2.RegisterMessageValidator(val2);
				Message msg2 = msgFactory2.CreateMessage("HB");
				Status status2 = msg2.IsCompliant();
				Check("Expected message to be non-compliant", status2.HasError());
				Check(status2.GetReason(), status2.GetReason().Contains("MyValidator is not happy with the message"));
			}

			//o Off-nominal case
			try {
				MessageFactory msgFactory = new MessageFactory();
				msgFactory.RegisterMessageValidator(null);
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("MessageValidator cannot be null"));
			}
		}


		private Message FromData(MessageFactory msgFactory, string data, DataType type, string expectedError)
		{
			Message msg = null;

			try
			{
				msg = msgFactory.FromData(data, type);

				if (expectedError != null)
				{
					Check("Expected an error: " + expectedError, false);
				}
			}
			catch (GmsecException e)
			{
				if (expectedError != null)
				{
					Check(e.ToString(), e.ToString().Contains(expectedError));
				}
				else
				{
					Check(e.ToString(), false);
				}
			}

			return msg;
		}
	}


	class MyValidator : MessageValidator
	{
		private bool m_nominal;
		private bool m_used;

		public MyValidator(bool nominal)
		{
			m_nominal = nominal;
			m_used    = false;
		}

		public bool WasUsed()
		{
			return m_used;
		}

		public void Reset()
		{
			m_used = false;
		}

		public override Status ValidateMessage(Message msg)
		{
			Status status = new Status();

			if (!m_nominal)
			{
				status.Set((int) StatusClass.MSG_ERROR, (int) StatusCode.MESSAGE_FAILED_VALIDATION, "MyValidator is not happy with the message");
			}

			return status;
		}
	}
}
