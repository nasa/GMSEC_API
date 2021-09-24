
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






 
/* 
 * @file gmsub_gmd.java
 *
 * A simple Java example of SmartSockets GMD subscribing/publishing.
 *
 * For more information about SmartSockets GMD please refer to the
 * SmartSockets User's Guide.
*/    

import gov.nasa.gsfc.gmsecapi.Config;
import gov.nasa.gsfc.gmsecapi.Connection;
import gov.nasa.gsfc.gmsecapi.ConnectionFactory;
import gov.nasa.gsfc.gmsecapi.ErrorCallback;
import gov.nasa.gsfc.gmsecapi.Field;
import gov.nasa.gsfc.gmsecapi.GMSEC_String;
import gov.nasa.gsfc.gmsecapi.GMSEC_U32;
import gov.nasa.gsfc.gmsecapi.Message;
import gov.nasa.gsfc.gmsecapi.Status;
import static gov.nasa.gsfc.gmsecapi.gmsecAPI.*;
import gov.nasa.gsfc.gmsecapi.util.*;


public class gmsub_gmd implements Example
{
  // Variable used for system dependent new line character
  static final String NEW_LINE = System.getProperty("line.separator");

  static final String DEFAULT_SUBJECT = "GMSEC.>";

  StatusChecker checker;
  Config config;
  Connection connection;
  Message message;

  gmsub_gmd(Config config) throws ExampleException
  {
    checker = new StatusChecker();
    this.config = config;
    Util.initialize(config);

    if (!Util.hasConnectionType(config) || !Util.hasServer(config)) {
      printUsage();
      throw new ExampleException("Invalid option");
    }
  }

  private void printUsage() {
    Log.Error("\nusage: java gmsub_gmd connectionType=<middleware> " +
        "server=<server name> " +
        "[ <parameter>=<value> ]\n" +
        "\n\tNote that the parameter 'connectiontype' " +
        "and 'server' are required."  +
        "\n\tThe rest of other parameters are optional.\n" +
        "\n\tsubject=<subject name>" +
        "\n\titerations=<iterations>" +
        "\n\tsubject.<N>=<sub-subject name>" +
        "\n\t\tNote: N must be 1 or greater" +
        "\n\t\tFor example, subject.1=GMSEC.A subject.2=GMSEC.B" +
        "\n\tmsg_timeout_ms=<timeout period (milliseconds)>" +
        "\n\tloglevel=<log level>" +
        "\n\tcfgfile=<config_filepath.xml>" +
        "\n\nFor more information, see API SmartSockets User's Guide\n\n");
  }

  public boolean run() throws ExampleException
  {
    // Not needed in production, but it allows you to see what
    // options actually get set in SS (as a sanity check)
    config.AddValue("COMMAND_FEEDBACK","always");

    // Add the gmdSubject to the connection configuration.  
    config.AddValue("gmdSubject","GMSEC.GMD_Subject_subscriber");

    // Set the Server mode, so it 'remembers' if you leave and come back.
    config.AddValue("SERVER_DISCONNECT_MODE","warm");

    // @TODO: do we need to add ITERATIONS beside SUBJECT and TIMEOUT_MS? 
    // determine what subjects to listen to
    java.util.List<String> subjects = new java.util.ArrayList<String>(10);
    for (int i = 0; i < 100; ++i) 
    {
      String tmp = Util.get(config, "SUBJECT." + i);
      if (tmp != null)
        subjects.add(tmp);
      else
        break;
    }

    if (subjects.size() == 0) 
    {
      String subject = Util.get(config, "SUBJECT", DEFAULT_SUBJECT);
      subjects.add(subject);
    }

    int iterations = Util.get(config, "ITERATIONS", 0);
    if (iterations > 0)
      Log.Info("waiting for up to " + iterations + " messages");

    int msg_timeout_ms = Util.get(config, "MSG_TIMEOUT_MS",
                                  Util.EX_MSG_TIMEOUT_MS);

    // Output GMSEC API version
    Log.Info(ConnectionFactory.GetAPIVersion());

    // Establish a connection
    connection = Util.openConnection(config);

    // Output middleware version
    Log.Info("Middleware version = " + connection.GetLibraryVersion());

    // Subscribe
    for (String subject : subjects) 
    {
      Log.Info(NEW_LINE + "subscribing to " + subject + NEW_LINE);
      checker.calling("Subscribe(" + subject + ")")
        .check(connection.Subscribe(subject));
    }


    Field field = new Field();
    GMSEC_U32 seq = new GMSEC_U32();

    Status status;

    // Wait for messages  
    int count = 0;
    boolean done = false;

    while (!done)
    {
      message = new Message();

      checker.calling("GetNextMsg(timeout=" + msg_timeout_ms + ")");
      status = connection.GetNextMsg(message, msg_timeout_ms);

      if (status.isError()) 
      {
        if (status.GetCode() == GMSEC_TIMEOUT_OCCURRED)
           Log.Info("timeout occurred");
        else
           checker.check(status); 
      }

      if (message.isValid())
      {
        ++count;
        if (iterations > 0 && count >= iterations)
          done = true;

        status = message.GetField("sequence number", field);
        if (!status.isError())
        {
          field.GetValueU32(seq);
          Log.Info("sequence number->" + seq + NEW_LINE);
        }

        GMSEC_String subject = new GMSEC_String();
        checker.calling("GetSubject").check(message.GetSubject(subject));
        if (subject.Get().equals("GMSEC.TERMINATE"))
          done = true;

        Message tmp = message;
        message = null;
        checker.calling("DestroyMessage").check(connection.DestroyMessage(tmp));
      }
    }

    return true;
  } 

  public boolean cleanup() throws ExampleException
  {
    if (message != null && message.isValid())
      Util.checkNoThrow("DestoryMessage", connection.DestroyMessage(message));

    if (connection != null)
      Util.closeConnection(connection);
 
    return true;
  }

  public static void main(String argv[])
  {
    Example example = null;

    try {
      Config config = new Config(argv);

      Util.addToConfigFromFile(config);

      example = new gmsub_gmd(config);
      example.run();
    }
    catch (ExampleException e) {
      e.printStackTrace();
    }
    finally {
      if (example != null)
        Util.cleanup(example);
    }

  }
}
