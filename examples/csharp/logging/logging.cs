using GMSEC.API;
using System;
using System.Text;


class BaseHandler : LogHandler
{
	Mutex  m_mutex;
	string m_whoami;

	public BaseHandler(string whoami)
	{
		m_mutex  = new Mutex();
		m_whoami = whoami;
	}

	public override void OnMessage(LogEntry entry)
	{
		m_mutex.EnterMutex();

		string time = TimeUtil.FormatTime(entry.time);
		string file = System.IO.Path.GetFileName(entry.file);

		StringBuilder sb = new StringBuilder();

		sb.Append("[").Append(m_whoami).Append("]");
		sb.Append(" ").Append(time);
		sb.Append(" [").Append(Log.ToString(entry.level)).Append("]");
		sb.Append(" [").Append(file).Append(":").Append(entry.line).Append("] ");
		sb.Append(entry.message).Append("\n");

		Console.WriteLine(sb.ToString().Replace("\n", "\n\t"));

		m_mutex.LeaveMutex();
	}
}

class ErrorHandler : BaseHandler
{
	public ErrorHandler() : base("ErrorHandler")
	{
	}
}

class WarningHandler : BaseHandler
{
	public WarningHandler() : base("WarningHandler")
	{
	}
}

class InfoHandler : BaseHandler
{
	public InfoHandler() : base("InfoHandler")
	{
	}
}

class VerboseHandler : BaseHandler
{
	public VerboseHandler() : base("VerboseHandler")
	{
	}
}

class DebugHandler : BaseHandler
{
	public DebugHandler() : base("DebugHandler")
	{
	}
}

class GeneralHandler : BaseHandler
{
	public GeneralHandler() : base("GeneralHandler")
	{
	}
}

class logging
{
	static int Main(string[] args)
	{
		if (args.Length < 1)
		{
			Console.WriteLine("Error: Missing arg.\n");
			Console.WriteLine("Usage: ./logging mw-id=<middleware>");
			return -1;
		}

		Config config = new Config(args);

		//o Create and register custom log handlers
		ErrorHandler   eh = new ErrorHandler();
		WarningHandler wh = new WarningHandler();
		InfoHandler    ih = new InfoHandler();
		VerboseHandler vh = new VerboseHandler();
		DebugHandler   dh = new DebugHandler();
		GeneralHandler gh = new GeneralHandler();

		//  Note: Registration of VERBOSE and DEBUG custom log handlers will
		//        be ignored at the Core API level.
		//
		Log.Info("**** Registering all custom log handlers ****");
		Log.RegisterHandler(LogLevel.logERROR,   eh);
		Log.RegisterHandler(LogLevel.logWARNING, wh);
		Log.RegisterHandler(LogLevel.logINFO,    ih);
		Log.RegisterHandler(LogLevel.logVERBOSE, vh);
		Log.RegisterHandler(LogLevel.logDEBUG,   dh);

		//o Set log reporting level
		Log.SetReportingLevel(LogLevel.logINFO);
		Log.Info("The log reporting level is now set to: " + Log.GetReportingLevel());

		//o Display the GMSEC API version
		Log.Info("API version: " + ConnectionManager.GetAPIVersion());

		try
		{
			//o Create a ConnectionManager
			using (ConnectionManager connMgr = new ConnectionManager(config))
			{
				//o Connect to the GMSEC Bus
				connMgr.Initialize();

				//o Disconnect from the GMSEC Bus
				connMgr.Cleanup();
			}
		}
		catch (GmsecException e)
		{
			Log.Error("Exception: " + e.ToString());
			return -1;
		}

		//o Unregister all custom log handlers
		Log.Info("**** Un-registering all custom log handlers ****");
		Log.RegisterHandler(null);

		//o Examples of logging messages
		Log.Error("This is an ERROR message");
		Log.Warning("This is a WARNING message");
		Log.Info("This is an INFO message");
		Log.Verbose("This is a VERBOSE message");
		Log.Debug("This is a DEBUG message");      // This message will not be displayed

		//o Reset log reporting level so that all log message are displayed
		Log.SetReportingLevel(LogLevel.logDEBUG);

		//o Confirm logging level
		if (Log.GetReportingLevel() == LogLevel.logDEBUG)
		{
			Log.Info("The log reporting level is now set to: " + Log.GetReportingLevel());
		}
		else
		{
			Log.Error("The log reporting level was NOT changed to logDEBUG");
		}
		Log.Debug("A DEBUG message can now be displayed");

		//o Convert log level names to LogLevel enumerated values
		Log.Error("ERROR reporting level as enum value: " + Log.FromString("ERROR"));
		Log.Warning("WARNING reporting level as enum value: " + Log.FromString("WARNING"));
		Log.Info("INFO reporting level as enum value: " + Log.FromString("INFO"));
		Log.Verbose("VERBOSE reporting level as enum value: " + Log.FromString("VERBOSE"));
		Log.Debug("DEBUG reporting level as enum value: " + Log.FromString("DEBUG"));

		//o Reset log reporting level so that only INFO and below are displayed
		Log.SetReportingLevel(LogLevel.logINFO);

		//o Replace all custom log handlers with a general purpose custom log handler
		Log.Info("**** Registering all general purpose custom log handler ****");
		Log.RegisterHandler(gh);

		Log.Error("ERROR reporting level as string: " + Log.ToString(LogLevel.logERROR));
		Log.Warning("WARNING reporting level as string: " + Log.ToString(LogLevel.logWARNING));
		Log.Info("INFO reporting level as string: " + Log.ToString(LogLevel.logINFO));
		Log.Verbose("VERBOSE reporting level as string: " + Log.ToString(LogLevel.logVERBOSE));   // Not displayed
		Log.Debug("DEBUG reporting level as string: " + Log.ToString(LogLevel.logDEBUG));         // Not displayed

		//o Unregister (all) custom log handler(s)
		Log.Info("**** Un-registering custom log handler ****");
		Log.RegisterHandler(null);

		Log.Error("This is an ERROR message");
		Log.Warning("This is a WARNING message");
		Log.Info("This is an INFO message");
		Log.Verbose("This is a VERBOSE message");  // Not displayed
		Log.Debug("This is a DEBUG message");      // Not displayed

		return 0;
	}
}
