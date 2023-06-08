#!/usr/bin/env node

const TestCase = require('./TestCase.js');
const gmsec = require('../../../bin/gmsec_nodejs.node');


class T002_Config extends TestCase
{
	constructor()
	{
		super();
	}

	testCase()
	{
		this.testConstructor_1();
		this.testConstructor_2();
		this.testConstructor_3();
		this.testConstructor_4();
		this.testCopyConstructor();
		this.testAddValue();
		this.testClearValue();
		this.testGetValue();
		this.testGetBooleanValue();
		this.testGetIntegerValue();
		this.testGetDoubleValue();
		this.testClear();
		this.testMerge();
		this.testToXML();
		this.testFromXML();
		this.testGetFromFile();
		this.testToJSON();
	}

	testConstructor_1()
	{
		gmsec.Log.info("Test constructor (no args)");

		var cp = new gmsec.ConfigPair();

		var c1 = new gmsec.Config();
		this.check("Expected no config entries", c1.getFirst(cp) == false);
		this.check("Expected name to be null", cp.getName() == null);
		this.check("Expected value to be null", cp.getValue() == null);

		this.check("Expected no config entries", c1.getNext(cp) == false);
		this.check("Expected name to be null", cp.getName() == null);
		this.check("Expected value to be null", cp.getValue() == null);
	}

	testConstructor_2()
	{
		gmsec.Log.info("Test cmd-line arg constructor");

		var cp = new gmsec.ConfigPair();

		var args = new gmsec.StringArray();
		args.add("mw-id=bolt");
		args.add("mw-server=localhost");

		var c1 = new gmsec.Config(args);
		this.check("Expected config entry", c1.getFirst(cp));
		this.check("Expected mw-id", cp.getName() == "mw-id");
		this.check("Expected bolt", cp.getValue() == "bolt");

		this.check("Expected config entry", c1.getNext(cp));
		this.check("Expected mw-server", cp.getName() == "mw-server");
		this.check("Expected localhost", cp.getValue() == "localhost");

		this.check("Expected no additional config entries", c1.getNext(cp) == false);
	}

	testConstructor_3()
	{
		gmsec.Log.info("Test key-value pair constructor");

		var cp = new gmsec.ConfigPair();

		// Using key-value pairs
		var c1 = new gmsec.Config("mw-id=bolt mw-server=localhost", gmsec.DataType_KEY_VALUE_DATA);
		this.check("Expected config entry", c1.getFirst(cp));
		this.check("Expected mw-id", cp.getName() == "mw-id");
		this.check("Expected bolt", cp.getValue() == "bolt");

		this.check("Expected config entry", c1.getNext(cp));
		this.check("Expected mw-server", cp.getName() == "mw-server");
		this.check("Expected localhost", cp.getValue() == "localhost");

		this.check("Expected no additional config entries", c1.getNext(cp) == false);

		// Using XML content
		var c2 = new gmsec.Config(c1.toXML(), gmsec.DataType_XML_DATA);
		this.check("Expected config entry", c2.getFirst(cp));
		this.check("Expected mw-id", cp.getName() == "mw-id");
		this.check("Expected bolt", cp.getValue() == "bolt");

		this.check("Expected config entry", c2.getNext(cp));
		this.check("Expected mw-server", cp.getName() == "mw-server");
		this.check("Expected localhost", cp.getValue() == "localhost");

		this.check("Expected no additional config entries", c2.getNext(cp) == false);

		// Using JSON content
		var c3 = new gmsec.Config(c1.toJSON(), gmsec.DataType_JSON_DATA);
		this.check("Expected config entry", c3.getFirst(cp));
		this.check("Expected mw-id", cp.getName() == "mw-id");
		this.check("Expected bolt", cp.getValue() == "bolt");

		this.check("Expected config entry", c3.getNext(cp));
		this.check("Expected mw-server", cp.getName() == "mw-server");
		this.check("Expected localhost", cp.getValue() == "localhost");

		this.check("Expected no additional config entries", c3.getNext(cp) == false);
	}


	testConstructor_4()
	{
		gmsec.Log.info("Test JSON string constructor");

		let cp = new gmsec.ConfigPair();

		const args = [ "mw-id=bolt", "mw-server=localhost" ];
		const c1   = new gmsec.Config(JSON.stringify(args));

		this.check("Expected config entry", c1.getFirst(cp));
		this.check("Expected mw-id", cp.getName() == "mw-id");
		this.check("Expected bolt", cp.getValue() == "bolt");

		this.check("Expected config entry", c1.getNext(cp));
		this.check("Expected mw-server", cp.getName() == "mw-server");
		this.check("Expected localhost", cp.getValue() == "localhost");

		this.check("Expected no additional config entries", c1.getNext(cp) == false);
	}


	testCopyConstructor()
	{
		gmsec.Log.info("Test copy constructor");

		var cp = new gmsec.ConfigPair();

		var c1 = new gmsec.Config("mw-id=bolt mw-server=localhost", gmsec.DataType_KEY_VALUE_DATA);
		var c2 = new gmsec.Config(c1);

		this.check("Expected config entry", c2.getFirst(cp));
		this.check("Expected mw-id", cp.getName() == "mw-id");
		this.check("Expected bolt", cp.getValue() == "bolt");

		this.check("Expected config entry", c2.getNext(cp));
		this.check("Expected mw-server", cp.getName() == "mw-server");
		this.check("Expected localhost", cp.getValue() == "localhost");

		this.check("Expected no additional config entries", c2.getNext(cp) == false);
	}

	testAddValue()
	{
		gmsec.Log.info("Test addValue()");

		var cp = new gmsec.ConfigPair();

		var c1 = new gmsec.Config();
		c1.addValue("mw-id", "bolt");

		this.check("Expected config entry", c1.getFirst(cp));
		this.check("Expected mw-id", cp.getName() == "mw-id");
		this.check("Expected bolt", cp.getValue() == "bolt");

		this.check("Expected no additional config entries", c1.getNext(cp) == false);
	}

	testClearValue()
	{
		gmsec.Log.info("Test clearValue()");

		var cp = new gmsec.ConfigPair();

		var c1 = new gmsec.Config("mw-id=bolt", gmsec.DataType_KEY_VALUE_DATA);

		this.check("Expected false when clearing non-existing key", c1.clearValue("mw-server") == false);
		this.check("Expected true when clearing existing key", c1.clearValue("mw-id"));

		this.check("Expected no additional config entries", c1.getFirst(cp) == false);
	}

	testGetValue()
	{
		gmsec.Log.info("Test getValue()");

		var c1 = new gmsec.Config("mw-id=bolt", gmsec.DataType_KEY_VALUE_DATA);

		this.check("Unexpected value 1", c1.getValue("mw-id") == "bolt");
		this.check("Unexpected value 2", c1.getValue("mw-server") == null);
		this.check("Unexpected value 3", c1.getValue("") == null);
		this.check("Unexpected value 5", c1.getValue("mw-server", "mb") == "mb");
	}

	testGetBooleanValue()
	{
		gmsec.Log.info("Test getBooleanValue()");

		var c1 = new gmsec.Config("gmsec-rocks=true", gmsec.DataType_KEY_VALUE_DATA);
		var c2 = new gmsec.Config("gmsec-rocks=untrue", gmsec.DataType_KEY_VALUE_DATA);

		this.check("Unexpected value", c1.getBooleanValue("gmsec-rocks"));

		this.check("Unexpected value", c1.getBooleanValue("gmsec-rocks", false));
		this.check("Unexpected value", c1.getBooleanValue("gmsec-missing", true));
		this.check("Unexpected value", c1.getBooleanValue("", true));

		try {
			c2.getBooleanValue("gmsec-rocks");
			this.check("An exception is expected", false);
		}
		catch (e) {
			this.check("Test is okay", true);
		}
	}

	testGetIntegerValue()
	{
		gmsec.Log.info("Test getIntegerValue()");

		var c1 = new gmsec.Config("gmsec-value=10", gmsec.DataType_KEY_VALUE_DATA);
		var c2 = new gmsec.Config("gmsec-value=NAN", gmsec.DataType_KEY_VALUE_DATA);

		this.check("Unexpected value", c1.getIntegerValue("gmsec-value") == 10);

		try {
			c2.getIntegerValue("gmsec-value");
			this.check("An exception is expected", false);
		}
		catch (e) {
			this.check("Test is okay", true);
		}

		this.check("Unexpected value", c1.getIntegerValue("gmsec-missing", 20) == 20);
		this.check("Unexpected value", c2.getIntegerValue("gmsec-value", 30) == 30);
		this.check("Unexpected value", c2.getIntegerValue("", 10) == 10);
	}

	testGetDoubleValue()
	{
		gmsec.Log.info("Test getDoubleValue()");

		var c1 = new gmsec.Config("gmsec-value=10.15", gmsec.DataType_KEY_VALUE_DATA);
		var c2 = new gmsec.Config("gmsec-value=NAN", gmsec.DataType_KEY_VALUE_DATA);

		this.check("Unexpected value", c1.getDoubleValue("gmsec-value") == 10.15);

		try {
			c2.getDoubleValue("gmsec-value");
			this.check("An exception is expected", false);
		}
		catch (e) {
			this.check("Test is okay", true);
		}

		this.check("Unexpected value", c1.getDoubleValue("gmsec-missing", 20.22) == 20.22);
		this.check("Unexpected value", c2.getDoubleValue("gmsec-value", 30.33) == 30.33);
		this.check("Unexpected value", c2.getDoubleValue("", 10.33) == 10.33);
	}

	testClear()
	{
		gmsec.Log.info("Test clear()");

		var cp = new gmsec.ConfigPair();

		var c1 = new gmsec.Config("mw-id=bolt mw-server=localhost", gmsec.DataType_KEY_VALUE_DATA);

		c1.clear();

		this.check("Unexpected result from getNext", c1.getNext(cp) == false);
		this.check("Unexpected result from getFirst", c1.getFirst(cp) == false);
	}

	testMerge()
	{
		gmsec.Log.info("Test merge()");

		var cp = new gmsec.ConfigPair();

		var c1 = new gmsec.Config("mw-id=bolt", gmsec.DataType_KEY_VALUE_DATA);
		var c2 = new gmsec.Config("mw-id=bolt", gmsec.DataType_KEY_VALUE_DATA);
		var c3 = new gmsec.Config("mw-id=mb mw-server=localhost", gmsec.DataType_KEY_VALUE_DATA);

		// Merge w/ overwrite
		c1.merge(c3, true);

		this.check("Expected to find first entry", c1.getFirst(cp));
		this.check("Unexpected entry name", cp.getName() == "mw-id");
		this.check("Unexpected entry value", cp.getValue() == "mb");

		this.check("Expected to find second entry", c1.getNext(cp));
		this.check("Unexpected entry name", cp.getName() == "mw-server");
		this.check("Unexpected entry value", cp.getValue() == "localhost");

		this.check("Was not expecting to find third entry", c1.getNext(cp) == false);

		// Merge w/ no overwrite	
		c2.merge(c3, false);

		this.check("Expected to find first entry", c2.getFirst(cp));
		this.check("Unexpected entry name", cp.getName() == "mw-id");
		this.check("Unexpected entry value", cp.getValue() == "bolt");

		this.check("Expected to find second entry", c2.getNext(cp));
		this.check("Unexpected entry name", cp.getName() == "mw-server");
		this.check("Unexpected entry value", cp.getValue() == "localhost");

		this.check("Was not expecting to find third entry", c2.getNext(cp) == false);
	}

	testToXML()
	{
		gmsec.Log.info("Test toXML()");

		var xml = "<CONFIG>\n" +
                  "\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n" +
                  "\t<PARAMETER NAME=\"mw-server\">localhost</PARAMETER>\n" +
                  "</CONFIG>";

		var c1 = new gmsec.Config("mw-id=bolt mw-server=localhost", gmsec.DataType_KEY_VALUE_DATA);

		this.check("Unexpected XML content", c1.toXML() == xml);
	}

	testFromXML()
	{
		gmsec.Log.info("Test fromXML()");

		var xml = "<CONFIG>\n" +
                  "\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n" +
                  "\t<PARAMETER NAME=\"mw-server\">localhost</PARAMETER>\n" +
                  "</CONFIG>";

		var c1 = new gmsec.Config();

		c1.fromXML(xml);

		this.check("Unexpected XML content", c1.toXML() == xml);
	}

	testGetFromFile()
	{
		gmsec.Log.info("Test getFromFile()");

		// Nominal tests
		try {
			var c1 = gmsec.Config.getFromFile( this.getDataFile("T002_Config_good.xml"), "Bolt");
			this.check("Unexpected mw-id value", c1.getValue("mw-id") == "bolt");
			this.check("Unexpected server value", c1.getValue("server") == "localhost");

			var c2 = gmsec.Config.getFromFile( this.getDataFile("T002_Config_good.xml"), "ActiveMQ");
			this.check("Unexpected mw-id value", c2.getValue("mw-id") == "activemq39");
			this.check("Unexpected server value", c2.getValue("server") == "tcp://localhost:61616");

			var c3 = gmsec.Config.getFromFile( this.getDataFile("T002_Config_good.xml"), "");
			this.check("Unexpected mw-id value", c3.getValue("mw-id") == "bolt");
			this.check("Unexpected server value", c3.getValue("server") == "localhost");
		}
		catch (e) {
			this.check(e.message, false);
		}

		// Off-nominal tests
		try {
			// named file does not exist
			var c1 = gmsec.Config.getFromFile( this.getDataFile("FooBar.xml"), "");
			this.check("Expected an exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Configuration file could not be found or opened"));
		}

		try {
			// named configuration does not exist
			var c1 = gmsec.Config.getFromFile( this.getDataFile("T002_Config_good.xml"), "FooBar");
			this.check("Expected an exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Named configuration could not be found"));
		}

		try {
			// malformed configuration file
			var c1 = gmsec.Config.getFromFile( this.getDataFile("T002_Config_bad.xml"), "Bolt");
			this.check("Expected an exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Invalid XML format -- parse error"));
		}

		try {
			// empty configuration file name
			var c1 = gmsec.Config.getFromFile("", "Bolt");
			this.check("Expected an exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Configuration file name cannot be NULL, nor be an empty string"));
		}
	}

	testToJSON()
	{
		gmsec.Log.info("Test toJSON()");

		var json = "{\"CONFIG\":{\"PARAMETER\":[{\"NAME\":\"mw-id\",\"VALUE\":\"bolt\"},{\"NAME\":\"mw-server\",\"VALUE\":\"localhost\"}]}}";

		var c1 = new gmsec.Config("mw-id=bolt mw-server=localhost", gmsec.DataType_KEY_VALUE_DATA);

		this.check("Unexpected JSON content", c1.toJSON() == json);
	}
}

test = new T002_Config();
test.setDescription("Test Config");
test.run(process.argv);
