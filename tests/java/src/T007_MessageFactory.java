import gov.nasa.gsfc.gmsec.api5.Config;
import gov.nasa.gsfc.gmsec.api5.Gmsec;
import gov.nasa.gsfc.gmsec.api5.GmsecException;
import gov.nasa.gsfc.gmsec.api5.Message;
import gov.nasa.gsfc.gmsec.api5.MessageFactory;
import gov.nasa.gsfc.gmsec.api5.MessageValidator;
import gov.nasa.gsfc.gmsec.api5.Specification;
import gov.nasa.gsfc.gmsec.api5.Status;
import gov.nasa.gsfc.gmsec.api5.StatusClassification;
import gov.nasa.gsfc.gmsec.api5.StatusCode;

import gov.nasa.gsfc.gmsec.api5.util.Log;


public class T007_MessageFactory extends TestCase
{
	public static void main(String[] args)
	{
		T007_MessageFactory test = new T007_MessageFactory();
		test.setDescription("Test MessageFactory");
		test.run(args);
	}


    public void testCase()
		throws Exception
	{
		try
		{
			test_create();
			test_set_standard_fields();
			test_clear_standard_fields();
			test_set_message_config();
			test_create_message_1();
			test_create_message_2();
			test_from_data();
			test_get_specification();
			test_register_message_validator();
		}
		catch (Exception e)
		{
			require(e.getMessage(), false);
		}
	}


	private void test_create()
		throws Exception
	{
		Log.info("Test create and destroy");

		// No config
		MessageFactory msgFactory = MessageFactory.create();

		check("Unexpected specification version", msgFactory.getSpecification().getVersion() == Gmsec.MSG_SPEC_CURRENT);
		check("Unexpected specification schema level", msgFactory.getSpecification().getSchemaLevel() == Specification.SchemaLevel.LEVEL_0);

		MessageFactory.destroy(msgFactory);


		// With config
		Config config = new Config("gmsec-schema-level=2", Gmsec.DataType.KEY_VALUE_DATA);

		msgFactory = MessageFactory.create(config);

		check("Unexpected specification version", msgFactory.getSpecification().getVersion() == Gmsec.MSG_SPEC_CURRENT);
		check("Unexpected specification schema level", msgFactory.getSpecification().getSchemaLevel() == Specification.SchemaLevel.LEVEL_2);

		MessageFactory.destroy(msgFactory);
	}


	private void test_set_standard_fields()
		throws Exception
	{
		Log.info("Test setStandardFields()");

		MessageFactory msgFactory = MessageFactory.create();

		setStandardFields(msgFactory);

		Message msg = msgFactory.createMessage();

		verifyStandardFields(msg);

		msgFactory.clearStandardFields();

		MessageFactory.destroy(msgFactory);
	}


	private void test_clear_standard_fields()
		throws Exception
	{
		Log.info("Test clearStandardFields()");

		MessageFactory msgFactory = MessageFactory.create();

		setStandardFields(msgFactory);

		Message msg1 = msgFactory.createMessage();

		verifyStandardFields(msg1);

		msgFactory.clearStandardFields();

		Message msg2 = msgFactory.createMessage();

		check("Expected message field count to be 0 (zero)", msg2.getFieldCount() == 0);

		MessageFactory.destroy(msgFactory);
	}


	private void test_set_message_config()
		throws Exception
	{
		Log.info("Test setMessageConfig()");

		MessageFactory msgFactory = MessageFactory.create();

		//o Nominal case
		{
			Config msgConfig = new Config("tracking=false", Gmsec.DataType.KEY_VALUE_DATA);

			msgFactory.setMessageConfig(msgConfig);

			Message msg = msgFactory.createMessage();

			check("Unexpected message config", msg.getConfig().toXML().equals(msgConfig.toXML()));
		}

		//o Off-nominal case
		try {
			msgFactory.setMessageConfig(null);
			check("Expected an exception", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Config is null"));
		}

		MessageFactory.destroy(msgFactory);
	}


	private void test_create_message_1()
		throws Exception
	{
		Log.info("Test createMessage() w/ no args");

		MessageFactory msgFactory = MessageFactory.create();

		Message msg = msgFactory.createMessage();

		check("Expected no fields in the message", msg.getFieldCount() == 0);

		MessageFactory.destroy(msgFactory);
	}


	private void test_create_message_2()
		throws Exception
	{
		Log.info("Test createMessage()");

		MessageFactory msgFactory = MessageFactory.create();

		//o Nominal case (valid schema ID)
		{
			Message msg = msgFactory.createMessage("HB");

			check("Unexpected MESSAGE-TYPE", msg.getStringValue("MESSAGE-TYPE").equals("MSG"));
			check("Unexpected MESSAGE-SUBTYPE", msg.getStringValue("MESSAGE-SUBTYPE").equals("HB"));
		}

		// Off-nominal case (invalid schema ID)
		{
			try {
				Message msg = msgFactory.createMessage("BOGUS");
				check("Expected an exception", false);
			}
			catch (GmsecException e) {
				check(e.getMessage(), e.getMessage().contains("SchemaID \"BOGUS\" could not be found in list of available schema"));
			}
		}

		// Off-nominal case (empty-string schema ID)
		{
			try {
				Message msg = msgFactory.createMessage("");
				check("Expected an exception", false);
			}
			catch (IllegalArgumentException e) {
				check(e.getMessage(), e.getMessage().contains("SchemaID cannot be null, nor be an empty string"));
			}
		}

		// Off-nominal case (null schema ID)
		{
			try {
				Message msg = msgFactory.createMessage(null);
				check("Expected an exception", false);
			}
			catch (IllegalArgumentException e) {
				check(e.getMessage(), e.getMessage().contains("SchemaID cannot be null, nor be an empty string"));
			}
		}

		MessageFactory.destroy(msgFactory);
	}


	private void test_from_data()
		throws Exception
	{
		Log.info("Test fromData()");

		//o Nominal tests
		{
			String xml = ""
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

			String xml_lower = ""
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

			String json = ""
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

			String json_lower = ""
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

			MessageFactory msgFactory = MessageFactory.create( new Config("gmsec-schema-level=2", Gmsec.DataType.KEY_VALUE_DATA) );

			Message msg1 = from_data(msgFactory, xml, Gmsec.DataType.XML_DATA, null);
			Message msg2 = from_data(msgFactory, json, Gmsec.DataType.JSON_DATA, null);
			Message msg3 = from_data(msgFactory, xml_lower, Gmsec.DataType.XML_DATA, null);
			Message msg4 = from_data(msgFactory, json_lower, Gmsec.DataType.JSON_DATA, null);

			MessageFactory.destroy(msgFactory);

			check("Unexpected XML",  msg1.toXML().equals(xml));
			check("Unexpected JSON", msg2.toJSON().equals(json));
			check("Unexpected XML",  msg3.toXML().equals(xml));
			check("Unexpected JSON", msg4.toJSON().equals(json));

			check("Message 1 missing CONTENT-VERSION", msg1.hasField("CONTENT-VERSION"));
			check("Message 1 missing HEADER-VERSION", msg1.hasField("HEADER-VERSION"));
			check("Message 1 missing MESSAGE-SUBTYPE", msg1.hasField("MESSAGE-SUBTYPE"));
			check("Message 1 missing MESSAGE-TYPE", msg1.hasField("MESSAGE-TYPE"));
			check("Message 1 missing PUB-RATE", msg1.hasField("PUB-RATE"));

			check("Message 2 missing CONTENT-VERSION", msg2.hasField("CONTENT-VERSION"));
			check("Message 2 missing HEADER-VERSION", msg2.hasField("HEADER-VERSION"));
			check("Message 2 missing MESSAGE-SUBTYPE", msg2.hasField("MESSAGE-SUBTYPE"));
			check("Message 2 missing MESSAGE-TYPE", msg2.hasField("MESSAGE-TYPE"));
			check("Message 2 missing PUB-RATE", msg2.hasField("PUB-RATE"));

			check("Message 3 missing CONTENT-VERSION", msg3.hasField("CONTENT-VERSION"));
			check("Message 3 missing HEADER-VERSION", msg3.hasField("HEADER-VERSION"));
			check("Message 3 missing MESSAGE-SUBTYPE", msg3.hasField("MESSAGE-SUBTYPE"));
			check("Message 3 missing MESSAGE-TYPE", msg3.hasField("MESSAGE-TYPE"));
			check("Message 3 missing PUB-RATE", msg3.hasField("PUB-RATE"));

			check("Message 4 missing CONTENT-VERSION", msg4.hasField("CONTENT-VERSION"));
			check("Message 4 missing HEADER-VERSION", msg4.hasField("HEADER-VERSION"));
			check("Message 4 missing MESSAGE-SUBTYPE", msg4.hasField("MESSAGE-SUBTYPE"));
			check("Message 4 missing MESSAGE-TYPE", msg4.hasField("MESSAGE-TYPE"));
			check("Message 4 missing PUB-RATE", msg4.hasField("PUB-RATE"));
		}

		// Off-nominal tests
		{
			MessageFactory msgFactory = MessageFactory.create();

			String invalid_msg_subject = "<MESSAGE SUBJECT=\"GMSEC FOO BAR\"/>";
			String invalid_xml         = "<MESSAGE SUBJECT=\"GMSEC.FOO.BAR\">";
			String invalid_json        = "{\"MESSAGE\":{\"SUBJECT\":\"GMSEC.FOO.BAR\"";
			String empty_str           = "";
			String null_str            = null;

			from_data(msgFactory, invalid_msg_subject, Gmsec.DataType.XML_DATA, "Invalid message subject");
			from_data(msgFactory, invalid_xml, Gmsec.DataType.XML_DATA, "Mismatched element");
			from_data(msgFactory, invalid_json, Gmsec.DataType.JSON_DATA, "Invalid JSON message format -- invalid JSON string data");
			from_data(msgFactory, empty_str, Gmsec.DataType.XML_DATA, "Message data is null or contains an empty string");
			from_data(msgFactory, null_str, Gmsec.DataType.JSON_DATA, "Message data is null or contains an empty string");

			MessageFactory.destroy(msgFactory);
		}
	}


	private void test_get_specification()
		throws Exception
	{
		Log.info("Test getSpecification()");

		MessageFactory msgFactory1 = MessageFactory.create();
		MessageFactory msgFactory2 = MessageFactory.create( new Config("gmsec-specification-version=201900", Gmsec.DataType.KEY_VALUE_DATA) );

		check("Unexpected specification version", msgFactory1.getSpecification().getVersion() == Gmsec.MSG_SPEC_CURRENT);
		check("Unexpected specification version", msgFactory2.getSpecification().getVersion() == Gmsec.MSG_SPEC_2019_00);

		MessageFactory.destroy(msgFactory1);
		MessageFactory.destroy(msgFactory2);
	}


	private void test_register_message_validator()
		throws Exception
	{
		Log.info("Test registerMessageValidator()");

		MyValidator val1 = new MyValidator(true);
		MyValidator val2 = new MyValidator(false);

		//o Nominal case
		{
			MessageFactory msgFactory1 = MessageFactory.create();

			setStandardFields(msgFactory1);
			msgFactory1.registerMessageValidator(val1);

			Message msg1 = msgFactory1.createMessage("HB");
			Status status1 = msg1.isCompliant();
			check("Expected message to be compliant", status1.hasError() == false);

			MessageFactory.destroy(msgFactory1);


			MessageFactory msgFactory2 = MessageFactory.create();

			msgFactory2.registerMessageValidator(val2);

			Message msg2 = msgFactory2.createMessage("HB");
			Status status2 = msg2.isCompliant();
			check("Expected message to be non-compliant", status2.hasError());
			check(status2.getReason(), status2.getReason().contains("MyValidator is not happy with the message"));

			MessageFactory.destroy(msgFactory2);
		}

		//o Off-nominal case
		MessageFactory msgFactory = null;
		try {
			msgFactory = MessageFactory.create();
			msgFactory.registerMessageValidator(null);
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("MessageValidator cannot be null"));
		}
		finally {
			if (msgFactory != null) {
				MessageFactory.destroy(msgFactory);
			}
		}
	}


	private Message from_data(MessageFactory msgFactory, String data, Gmsec.DataType type, String expectedError)
	{
		Message msg = null;

		try
		{
			msg = msgFactory.fromData(data, type);

			if (expectedError != null)
			{
				check("Expected an error: " + expectedError, false);
			}
		}
		catch (IllegalArgumentException | GmsecException e)
		{
			if (expectedError != null)
			{
				check(e.getMessage(), e.getMessage().contains(expectedError));
			}
			else
			{
				check(e.getMessage(), false);
			}
		}

		return msg;
	}
}


class MyValidator extends MessageValidator
{
	private boolean m_nominal;
	private boolean m_used;

	public MyValidator(boolean nominal)
	{
		m_nominal = nominal;
		m_used    = false;
	}

	public boolean wasUsed()
	{
		return m_used;
	}

	public void reset()
	{
		m_used = false;
	}

	public Status validateMessage(Message msg)
	{
		Status status = new Status();

		if (!m_nominal)
		{
			status.set(StatusClassification.MSG_ERROR, StatusCode.MESSAGE_FAILED_VALIDATION, "MyValidator is not happy with the message");
		}

		return status;
	}
}
