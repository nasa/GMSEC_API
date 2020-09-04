import gov.nasa.gsfc.gmsecapi.Config;
import gov.nasa.gsfc.gmsecapi.Connection;
import gov.nasa.gsfc.gmsecapi.ConnectionFactory;
import gov.nasa.gsfc.gmsecapi.GMSEC_String;
import gov.nasa.gsfc.gmsecapi.Status;
import gov.nasa.gsfc.gmsecapi.util.*;
import static gov.nasa.gsfc.gmsecapi.gmsecAPI.GMSEC_WAIT_FOREVER;

import java.io.IOException;
import java.io.File;
import java.io.BufferedReader;
import java.io.FileReader;


public class Util
{

  public final static int EX_MSG_TIMEOUT_MS = GMSEC_WAIT_FOREVER;
  public final static int EX_PROG_TIMEOUT_S = GMSEC_WAIT_FOREVER;

	static boolean check(String tag, Status status) throws ExampleException {

		if (!checkNoThrow(tag, status))
			throw new ExampleException(tag + ": " + status.Get());

		return true;
	}

	static boolean checkNoThrow(String tag, Status status) {
		if (!status.isError())
			return true;

		Log.Warning(tag + ": " + status.Get());

		return false;
	}

	static boolean initialize(Config config) {

		String loglevel = get(config, "LOGLEVEL", "INFO");
		Log.SetReportingLevel(Log.LevelFromString(loglevel));

		Log.RegisterHandler(new LogHandler() {
				public void OnMessage(LogEntry entry) {
					System.out.println("\n" + TimeUtil.FormatTime_s(entry.time)
							+ ":" + Log.LevelToString(entry.level) 
							+ "\n" + entry.message);
				}
			});

		return true;
	}

	static boolean addToConfigFromFile(Config config)
	{
		String cfgfilename = get(config, "CFGFILE"); // This assumes, now, that there is
		                                             // only _one_ CFGFILE=filename.xml
		                                             // arg specified.

		if (cfgfilename != null)
		{
			File   f                  = new File(cfgfilename);

			String file_text_contents = new String();

			if (f.canRead())
			{
				try
				{
					BufferedReader br =
					  new BufferedReader(new FileReader(cfgfilename));

					String text_line;
					while ((text_line = br.readLine()) != null)
					{
						file_text_contents += text_line;
						Log.Debug(text_line);
					}

					Config config_from_file = new Config();

					config_from_file.FromXML(file_text_contents);

					GMSEC_String name  = new GMSEC_String();
					GMSEC_String value = new GMSEC_String();

					//
					// Read all the config file name, value pairs
					// and add them to the current base
					// configuration (which can have its
					// (name, value) pairs from both command-line
					// pairs and from the input current config 
					// file that was read).
					//
					Status status = config_from_file.GetFirst(name,  value);

					while (!status.IsError())
					{
						Log.Debug("Adding ("
						          + name + ", " + value
						          + ") from config file "
						          + cfgfilename);
						config.AddValue(name.toString(),
						                value.toString());

						status = config_from_file.GetNext(name, value);
					}
				}
				catch (IOException ioe)
				{
					Log.Warning("Could not successfully " +
					            "read " + cfgfilename +
					            ":  " + ioe.getMessage());
				}

			}
			else
			{
				Log.Warning("Non-valid-filepath config "
				            + "argument " + cfgfilename
				            + " seen; ignoring it.");
			}
		}// if there is a config file argument at all

		return true;

	}

	static boolean hasConnectionType(Config config) {
		if (get(config, "connectionType") == null && get(config, "mw-id") == null)
			return false;

		return true;
	}

	static boolean hasServer(Config config) {
		if (get(config, "server") == null)
			return false;

		return true;
	}

	static Connection openConnection(Config config) throws ExampleException {

		Connection connection = new Connection();

		// Create the connection
		check("Creating Connection",
				ConnectionFactory.Create(config, connection));

		// Connect
		check("Connecting", connection.Connect());

		return connection;
	}

	static void closeConnection(Connection connection) throws ExampleException {
		if (connection.IsConnected())
			check("Disconnecting", connection.Disconnect());
		check("Destroy Connection", ConnectionFactory.Destroy(connection));
	}


	static boolean cleanup(Example example) {
		boolean ok = false;
		try {
			ok = example.cleanup();
		}
		catch (ExampleException e) {
			e.printStackTrace();
		}
		return ok;
	}

	static String get(Config config, String key) {
		GMSEC_String work = new GMSEC_String();
		Status status = config.GetValue(key, work);
		if (!status.isError() && work.Get() != null)
			return work.Get();
		return null;
	}

	static String get(Config config, String key, String xdefault) {
		String value = get(config, key);
		if (value == null)
			value = xdefault;
		return value;
	}

	static Integer get(Config config, String key, Integer xdefault) {
		Integer value;
		String s = get(config, key);
		if (s != null)
			value = Integer.parseInt(s);
		else
			value = xdefault;
		return value;
	}

	static Double get(Config config, String key, Double xdefault) {
		Double value;
		String s = get(config, key);
		if (s != null)
			value = Double.parseDouble(s);
		else
			value = xdefault;
		return value;
	}

	static void millisleep(int millis) {
		try {
			Thread.sleep(millis);
		}
		catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
}
