 
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






 
/** 
 * @file logging.java
 *
 * A Java demonstration example of GMSEC logging.
 *
 */

import gov.nasa.gsfc.gmsecapi.ConnectionFactory;
import gov.nasa.gsfc.gmsecapi.Config;
import gov.nasa.gsfc.gmsecapi.Connection;
import gov.nasa.gsfc.gmsecapi.Status;
import gov.nasa.gsfc.gmsecapi.GMSEC_String;
import gov.nasa.gsfc.gmsecapi.gmsecAPI;
import gov.nasa.gsfc.gmsecapi.Field;
import gov.nasa.gsfc.gmsecapi.Message;
import gov.nasa.gsfc.gmsecapi.util.*;

class ErrorHandler implements LogHandler
{
  public void OnMessage(LogEntry entry)
  {
     System.out.println("On ERROR message:");
     System.out.println("  " + TimeUtil.FormatTime_s(entry.time) + ":" + 
                        Log.LevelToString(entry.level) + ":" + entry.message);
  }
}

class WarningHandler implements LogHandler
{
  public void OnMessage(LogEntry entry)
  {
    System.out.println("On WARNING message:");
    System.out.println("  " + TimeUtil.FormatTime_s(entry.time) + ":" + 
                       Log.LevelToString(entry.level) + ":" + entry.message);
  }
}

class InfoHandler implements LogHandler
{
  public void OnMessage(LogEntry entry)
  {
    System.out.println("On INFO message:");
    System.out.println("  " + TimeUtil.FormatTime_s(entry.time) + ":" + 
                       Log.LevelToString(entry.level) + ":" + entry.message);
  }
}

class VerboseHandler implements LogHandler
{
  public void OnMessage(LogEntry entry)
  {
    System.out.println("On VERBOSE message:");
    System.out.println("  " + TimeUtil.FormatTime_s(entry.time) + ":" + 
                       Log.LevelToString(entry.level) + ":" + entry.message);
  }
}

class DebugHandler implements LogHandler
{
  public void OnMessage(LogEntry entry)
  {
    System.out.println("On DEBUG message:");
    System.out.println("  " + TimeUtil.FormatTime_s(entry.time) + ":" + 
                       Log.LevelToString(entry.level) + ":" + entry.message);
  }
}

class AnyHandler implements LogHandler
{
  public void OnMessage(LogEntry entry)
  {
    System.out.println("On ANY message:");
    System.out.println("  " + TimeUtil.FormatTime_s(entry.time) + ":" + 
                       Log.LevelToString(entry.level) + ":" + entry.message);
  }
}

public class logging {

  public static void logging_printUsage() {
    Log.Error("\n\nusage: java logging connectionType=<middleware> " +
        "[ <parameter>=<value> ]\n" +
        "\n\tNote that the parameter 'connectiontype' is required. "  +
        "\n\tThe rest of other parameters are optional.\n" +
        "\n\tserver=<server name> " +
        "(required if middleware is not bolt/MB locally)" +
        "\n\tsubject=<subject name>" +
        "\n\tloglevel=<log level>" +
        "\n\tcfgfile=<config_filepath.xml>" +
        "\n\nFor more information, see API User's Guide\n\n");
  }

  public static void main(String args[]) {

    ErrorHandler errorHandler = new ErrorHandler();
    WarningHandler warningHandler = new WarningHandler();    
    InfoHandler infoHandler = new InfoHandler();
    VerboseHandler verboseHandler = new VerboseHandler();    
    DebugHandler debugHandler = new DebugHandler();    
    AnyHandler anyHandler = new AnyHandler();    

    Log.RegisterHandler(LogLevel.ERROR, errorHandler);    
    Log.RegisterHandler(LogLevel.WARNING, warningHandler);    
    Log.RegisterHandler(LogLevel.INFO, infoHandler);    
    Log.RegisterHandler(LogLevel.VERBOSE, verboseHandler);    
    Log.RegisterHandler(LogLevel.DEBUG, debugHandler);    

    Example publisher = null;
    try {
      Config cfg = new Config(args);

      Util.addToConfigFromFile(cfg);
 
      if (!Util.hasConnectionType(cfg)) {
        logging_printUsage();
        throw new ExampleException("Invalid option");
      }
      publisher = new gmpub(cfg);
      publisher.run();
    }
    catch (ExampleException e) {
      e.printStackTrace();
    }
    finally {
      if (publisher != null)
        Util.cleanup(publisher);
    }

    System.out.println("Setting log level to " + Log.LevelFromString("VERBOSE"));
    Log.SetReportingLevel(LogLevel.VERBOSE);

    Log.Info("The reporting level is set to " + Log.GetReportingLevel());

    Log.Error("This is an example error message.");
    Log.Warning("This is an example warning message.");
    Log.Verbose("This is an example \"verbose\" message.");
    Log.Debug("This is an example debug message which should not show.");
    // This last message cannot be shown right now because
    // Log.SetReportingLevel(LogLevel.VERBOSE), used above, does not
    // allow DEBUG messages to come out.
    Log.Verbose("This is another example \"verbose\" message.");

    /* Set logging reporting level to now allow DEBUG messages to be 
       shown */
    Log.SetReportingLevel(LogLevel.DEBUG);
    if (Log.GetReportingLevel() == LogLevel.DEBUG)
    {
        Log.Info("Changed reporting level to LogLevel.DEBUG");
    }
    else
    {
        Log.Error("Failed to change reporting level to LogLevel.DEBUG");
    }
    Log.Debug("This is an example debug message which should show.");

    Log.Debug("NONE reporting level, numerically, is " +
              Log.LevelFromString("NONE"));
    Log.Debug("ERROR reporting level, numerically, is " +
              Log.LevelFromString("ERROR"));
    Log.Debug("SECURE reporting level, numerically, is " +
              Log.LevelFromString("SECURE"));
    Log.Debug("WARNING reporting level, numerically, is " +
              Log.LevelFromString("WARNING"));
    Log.Debug("INFO reporting level, numerically, is " +
              Log.LevelFromString("INFO"));
    Log.Debug("VERBOSE reporting level, numerically, is " +
              Log.LevelFromString("VERBOSE"));
    Log.Debug("DEBUG reporting level, numerically, is " +
              Log.LevelFromString("DEBUG"));

    /* Register general-purpose handler and test */

    Log.RegisterHandler(anyHandler);

    Log.Error("NONE reporting level, numerically, is " +
             Log.LevelFromString("NONE"));
    Log.Error("ERROR reporting level, numerically, is " +
              Log.LevelFromString("ERROR"));
    Log.Warning("WARNING reporting level, numerically, is " +
                Log.LevelFromString("WARNING"));
    Log.Info("INFO reporting level, numerically, is " +
             Log.LevelFromString("INFO"));
    Log.Verbose("VERBOSE reporting level, numerically, is " +
                Log.LevelFromString("VERBOSE"));
    Log.Debug("DEBUG reporting level, numerically, is " +
              Log.LevelFromString("DEBUG"));

  } //..main()

} //..class logging
