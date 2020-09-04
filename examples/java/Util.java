import gov.nasa.gsfc.gmsec.api.Config;
import gov.nasa.gsfc.gmsec.api.Connection;
import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;
import gov.nasa.gsfc.gmsec.api.util.*;

import java.io.IOException;
import java.io.File;
import java.io.BufferedReader;
import java.io.FileReader;


public class Util
{
	static boolean initialize(Config config)
	{
		String loglevel = get(config, "LOGLEVEL", "INFO");

		Log.setReportingLevel(Log.levelFromString(loglevel));

		Log.registerHandler(new LogHandler() {
			public void onMessage(LogEntry entry)
			{
				String time = TimeUtil.formatTime(entry.time);

				StringBuilder sb = new StringBuilder();

				sb.append(time).append(" ");
				sb.append("[").append(Log.levelToString(entry.level)).append("] ");
				sb.append(": ").append(entry.message).append("\n");

				System.out.println(sb.toString());
			}
		});
		Log.debug("Registered custom LogHandler!");

		return true;
	}

	static boolean addToConfigFromFile(Config config)
	{
		String cfgfilename = get(config, "CFGFILE"); // This assumes, now, that there is
		                                             // only _one_ CFGFILE=filename.xml
		                                             // arg specified.

		if (cfgfilename != null)
		{
			File   file        = new File(cfgfilename);
			String xmlContents = new String();

			if (file.canRead())
			{
				try
				{
					BufferedReader br = new BufferedReader(new FileReader(cfgfilename));

					String line;
					while ((line = br.readLine()) != null)
					{
						xmlContents += line;
						Log.debug(line);
					}

					// Attempt to ingest the data from the XML configuration file
					try
					{
						Config configFromFile = new Config(xmlContents);

						java.util.Collection<java.util.Map.Entry<String, String>> set = configFromFile.entrySet();

						// Read all the config file name, value pairs
						// and add them to the current base
						// configuration (which can have its
						// (name, value) pairs from both command-line
						// pairs and from the input current config 
						// file that was read).
						//
						for (java.util.Map.Entry<String, String> entry : set)
						{
							String key   = entry.getKey();
							String value = entry.getValue();

							Log.debug("Adding (" + key + ", " + value + ") from config file " + cfgfilename);
							config.addValue(key, value);
						}
					}
					catch (GMSEC_Exception e)
					{
						Log.warning("Error parsing XML configuration file; " + e.toString());
						return false;
					}
				}
				catch (IOException ioe)
				{
					Log.warning("Could not successfully " + "read " + cfgfilename + ":  " + ioe.getMessage());
				}
			}
			else
			{
				Log.warning("Non-valid-filepath config argument " + cfgfilename + " seen; ignoring it.");
			}
		}

		return true;
	}


	static boolean hasConnectionType(Config config)
	{
		if (get(config, "connectionType") == null && get(config, "mw-id") == null)
			return false;

		return true;
	}


	static boolean hasServer(Config config)
	{
		if (get(config, "server") == null)
			return false;

		return true;
	}


	static Connection openConnection(Config config) throws GMSEC_Exception
	{
		// Create the connection
		Connection conn = Connection.create(config);

		// Connect
		conn.connect();

		return conn;
	}


	static void closeConnection(Connection conn)
	{
		conn.disconnect();

		Connection.destroy(conn);
	}


	static boolean cleanup(Example example)
	{
		boolean ok = false;
		try {
			ok = (example != null && example.cleanup());
		}
		catch (GMSEC_Exception e) {
			e.printStackTrace();
		}
		return ok;
	}


	static String get(Config config, String key)
	{
		String value = null;

		try {
			value = config.getValue(key);
		}
		catch (IllegalArgumentException e) {
			Log.warning(e.toString());
		}

		return value;
	}


	static String get(Config config, String key, String xdefault)
	{
		String value = get(config, key);
		if (value == null)
			value = xdefault;
		return value;
	}


	static Integer get(Config config, String key, Integer xdefault)
	{
		Integer value = xdefault;

		try {
			value = config.getIntegerValue(key);
		}
		catch (IllegalArgumentException e) {
			Log.debug(e.toString());
		}
		catch (GMSEC_Exception e) {
			Log.debug(e.toString());
		}

		return value;
	}


	static Double get(Config config, String key, Double xdefault)
	{
		Double value = xdefault;

		try {
			value = config.getDoubleValue(key);
		}
		catch (IllegalArgumentException e) {
			Log.debug(e.toString());
		}
		catch (GMSEC_Exception e) {
			Log.debug(e.toString());
		}

		return value;
	}


	static void millisleep(int millis)
	{
		try {
			Thread.sleep(millis);
		}
		catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
}
