namespace T002
{
	using GMSEC.API5;
	using GMSEC.API5.Testing.Common;
	using System;
	using System.Text;


	class T002_Config : TestCase
	{
		static void Main(string[] args)
		{
			T002_Config test = new T002_Config();
			test.SetDescription("Test Config");
			test.Run(args);
		}


		public override void Run()
		{
			try
			{
				Test_Constructor_1();
				Test_Constructor_2();
				Test_Constructor_3();
				Test_CopyConstructor();
				Test_AddValue();
				Test_ClearValue();
				Test_GetValue();
				Test_GetBooleanValue();
				Test_GetIntegerValue();
				Test_GetDoubleValue();
				Test_Clear();
				Test_Merge();
				Test_ToXML();
				Test_FromXML();
				Test_GetFromFile();
				Test_ToJSON();
			}
			catch (GmsecException e)
			{
				Require(e.ToString(), false);
			}
		}

		private void Test_Constructor_1()
		{
			Log.Info("Test constructor (no-arg)");

			Config c1 = new Config();

			Check("Unexpected XML content", c1.ToXML() == "<CONFIG>\n</CONFIG>");
		}


		private void Test_Constructor_2()
		{
			Log.Info("Test constructor (command line args)");

			string[] args = new string[2];
			args[0] = "mw-id=bolt";
			args[1] = "server=localhost";

			Config c1 = new Config(args);

			Check("Unexpected XML content", c1.ToXML() == "<CONFIG>\n\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n\t<PARAMETER NAME=\"server\">localhost</PARAMETER>\n</CONFIG>");
		}


		private void Test_Constructor_3()
		{
			Log.Info("Test constructor (data args)");

			// Test: data constructor

			// Using string with options
			Config c1 = new Config("mw-id=bolt server=localhost", DataType.KEY_VALUE_DATA);
			Check("Unexpected XML content", c1.ToXML() == "<CONFIG>\n\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n\t<PARAMETER NAME=\"server\">localhost</PARAMETER>\n</CONFIG>");

			// Using XML
			Config c2 = new Config(c1.ToXML(), DataType.XML_DATA);
			Check("Unexpected XML content", c2.ToXML() == "<CONFIG>\n\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n\t<PARAMETER NAME=\"server\">localhost</PARAMETER>\n</CONFIG>");

			// Using JSON
			Config c3 = new Config(c1.ToJSON(), DataType.JSON_DATA);
			Check("Unexpected XML content", c3.ToXML() == "<CONFIG>\n\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n\t<PARAMETER NAME=\"server\">localhost</PARAMETER>\n</CONFIG>");

			// Using lowercase XML
			string lowerXML = c1.ToXML().ToLower();

			Config c4 = new Config(lowerXML, DataType.XML_DATA);
			Check("Unexpected XML content", c4.ToXML() == "<CONFIG>\n\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n\t<PARAMETER NAME=\"server\">localhost</PARAMETER>\n</CONFIG>");

			// Using lowercase JSON
			string lowerJSON = c1.ToJSON().ToLower();

			Config c5 = new Config(lowerJSON, DataType.JSON_DATA);
			Check("Unexpected XML content", c5.ToXML()== "<CONFIG>\n\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n\t<PARAMETER NAME=\"server\">localhost</PARAMETER>\n</CONFIG>");
		}


		private void Test_CopyConstructor()
		{
			Log.Info("Test copy-constructor");

			Config c1 = new Config("mw-id=bolt server=localhost", DataType.KEY_VALUE_DATA);
			Config c2 = new Config(c1);

			Check("Config is not the same as the original", c1.ToXML() == c2.ToXML());
		}


		private void Test_AddValue()
		{
			Log.Info("Test AddValue()");

			Config c1 = new Config();

			Check("Did not expect to find value", c1.GetValue("mw-id") == null);

			c1.AddValue("mw-id", "bolt");

			Check("Expected to find value", c1.GetValue("mw-id") == "bolt");
	}


		private void Test_ClearValue()
		{
			Log.Info("Test ClearValue()");

			Config c1 = new Config("mw-id=bolt", DataType.KEY_VALUE_DATA);

			Check("Expected to find value", c1.GetValue("mw-id") == "bolt");

			c1.ClearValue("mw-id");

			Check("Did not expect to find value", c1.GetValue("mw-id") == null);
		}


		private void Test_GetValue()
		{
			Log.Info("Test GetValue()");

			Config c1 = new Config("mw-id=bolt", DataType.KEY_VALUE_DATA);

			Check("Unexpected value", c1.GetValue("mw-id") == "bolt");
			Check("Unexpected value", c1.GetValue("server") == null);

			Log.Info("Test GetValue() w/ default value");

			Check("Unexpected default value", c1.GetValue("connectionType", "mb") == "mb");
			Check("Unexpected default value", c1.GetValue(null, "mb") == "mb");
		}


		private void Test_GetBooleanValue()
		{
			Log.Info("Test GetBooleanValue()");

			Config c1 = new Config("gmsec-rocks=true", DataType.KEY_VALUE_DATA);
			Config c2 = new Config("gmsec-rocks=sure", DataType.KEY_VALUE_DATA);

			Check("Unexpected boolean value", true == c1.GetBooleanValue("gmsec-rocks"));

			try {
				c1.GetBooleanValue(null);
				Check("Expected GetBooleanValue() to throw exception", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), true);
			}
			try {
				c2.GetBooleanValue("gmsec-rocks");
				Check("Expected GetBooleanValue() to throw exception", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), true);
			}

			Log.Info("Test GetBooleanValue() w/ default value");

			Check("Expected GetBooleanValue() to return default", false == c1.GetBooleanValue("gmsec-missing", false));
			Check("Expected GetBooleanValue() to return default", true  == c1.GetBooleanValue("gmsec-missing", true));
			Check("Expected GetBooleanValue() to return default", true  == c2.GetBooleanValue("gmsec-rocks", true));
			Check("Expected GetBooleanValue() to return default", true  == c2.GetBooleanValue("", true));
			Check("Expected GetBooleanValue() to return default", false == c2.GetBooleanValue(null, false));
		}


		private void Test_GetIntegerValue()
		{
			Log.Info("Test GetIntegerValue()");

			Config c1 = new Config("gmsec-value-1=10", DataType.KEY_VALUE_DATA);
			Config c2 = new Config("gmsec-value-2=NAN", DataType.KEY_VALUE_DATA);

			Check("Unexpected integer value", 10 == c1.GetIntegerValue("gmsec-value-1"));

			try {
				c1.GetIntegerValue(null);
				Check("Expected GetIntegerValue() to throw exception", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), true);
			}
			try {
				c2.GetIntegerValue("gmsec-value-2");
				Check("Expected GetIntegerValue() to throw exception", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), true);
			}

			Log.Info("Test GetIntegerValue() w/ default value");

			Check("Expected GetIntegerValue() to return default", 20 == c1.GetIntegerValue("gmsec-missing", 20));
			Check("Expected GetIntegerValue() to return default", 30 == c2.GetIntegerValue("gmsec-value-2", 30));
			Check("Expected GetIntegerValue() to return default", 10 == c2.GetIntegerValue("", 10));
			Check("Expected GetIntegerValue() to return default", 10 == c2.GetIntegerValue(null, 10));
		}


		private void Test_GetDoubleValue()
		{
			Log.Info("Test GetDoubleValue()");

			Config c1 = new Config("gmsec-value-1=10.15", DataType.KEY_VALUE_DATA);
			Config c2 = new Config("gmsec-value-2=NAN", DataType.KEY_VALUE_DATA);

			Check("Unexpected double value", 10.15 == c1.GetDoubleValue("gmsec-value-1"));

			try {
				c1.GetDoubleValue(null);
				Check("Expected GetDoubleValue() to throw exception", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), true);
			}
			try {
				c2.GetDoubleValue("gmsec-value-2");
				Check("Expected GetDoubleValue() to throw exception", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), true);
			}

			Log.Info("Test GetDoubleValue() w/ default value");

			Check("Expected GetDoubleValue() to return default", 20.22 == c1.GetDoubleValue("gmsec-missing", 20.22));
			Check("Expected GetDoubleValue() to return default", 30.33 == c2.GetDoubleValue("gmsec-value-2", 30.33));
			Check("Expected GetDoubleValue() to return default", 10.33 == c2.GetDoubleValue("", 10.33));
			Check("Expected GetDoubleValue() to return default", 10.33 == c2.GetDoubleValue(null, 10.33));
		}


		private void Test_Clear()
		{
			Log.Info("Test Clear()");

			Config c1 = new Config("mw-id=bolt server=localhost", DataType.KEY_VALUE_DATA);

			c1.Clear();

			Check("Unexpected XML content", c1.ToXML() == "<CONFIG>\n</CONFIG>");
		}


		private void Test_Merge()
		{
			Log.Info("Test Merge()");

			Config c1 = new Config("mw-id=bolt", DataType.KEY_VALUE_DATA);
			Config c2 = new Config("mw-id=activemq39", DataType.KEY_VALUE_DATA);
			Config c3 = new Config("mw-id=mb server=localhost", DataType.KEY_VALUE_DATA);

			// Overwrite existing config entries
			c1.Merge(c3, true);

			Check("Unexpected mw-id value", c1.GetValue("mw-id") == "mb");
			Check("Unexpected server value", c1.GetValue("server") == "localhost");

			// Do not overwrite existing entries
			c2.Merge(c3, false);

			Check("Unexpected mw-id value", c2.GetValue("mw-id") == "activemq39");
			Check("Unexpected server value", c2.GetValue("server") == "localhost");
		}


		private void Test_ToXML()
		{
			Log.Info("Test ToXML()");

			string xml = "<CONFIG>\n"
			             + "\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n"
			             + "\t<PARAMETER NAME=\"server\">localhost</PARAMETER>\n"
			             + "</CONFIG>";

			Config c1 = new Config("mw-id=bolt server=localhost", DataType.KEY_VALUE_DATA);

			Check("Unexpected XML content", c1.ToXML() == xml);
		}


		private void Test_FromXML()
		{
			Log.Info("Test FromXML()");

			string xml = "<CONFIG>\n"
		    	         + "\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n"
		       	      + "\t<PARAMETER NAME=\"server\">localhost</PARAMETER>\n"
		       	      + "</CONFIG>";

			Config c1 = new Config();

			c1.FromXML(xml);

			Check("Unexpected XML content", c1.ToXML() == xml);
		}


		private void Test_GetFromFile()
		{
			Log.Info("Test GetFromFile()");

			// Nominal test
			try
			{
				Config config1 = Config.GetFromFile( GetDataFile("T002_Config_good.xml"), "Bolt" );
				Check("Unexpected return value", config1 != null);
				Check("Unexpected mw-id value", config1.GetValue("mw-id") == "bolt");
				Check("Unexpected server value", config1.GetValue("server") == "localhost");

				Config config2 = Config.GetFromFile( GetDataFile("T002_Config_good.xml"), "ActiveMQ" );
				Check("Unexpected return value", config2 != null);
				Check("Unexpected mw-id value", config2.GetValue("mw-id") == "activemq39");
				Check("Unexpected server value", config2.GetValue("server") == "tcp://localhost:61616");

				Config config3 = Config.GetFromFile( GetDataFile("T002_Config_good.xml"), null );
				Check("Unexpected return value", config3 != null);
				Check("Unexpected mw-id value", config3.GetValue("mw-id") == "bolt");
				Check("Unexpected server value", config3.GetValue("server") == "localhost");
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), false);
			}

			// Off-nominal tests
			try
			{
				Config.GetFromFile( GetDataFile("T002_Config_bad.xml"), "Bolt" );
				Check("Unexpected result", false);
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), e.ToString().Contains("Invalid XML format -- parse error"));
			}

			try
			{
				Config.GetFromFile( "file-does-not-exist", "Bolt" );
				Check("Unexpected result", false);
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), e.ToString().Contains("Configuration file could not be found or opened"));
			}

			try
			{
				Config.GetFromFile(null, "Bolt");
				Check("Unexpected result", false);
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), e.ToString().Contains("Configuration file name cannot be NULL, nor be an empty string"));
			}

			try
			{
				Config.GetFromFile("", "Bolt");
				Check("Unexpected result", false);
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), e.ToString().Contains("Configuration file name cannot be NULL, nor be an empty string"));
			}
		}


		private void Test_ToJSON()
		{
			Log.Info("Test ToJSON()");

			string json = "{\"CONFIG\":{\"PARAMETER\":[{\"NAME\":\"mw-id\",\"VALUE\":\"bolt\"},{\"NAME\":\"server\",\"VALUE\":\"localhost\"}]}}";

			Config c1 = new Config("mw-id=bolt server=localhost", DataType.KEY_VALUE_DATA);

			Check("Unexpected JSON content", c1.ToJSON() == json);
		}
	}
}
