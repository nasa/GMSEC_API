/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






 
/** 
 * @file gmsub_cb.java
 *
 * A simple example Java subscriber using callback.
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



class PublishCallback implements Callback {
  private String m_handlerName;

  public PublishCallback(String handlerName) {
    m_handlerName = new String(handlerName);
  }

  public void OnMessage(Connection conn, Message msg) {
    GMSEC_String prtMsg = new GMSEC_String();
    msg.ToXML(prtMsg);
    Log.Info("******************* CALLBACK ********************");
    Log.Info("  Handler is " + m_handlerName);
    Log.Info(prtMsg.Get());
    // Do not destroy the message here
  }
}

public class gmsub_cb implements Example 
{
  static final String DEFAULT_SUBJECT = "GMSEC.>";

  StatusChecker checker;
  Config config;
  Connection connection;
  Message message;


  gmsub_cb(Config config) throws ExampleException {
    checker = new StatusChecker();
    this.config = config;
    Util.initialize(config);

    if (!Util.hasConnectionType(config)) {
      printUsage();
      throw new ExampleException("Invalid option");
    }
  }

  private void printUsage() {
    Log.Error("\nusage: java gmsub_cb connectionType=<middleware> " +
        "[ <parameter>=<value> ]\n" +
        "\n\tNote that the parameter 'connectiontype' is required. "  +
        "\n\tThe rest of other parameters are optional.\n" +
        "\n\tserver=<server name> " +
        "(required if middleware is not bolt/MB locally)" +
        "\n\tsubject=<subject name>" +
        "\n\titerations=<iterations>" +
        "\n\tsubject.<N>=<sub-subject name>" +
        "\n\t\tNote: N must be 1 or greater" +
        "\n\t\tFor example, subject.1=GMSEC.A subject.2=GMSEC.B" +
        "\n\tmsg_timeout_ms=<timeout period (milliseconds)>" +
		"\n\tprog_timeout_s=<timeout period (seconds)>" +
		"\n\t\tNote: msg_timeout_ms must be specified for prog_timeout_s" +
        "\n\tloglevel=<log level>" +
        "\n\tcfgfile=<config_filepath.xml>" +
        "\n\nFor more information, see API User's Guide\n\n");
  }

  public boolean run() throws ExampleException {

    Status status;
 
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
      String subject = Util.get(config, "SUBJECT", DEFAULT_SUBJECT);
      subjects.add(subject);
    }
    else {
      subjects.add("GMSEC.TERMINATE");
    }

    int iterations = Util.get(config, "ITERATIONS", 0);
    if (iterations > 0)
      Log.Info("waiting for up to " + iterations + " messages");

    int msg_timeout_ms = Util.get(config, "MSG_TIMEOUT_MS",
									Util.EX_MSG_TIMEOUT_MS);
	int prog_timeout_s = Util.get(config, "PROG_TIMEOUT_S",
									Util.EX_PROG_TIMEOUT_S);
    if(msg_timeout_ms == GMSEC_WAIT_FOREVER && 
									prog_timeout_s != GMSEC_WAIT_FOREVER)
    {
        Log.Error("Usage: To use prog_timeout_s, msg_timeout_ms must be defined!");
        return false;
    }

	// Output GMSEC API version
    Log.Info(ConnectionFactory.GetAPIVersion());
	
    // Establish a connection
    connection = Util.openConnection(config);

    // Output middleware version
    Log.Info("Middleware version = " + connection.GetLibraryVersion());

    // Subscribe
    for (String subject : subjects)
    {
      Log.Info("subscribing to " + subject);
      checker.calling("Subcribe(" + subject + ")")
        .check(connection.Subscribe(subject, new PublishCallback(subject)));
    }

    //  Listen
    int count = 0;
    boolean done = false;
    long prevTime = System.currentTimeMillis();
	long nextTime = 0;
	int elapsedTime = 0;	//ms

    while (!done) 
    {
    
	  if(elapsedTime >= (prog_timeout_s * 1000)
							&& prog_timeout_s != GMSEC_WAIT_FOREVER)
	  {
		  done = true;
	      Log.Info("Program Timeout Reached");
		  continue;
	  }
    
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
      
      nextTime = System.currentTimeMillis();
	  elapsedTime += (int) (nextTime - prevTime);
	  prevTime = nextTime;

      if (message.isValid())
      {
        ++count;
        if (iterations > 0 && count >= iterations)
          done = true;

        // Dispatch Callbacks
        checker.calling("DispatchMsg(" + message + ")")
          .check(connection.DispatchMsg(message));
        
        GMSEC_String subject = new GMSEC_String();
        checker.calling("GetSubject").check(message.GetSubject(subject));
        if (subject.Get().equals("GMSEC.TERMINATE"))
          done = true;

        Message tmp = message;
        message = null;
        checker.calling("DestroyMessage")
          .check(connection.DestroyMessage(tmp));
      }
    }

    return true;
  }


  public boolean cleanup() throws ExampleException {
    if (message != null && message.isValid()) {
      Status status = connection.DestroyMessage(message);
      Util.checkNoThrow("DestroyMessage", status);
    }

    if (connection != null)
      Util.closeConnection(connection);

    return true;
  }


  public static void main(String args[]) {

    Example example = null;

    try {
      Config config = new Config(args);

      Util.addToConfigFromFile(config);

      example = new gmsub_cb(config);
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
