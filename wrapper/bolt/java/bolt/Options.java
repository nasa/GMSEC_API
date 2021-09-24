
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package bolt;

import java.util.logging.Level;
import java.util.logging.Logger;

public class Options
{
	static final Logger source = Log.createSource("Options");

	// REPORT_n is a parameter for performance diagnostics
	public static final int REPORT_n = 1000;

	public static final int DEFAULT_PORT = 9100;
	static final int CLIENTS_PER_MANAGER = 64;

	// default queue sizes
	static final int DEFAULT_PROCESSOR_QUEUE = 16;
	static final int DEFAULT_DISTRIBUTOR_QUEUE = 16;

	static final int DEFAULT_ = 10 * 1000;

	int port = DEFAULT_PORT;
	int inputBufferSize = 1000 * 1024;
	int outputBufferSize = 1000 * 1024;
	int maxMessageSize = 10 * 1024 * 1024;
	int inputBufferLimit = 20 * 1024 * 1024;
	int clientsPerManager = CLIENTS_PER_MANAGER;
	int selectionTimeout_ms = 2000;
	int backlogSize = 50;
	Level logLevel = Level.INFO;

	int inactivityInterval_ms = 8 * 1000;
	int inactivityTimeout_ms = 20 * 1000;

	// private boolean ipv6 = false;
	// private String encrypt = null;

	void initialize (Client.Options options) {
		options.inputBufferSize = inputBufferSize;
		options.outputBufferSize = outputBufferSize;
		options.maxMessageSize = maxMessageSize;

		options.inactivityTimeout_ms = inactivityTimeout_ms;
	}

	boolean usage (String message) {
		Log.warning(source, message);
		Log.monitor(source, "\nUsage: java -jar bolt.jar [logging option] [key=value] ...\n"
				+ "\nLogging options:\n"
				+ "\n\t--SEVERE, --WARNING, --INFO, --FINE, --FINER, --FINEST, --OFF\n"
				+ "\nKnown key/values are:"
				+ "\n\tport=<integer>"
				+ "\n\tmaxMessageSize=<integer>"
				+ "\n\tinputBufferSize=<integer>"
				+ "\n\toutputBufferSize=<integer>"
				+ "\n\tinputBufferLimit=<integer>"
				+ "\n\tclientsPerManager=<integer>"
				+ "\n\tselectionTimeout_ms=<integer>"
				+ "\n\tinactivityInterval_ms=<integer>"
				+ "\n\tinactivityTimeout_ms=<integer>"
				+ "\n\tbacklogSize=<integer>"
				);
		return false;
	}

	boolean parse (String[] args) {
		for (String arg : args) {
			if (arg.startsWith("--")) {
				logLevel = Level.parse(arg.substring(2).toUpperCase());
			}
			else {
				int p = arg.indexOf('=');
				if (p == -1)
					return usage("invalid argument " + arg);

				String key = arg.substring(0, p);
				String value = arg.substring(p + 1);

				if (key.equalsIgnoreCase("port"))
					port = Integer.parseInt(value);
				else if (key.equalsIgnoreCase("maxMessageSize"))
					maxMessageSize = Integer.parseInt(value);
				else if (key.equalsIgnoreCase("inputBufferSize"))
					inputBufferSize = Integer.parseInt(value);
				else if (key.equalsIgnoreCase("outputBufferSize"))
					outputBufferSize = Integer.parseInt(value);
				else if (key.equalsIgnoreCase("clientsPerManager"))
					clientsPerManager = Integer.parseInt(value);
				else if (key.equalsIgnoreCase("inputBufferLimit"))
					inputBufferLimit = Integer.parseInt(value);
				else if (key.equalsIgnoreCase("inactivityInterval_ms"))
					inactivityInterval_ms = Integer.parseInt(value);
				else if (key.equalsIgnoreCase("inactivityTimeout_ms"))
					inactivityTimeout_ms = Integer.parseInt(value);
				else if (key.equalsIgnoreCase("selectionTimeout_ms"))
					selectionTimeout_ms = Integer.parseInt(value);
				else if (key.equalsIgnoreCase("backlogSize"))
					backlogSize = Integer.parseInt(value);
				else
					return usage("unknown option " + arg);
			}
		}

		return true;
	}
}
