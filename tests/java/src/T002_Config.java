import gov.nasa.gsfc.gmsec.api5.Config;
import gov.nasa.gsfc.gmsec.api5.Gmsec;
import gov.nasa.gsfc.gmsec.api5.GmsecException;

import gov.nasa.gsfc.gmsec.api5.util.Log;


public class T002_Config extends TestCase
{
	public static void main(String[] args)
	{
		T002_Config test = new T002_Config();
		test.setDescription("Test Config");
		test.run(args);
	}


	public void testCase()
		throws Exception
	{
		try
		{
			test_constructor_1();
			test_constructor_2();
			test_constructor_3();
			test_copy_constructor();
			test_addValue();
			test_clearValue();
			test_getValue();
			test_getBooleanValue();
			test_getIntegerValue();
			test_getDoubleValue();
			test_clear();
			test_merge();
			test_toXML();
			test_fromXML();
			test_getFromFile();
			test_toJSON();
		}
		catch (Exception e)
		{
			require(e.getMessage(), false);
		}
	}

	private void test_constructor_1()
		throws Exception
	{
		Log.info("Test constructor (no-arg)");

		Config c1 = new Config();

		check("Unexpected XML content", c1.toXML().equals("<CONFIG>\n</CONFIG>"));
	}


	private void test_constructor_2()
		throws Exception
	{
		Log.info("Test constructor (command line args)");

		String[] args = new String[2];
		args[0] = "mw-id=bolt";
		args[1] = "server=localhost";

		Config c1 = new Config(args);

		check("Unexpected XML content", c1.toXML().equals("<CONFIG>\n\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n\t<PARAMETER NAME=\"server\">localhost</PARAMETER>\n</CONFIG>"));
	}


	private void test_constructor_3()
		throws Exception
	{
		Log.info("Test constructor (data args)");

		// Test: data constructor

		// Using string with options
		Config c1 = new Config("mw-id=bolt server=localhost", Gmsec.DataType.KEY_VALUE_DATA);
		check("Unexpected XML content", c1.toXML().equals("<CONFIG>\n\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n\t<PARAMETER NAME=\"server\">localhost</PARAMETER>\n</CONFIG>"));

		// Using XML
		Config c2 = new Config(c1.toXML(), Gmsec.DataType.XML_DATA);
		check("Unexpected XML content", c2.toXML().equals("<CONFIG>\n\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n\t<PARAMETER NAME=\"server\">localhost</PARAMETER>\n</CONFIG>"));

		// Using JSON
		Config c3 = new Config(c1.toJSON(), Gmsec.DataType.JSON_DATA);
		check("Unexpected XML content", c3.toXML().equals("<CONFIG>\n\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n\t<PARAMETER NAME=\"server\">localhost</PARAMETER>\n</CONFIG>"));

		// Using lowercase XML
		String lowerXML = c1.toXML().toLowerCase();

		Config c4 = new Config(lowerXML, Gmsec.DataType.XML_DATA);
		check("Unexpected XML content", c4.toXML().equals("<CONFIG>\n\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n\t<PARAMETER NAME=\"server\">localhost</PARAMETER>\n</CONFIG>"));

		// Using lowercase JSON
		String lowerJSON = c1.toJSON().toLowerCase();

		Config c5 = new Config(lowerJSON, Gmsec.DataType.JSON_DATA);
		check("Unexpected XML content", c5.toXML().equals("<CONFIG>\n\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n\t<PARAMETER NAME=\"server\">localhost</PARAMETER>\n</CONFIG>"));
	}


	private void test_copy_constructor()
		throws Exception
	{
		Log.info("Test copy-constructor");

		Config c1 = new Config("mw-id=bolt server=localhost", Gmsec.DataType.KEY_VALUE_DATA);
		Config c2 = new Config(c1);

		check("Config is not the same as the original", c1.toXML().equals(c2.toXML()));
	}


	private void test_addValue()
		throws Exception
	{
		Log.info("Test addValue()");

		Config c1 = new Config();

		check("Did not expect to find value", c1.getValue("mw-id") == null);

		c1.addValue("mw-id", "bolt");

		check("Expected to find value", c1.getValue("mw-id").equals("bolt"));
	}


	private void test_clearValue()
		throws Exception
	{
		Log.info("Test clearValue()");

		Config c1 = new Config("mw-id=bolt", Gmsec.DataType.KEY_VALUE_DATA);

		check("Expected to find value", c1.getValue("mw-id").equals("bolt"));

		c1.clearValue("mw-id");

		check("Did not expect to find value", c1.getValue("mw-id") == null);
	}


	private void test_getValue()
		throws Exception
	{
		Log.info("Test getValue()");

		Config c1 = new Config("mw-id=bolt", Gmsec.DataType.KEY_VALUE_DATA);

		check("Unexpected value", c1.getValue("mw-id").equals("bolt"));
		check("Unexpected value", c1.getValue("server") == null);

		Log.info("Test getValue() w/ default value");

		check("Unexpected default value", c1.getValue("connectionType", "mb").equals("mb"));
		check("Unexpected default value", c1.getValue(null, "mb").equals("mb"));
	}


	private void test_getBooleanValue()
		throws Exception
	{
		Log.info("Test getBooleanValue()");

		Config c1 = new Config("gmsec-rocks=true", Gmsec.DataType.KEY_VALUE_DATA);
		Config c2 = new Config("gmsec-rocks=sure", Gmsec.DataType.KEY_VALUE_DATA);

		check("Unexpected boolean value", true == c1.getBooleanValue("gmsec-rocks"));

		try {
			c1.getBooleanValue(null);
			check("Expected getBooleanValue() to throw exception", false);
		}
		catch (Exception e) {
			check(e.getMessage(), true);
		}
		try {
			c2.getBooleanValue("gmsec-rocks");
			check("Expected getBooleanValue() to throw exception", false);
		}
		catch (Exception e) {
			check(e.getMessage(), true);
		}

		Log.info("Test getBooleanValue() w/ default value");

		check("Expected getBooleanValue() to return default", false == c1.getBooleanValue("gmsec-missing", false));
		check("Expected getBooleanValue() to return default", true  == c1.getBooleanValue("gmsec-missing", true));
		check("Expected getBooleanValue() to return default", true  == c2.getBooleanValue("gmsec-rocks", true));
		check("Expected getBooleanValue() to return default", true  == c2.getBooleanValue("", true));
		check("Expected getBooleanValue() to return default", false == c2.getBooleanValue(null, false));
	}


	private void test_getIntegerValue()
		throws Exception
	{
		Log.info("Test getIntegerValue()");

		Config c1 = new Config("gmsec-value-1=10", Gmsec.DataType.KEY_VALUE_DATA);
		Config c2 = new Config("gmsec-value-2=NAN", Gmsec.DataType.KEY_VALUE_DATA);

		check("Unexpected integer value", 10 == c1.getIntegerValue("gmsec-value-1"));

		try {
			c1.getIntegerValue(null);
			check("Expected getIntegerValue() to throw exception", false);
		}
		catch (Exception e) {
			check(e.getMessage(), true);
		}
		try {
			c2.getIntegerValue("gmsec-value-2");
			check("Expected getIntegerValue() to throw exception", false);
		}
		catch (Exception e) {
			check(e.getMessage(), true);
		}

		Log.info("Test getIntegerValue() w/ default value");

		check("Expected getIntegerValue() to return default", 20 == c1.getIntegerValue("gmsec-missing", 20));
		check("Expected getIntegerValue() to return default", 30 == c2.getIntegerValue("gmsec-value-2", 30));
		check("Expected getIntegerValue() to return default", 10 == c2.getIntegerValue("", 10));
		check("Expected getIntegerValue() to return default", 10 == c2.getIntegerValue(null, 10));
	}


	private void test_getDoubleValue()
		throws Exception
	{
		Log.info("Test getDoubleValue()");

		Config c1 = new Config("gmsec-value-1=10.15", Gmsec.DataType.KEY_VALUE_DATA);
		Config c2 = new Config("gmsec-value-2=NAN", Gmsec.DataType.KEY_VALUE_DATA);

		check("Unexpected double value", 10.15 == c1.getDoubleValue("gmsec-value-1"));

		try {
			c1.getDoubleValue(null);
			check("Expected getDoubleValue() to throw exception", false);
		}
		catch (Exception e) {
			check(e.getMessage(), true);
		}
		try {
			c2.getDoubleValue("gmsec-value-2");
			check("Expected getDoubleValue() to throw exception", false);
		}
		catch (Exception e) {
			check(e.getMessage(), true);
		}

		Log.info("Test getDoubleValue() w/ default value");

		check("Expected getDoubleValue() to return default", 20.22 == c1.getDoubleValue("gmsec-missing", 20.22));
		check("Expected getDoubleValue() to return default", 30.33 == c2.getDoubleValue("gmsec-value-2", 30.33));
		check("Expected getDoubleValue() to return default", 10.33 == c2.getDoubleValue("", 10.33));
		check("Expected getDoubleValue() to return default", 10.33 == c2.getDoubleValue(null, 10.33));
	}


	private void test_clear()
		throws Exception
	{
		Log.info("Test clear()");

		Config c1 = new Config("mw-id=bolt server=localhost", Gmsec.DataType.KEY_VALUE_DATA);

		c1.clear();

		check("Unexpected XML content", c1.toXML().equals("<CONFIG>\n</CONFIG>"));
	}


	private void test_merge()
		throws Exception
	{
		Log.info("Test merge()");

		Config c1 = new Config("mw-id=bolt", Gmsec.DataType.KEY_VALUE_DATA);
		Config c2 = new Config("mw-id=activemq39", Gmsec.DataType.KEY_VALUE_DATA);
		Config c3 = new Config("mw-id=mb server=localhost", Gmsec.DataType.KEY_VALUE_DATA);

		// Overwrite existing config entries
		c1.merge(c3, true);

		check("Unexpected mw-id value", c1.getValue("mw-id").equals("mb"));
		check("Unexpected server value", c1.getValue("server").equals("localhost"));

		// Do not overwrite existing entries
		c2.merge(c3, false);

		check("Unexpected mw-id value", c2.getValue("mw-id").equals("activemq39"));
		check("Unexpected server value", c2.getValue("server").equals("localhost"));
	}


	private void test_toXML()
		throws Exception
	{
		Log.info("Test toXML()");

		String xml = "<CONFIG>\n"
		             + "\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n"
		             + "\t<PARAMETER NAME=\"server\">localhost</PARAMETER>\n"
		             + "</CONFIG>";

		Config c1 = new Config("mw-id=bolt server=localhost", Gmsec.DataType.KEY_VALUE_DATA);

		check("Unexpected XML content", c1.toXML().equals(xml));
	}


	private void test_fromXML()
		throws Exception
	{
		Log.info("Test fromXML()");

		String xml = "<CONFIG>\n"
		             + "\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n"
		             + "\t<PARAMETER NAME=\"server\">localhost</PARAMETER>\n"
		             + "</CONFIG>";


		Config c1 = new Config();

		c1.fromXML(xml);

		check("Unexpected XML content", c1.toXML().equals(xml));
	}


	private void test_getFromFile()
	{
		Log.info("Test getFromFile()");

		// Nominal test
		try
		{
			Config config1 = Config.getFromFile( getDataFile("T002_Config_good.xml"), "Bolt" );
			check("Unexpected result", config1 != null);
			check("Unexpected mw-id value", config1.getValue("mw-id").equals("bolt"));
			check("Unexpected server value", config1.getValue("server").equals("localhost"));

			Config config2 = Config.getFromFile( getDataFile("T002_Config_good.xml"), "ActiveMQ" );
			check("Unexpected result", config2 != null);
			check("Unexpected mw-id value", config2.getValue("mw-id").equals("activemq39"));
			check("Unexpected server value", config2.getValue("server").equals("tcp://localhost:61616"));

			Config config3 = Config.getFromFile( getDataFile("T002_Config_good.xml"), null );
			check("Unexpected result", config3 != null);
			check("Unexpected mw-id value", config3.getValue("mw-id").equals("bolt"));
			check("Unexpected server value", config3.getValue("server").equals("localhost"));
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), false);
		}

		// Off-nominal tests
		try
		{
			Config.getFromFile( getDataFile("T002_Config_bad.xml"), "Bolt" );
			check("Unexpected result", false);
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), e.getMessage().contains("Invalid XML format -- parse error"));
		}

		try
		{
			Config.getFromFile( null, "Bolt" );
			check("Unexpected result", false);
		}
		catch (IllegalArgumentException | GmsecException e)
		{
			check(e.getMessage(), e.getMessage().contains("Configuration file name is null or contains an empty string"));
		}

		try
		{
			Config.getFromFile( "", "Bolt" );
			check("Unexpected result", false);
		}
		catch (IllegalArgumentException | GmsecException e)
		{
			check(e.getMessage(), e.getMessage().contains("Configuration file name is null or contains an empty string"));
		}
	}


	private void test_toJSON()
		throws Exception
	{
		Log.info("Test toJSON()");

		String json = "{\"CONFIG\":{\"PARAMETER\":[{\"NAME\":\"mw-id\",\"VALUE\":\"bolt\"},{\"NAME\":\"server\",\"VALUE\":\"localhost\"}]}}";

		Config c1 = new Config("mw-id=bolt server=localhost", Gmsec.DataType.KEY_VALUE_DATA);

		check("Unexpected JSON content", c1.toJSON().equals(json));
	}
}
