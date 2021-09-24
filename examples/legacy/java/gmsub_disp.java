 
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






 
/** 
 * @file gmsub_disp.java
 *
 * A simple example Java GMSEC subscriber 
 *
 */

import gov.nasa.gsfc.gmsecapi.Callback;
import gov.nasa.gsfc.gmsecapi.Config;
import gov.nasa.gsfc.gmsecapi.Connection;
import gov.nasa.gsfc.gmsecapi.ConnectionFactory;
import gov.nasa.gsfc.gmsecapi.Field;
import gov.nasa.gsfc.gmsecapi.GMSEC_String;
import gov.nasa.gsfc.gmsecapi.Message;
import gov.nasa.gsfc.gmsecapi.Status;
import static gov.nasa.gsfc.gmsecapi.gmsecAPI.*;
import gov.nasa.gsfc.gmsecapi.util.*;


class PublishDispatcher implements Callback 
{
  public void OnMessage(Connection conn, Message message) 
  {
    // Dump Message
    GMSEC_String xml = new GMSEC_String();
    message.ToXML(xml);
    Log.Info("received \n" + xml.Get());
    // Do not destroy the message here
  }
}

public class gmsub_disp implements Example 
{

  StatusChecker checker;
  Config config;
  Connection connection;
  Message message;

  gmsub_disp(Config config) throws ExampleException
  {
    checker = new StatusChecker();
    this.config = config;
    Util.initialize(config);

    if (!Util.hasConnectionType(config)) {
      printUsage();
      throw new ExampleException("Invalid option");
    }
  }

  private void printUsage() {
    Log.Error("\nusage: java gmsub_disp connectionType=<middleware> " +
        "[ <parameter>=<value> ]\n" +
        "\n\tNote that the parameter 'connectiontype' is required. "  +
        "\n\tThe rest of other parameters are optional.\n" +
        "\n\tserver=<server name> " +
        "(required if middleware is not bolt/MB locally)" +
        "\n\tsubject=<subject name>" +
        "\n\tsubject.<N>=<sub-subject name>" +
        "\n\t\tNote: N must be 1 or greater" +
        "\n\t\tFor example, subject.1=GMSEC.A subject.2=GMSEC.B" +
        "\n\tloglevel=<log level>" +
        "\n\tcfgfile=<config_filepath.xml>" +
        "\n\nFor more information, see API User's Guide\n\n");
  }

  public boolean run() throws ExampleException
  {
    Status result = new Status();

    // Output GMSEC API version
    Log.Info(ConnectionFactory.GetAPIVersion());

    // determine what subjects to listen to
    java.util.List<String> subjects = new java.util.ArrayList<String>(10);

    String value = Util.get(config,"SUBJECT");
    if (value != null) {
      subjects.add(value);
    }
    else {
      for (int i = 1; ; ++i) {
        String tmp = Util.get(config, "SUBJECT." + i);
        if (tmp != null)
          subjects.add(tmp);
        else
          break;
      }
    }

    if (subjects.isEmpty()) {
      String subject = Util.get(config, "SUBJECT", "GMSEC.>");
      subjects.add(subject);
    }
    else {
      subjects.add("GMSEC.TERMINATE");
    }

    // Establish a connection
    connection = Util.openConnection(config);

    // Output middleware version
    Log.Info("Middleware version = " + connection.GetLibraryVersion());

    // Subscribe
    for (String subject : subjects)
    {
      Log.Info("subscribing to '" + subject + "' with PublishDispatcher");
      checker.calling("Subscribe(" + subject + ") with new PublishDispatcher")
        .check(connection.Subscribe(subject, new PublishDispatcher()));
    }

    Log.Info("Starting AutoDispatch");
    checker.calling("StartAutoDispatch").check(connection.StartAutoDispatch());

    Log.Info("Press the enter key to exit");

    try {
      while (System.in.available() == 0)
      {
        Util.millisleep(100);
      } 
      
    }
    catch (Exception e) {
    }

    Log.Info("Stopping AutoDispatch");
    checker.calling("StopAutoDispatch").check(connection.StopAutoDispatch());

    return true;
  }

  public boolean cleanup() throws ExampleException 
  {
    if (message != null && message.isValid()) {
      Status status = connection.DestroyMessage(message);
      Util.checkNoThrow("DestroyMessage", status);
    }

    if (connection != null)
      Util.closeConnection(connection);

    return true;
  }

  public static void main(String args[]) 
  {
    Example example = null;
    try {
      Config config = new Config(args);

      Util.addToConfigFromFile(config);

      example = new gmsub_disp(config);
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
