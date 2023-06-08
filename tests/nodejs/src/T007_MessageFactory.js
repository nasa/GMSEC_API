#!/usr/bin/env node

const TestCase = require('./TestCase.js');
const gmsec = require('../../../bin/gmsec_nodejs.node');


class T007_MessageFactory extends TestCase
{
	constructor()
	{
		super();
	}

	testCase()
	{
		this.testConstructor();
		this.testSetStandardFields();
		this.testClearStandardFields();
		this.testSetMessageConfig();
		this.testCreateMessage_1();
		this.testCreateMessage_2();
		this.testFromData();
		this.testGetSpecification();
	}

	testConstructor()
	{
		gmsec.Log.info("Test constructor");

		try {
			// Without config
			var msgFactory1 = new gmsec.MessageFactory();
			this.check("Unexpected specification version", msgFactory1.getSpecification().getVersion() == gmsec.GMSEC_MSG_SPEC_CURRENT);
			this.check("Unexpected specification schema level", msgFactory1.getSpecification().getSchemaLevel() == gmsec.Specification.SchemaLevel_LEVEL_0);

			// With config
			var config = new gmsec.Config("gmsec-schema-level=2", gmsec.DataType_KEY_VALUE_DATA);
			var msgFactory2 = new gmsec.MessageFactory(config);
			this.check("Unexpected specification version", msgFactory2.getSpecification().getVersion() == gmsec.GMSEC_MSG_SPEC_CURRENT);
			this.check("Unexpected specification schema level", msgFactory2.getSpecification().getSchemaLevel() == gmsec.Specification.SchemaLevel_LEVEL_2);
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testSetStandardFields()
	{
		gmsec.Log.info("Test setStandardFields()");

		try {
			var msgFactory = new gmsec.MessageFactory();
			this.setStandardFields(msgFactory);

			var msg = msgFactory.createMessage();
			this.verifyStandardFields(msg);
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testClearStandardFields()
	{
		gmsec.Log.info("Test clearStandardFields()");

		try {
			var msgFactory = new gmsec.MessageFactory();
			this.setStandardFields(msgFactory);

			msgFactory.clearStandardFields();

			var msg = msgFactory.createMessage();
			this.check("Expected message field count to be 0 (zero)", msg.getFieldCount() == 0);
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testSetMessageConfig()
	{
		gmsec.Log.info("Test setMessageConfig()");

		try {
			var msgFactory = new gmsec.MessageFactory();
			var config = new gmsec.Config("tracking=false", gmsec.DataType_KEY_VALUE_DATA);

			msgFactory.setMessageConfig(config);

			var msg = msgFactory.createMessage();
			this.check("Unexpected message config", msg.getConfig().toXML() === config.toXML());
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testCreateMessage_1()
	{
		gmsec.Log.info("Test createMessage() w/ no args");

		try {
			var msgFactory = new gmsec.MessageFactory();
			var msg = msgFactory.createMessage();
			this.check("Expected no fields in the message", msg.getFieldCount() == 0);
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testCreateMessage_2()
	{
		gmsec.Log.info("Test createMessage()");

		try {
			var msgFactory = new gmsec.MessageFactory();

			// Nominal case (valid schema ID)
			var msg = msgFactory.createMessage("HB");
			this.check("Unexpected MESSAGE-TYPE", msg.getStringValue("MESSAGE-TYPE") === "MSG");
			this.check("Unexpected MESSAGE-SUBTYPE", msg.getStringValue("MESSAGE-SUBTYPE") === "HB");

			// Off-nominal case (invalid schema ID)
			try {
				msg = msgFactory.createMessage("BOGUS");
				this.check("An exception was expected", false);
			}
			catch (e) {
				//SWIG BUG this.check(e.message, e.message.includes("SchemaID \"BOGUS\" could not be found in list of available schema"));
				this.check(e.message, e.message.includes("Illegal arguments for function createMessage"));
			}

			// Off-nominal case (empty-string schema ID)
			try {
				msg = msgFactory.createMessage("");
				this.check("An exception was expected", false);
			}
			catch (e) {
				//SWIG BUG this.check(e.message, e.message.includes("SchemaID cannot be NULL, nor be an empty string"));
				this.check(e.message, e.message.includes("Illegal arguments for function createMessage"));
			}
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testFromData()
	{
		gmsec.Log.info("Test fromData()");

		var xml = "";
		xml += "<MESSAGE SUBJECT=\"GMSEC.MSSN.SAT1.MSG.HB.GMPUB\" KIND=\"PUBLISH\">\n";
		xml += "\t<FIELD NAME=\"COMPONENT\" TYPE=\"STRING\">GMPUB</FIELD>\n";
		xml += "\t<FIELD NAME=\"CONSTELLATION-ID\" TYPE=\"STRING\">CNS1</FIELD>\n";
		xml += "\t<FIELD NAME=\"CONTENT-VERSION\" TYPE=\"F32\" BITS=\"44FC6000\">2019</FIELD>\n";
		xml += "\t<FIELD NAME=\"COUNTER\" TYPE=\"U16\">1</FIELD>\n";
		xml += "\t<FIELD NAME=\"DOMAIN1\" TYPE=\"STRING\">DOM1</FIELD>\n";
		xml += "\t<FIELD NAME=\"DOMAIN2\" TYPE=\"STRING\">DOM2</FIELD>\n";
		xml += "\t<FIELD NAME=\"FACILITY\" TYPE=\"STRING\">MY-FACILITY</FIELD>\n";
		xml += "\t<FIELD NAME=\"HEADER-VERSION\" TYPE=\"F32\" BITS=\"44FC6000\">2019</FIELD>\n";
		xml += "\t<FIELD NAME=\"MESSAGE-SUBTYPE\" TYPE=\"STRING\">HB</FIELD>\n";
		xml += "\t<FIELD NAME=\"MESSAGE-TYPE\" TYPE=\"STRING\">MSG</FIELD>\n";
		xml += "\t<FIELD NAME=\"MISSION-ID\" TYPE=\"STRING\">MSSN</FIELD>\n";
		xml += "\t<FIELD NAME=\"PUB-RATE\" TYPE=\"U16\">1</FIELD>\n";
		xml += "\t<FIELD NAME=\"SAT-ID-LOGICAL\" TYPE=\"STRING\">SAT1</FIELD>\n";
		xml += "\t<FIELD NAME=\"SAT-ID-PHYSICAL\" TYPE=\"STRING\">SAT1</FIELD>\n";
		xml += "\t<FIELD NAME=\"SPECIFICATION\" TYPE=\"STRING\">GMSEC</FIELD>\n";
		xml += "</MESSAGE>";

		var json = "";
		json += "{\"MESSAGE\":{\"SUBJECT\":\"GMSEC.MSSN.SAT1.MSG.HB.GMPUB\",\"KIND\":\"PUBLISH\",";
		json += "\"FIELD\":[";
		json += "{\"NAME\":\"COMPONENT\",\"TYPE\":\"STRING\",\"VALUE\":\"GMPUB\"},";
		json += "{\"NAME\":\"CONSTELLATION-ID\",\"TYPE\":\"STRING\",\"VALUE\":\"CNS1\"},";
		json += "{\"NAME\":\"CONTENT-VERSION\",\"TYPE\":\"F32\",\"BITS\":\"44FC6000\",\"VALUE\":\"2019\"},";
		json += "{\"NAME\":\"COUNTER\",\"TYPE\":\"U16\",\"VALUE\":\"1\"},";
		json += "{\"NAME\":\"DOMAIN1\",\"TYPE\":\"STRING\",\"VALUE\":\"DOM1\"},";
		json += "{\"NAME\":\"DOMAIN2\",\"TYPE\":\"STRING\",\"VALUE\":\"DOM2\"},";
		json += "{\"NAME\":\"FACILITY\",\"TYPE\":\"STRING\",\"VALUE\":\"MY-FACILITY\"},";
		json += "{\"NAME\":\"HEADER-VERSION\",\"TYPE\":\"F32\",\"BITS\":\"44FC6000\",\"VALUE\":\"2019\"},";
		json += "{\"NAME\":\"MESSAGE-SUBTYPE\",\"TYPE\":\"STRING\",\"VALUE\":\"HB\"},";
		json += "{\"NAME\":\"MESSAGE-TYPE\",\"TYPE\":\"STRING\",\"VALUE\":\"MSG\"},";
		json += "{\"NAME\":\"MISSION-ID\",\"TYPE\":\"STRING\",\"VALUE\":\"MSSN\"},";
		json += "{\"NAME\":\"PUB-RATE\",\"TYPE\":\"U16\",\"VALUE\":\"1\"},";
		json += "{\"NAME\":\"SAT-ID-LOGICAL\",\"TYPE\":\"STRING\",\"VALUE\":\"SAT1\"},";
		json += "{\"NAME\":\"SAT-ID-PHYSICAL\",\"TYPE\":\"STRING\",\"VALUE\":\"SAT1\"},";
		json += "{\"NAME\":\"SPECIFICATION\",\"TYPE\":\"STRING\",\"VALUE\":\"GMSEC\"}";
		json += "]}}";

		try {
			var config = new gmsec.Config("gmsec-schema-level=2", gmsec.DataType_KEY_VALUE_DATA);
			var msgFactory = new gmsec.MessageFactory(config);
			var msg1 = msgFactory.fromData(xml, gmsec.DataType_XML_DATA);
			var msg2 = msgFactory.fromData(json, gmsec.DataType_JSON_DATA);

			// Nominal tests
			this.check("Unexpected XML", msg1.toXML() === xml);
			this.check("Unexpected JSON", msg2.toJSON() === json);

			// Off-nominal tests
			var invalid_msg_subject = "<MESSAGE SUBJECT=\"GMSEC FOO BAR\"/>";
			var invalid_xml         = "<MESSAGE SUBJECT=\"GMSEC.FOO.BAR\">";
			var invalid_json        = "{\"MESSAGE\":{\"SUBJECT\":\"GMSEC.FOO.BAR\"";
			var empty_str           = "";

			try {
				msgFactory.fromData(invalid_msg_subject, gmsec.DataType_XML_DATA);
				this.check("Expected exception from invalid subject", false);
			}
			catch (e) {
				this.check(e.message, e.message.includes("Invalid message subject"));
			}

			try {
				msgFactory.fromData(invalid_xml, gmsec.DataType_XML_DATA);
				this.check("Expected exception from invalid XML", false);
			}
			catch (e) {
				this.check(e.message, e.message.includes("Mismatched element"));
			}

			try {
				var msgX = msgFactory.fromData(invalid_json, gmsec.DataType_JSON_DATA);
				this.check("Expected exception from invalid JSON", false);
			}
			catch (e) {
				this.check(e.message, e.message.includes("Invalid JSON message format -- invalid JSON string data"));
			}

			try {
				msgFactory.fromData(empty_str, gmsec.DataType_XML_DATA);
				this.check("Expected exception from using empty string", false);
			}
			catch (e) {
				this.check(e.message, e.message.includes("Data string cannot be NULL, nor empty"));
			}
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testGetSpecification()
	{
		gmsec.Log.info("Test getSpecification()");

		try {
			var config = new gmsec.Config("gmsec-specification-version=201900", gmsec.DataType_KEY_VALUE_DATA);
			var msgFactory1 = new gmsec.MessageFactory();
			var msgFactory2 = new gmsec.MessageFactory(config);

			this.check("Unexpected specification version", msgFactory1.getSpecification().getVersion() == gmsec.GMSEC_MSG_SPEC_CURRENT);
			this.check("Unexpected specification version", msgFactory2.getSpecification().getVersion() == gmsec.GMSEC_MSG_SPEC_2019_00);
		}
		catch (e) {
			this.require(e.message, false);
		}
	}
}

test = new T007_MessageFactory();
test.setDescription("Test MessageFactory");
test.run(process.argv);
