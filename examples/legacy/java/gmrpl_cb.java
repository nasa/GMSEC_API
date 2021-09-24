 
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






 
/** 
 * @file gmrpl_cb.java
 *
 * An example replier with callback using the Java GMSEC API.
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


class MyReqReplyCallback implements Callback, Example 
{
	StatusChecker checker = new StatusChecker();
	Connection connection;
	Message message;
	Message reply;

  public void OnMessage(Connection conn, Message msg) 
  {
    // Dump Request
    GMSEC_String requestMsg = new GMSEC_String();
    msg.ToXML(requestMsg);
    Log.Info(requestMsg.Get());

    if (this == null)
    {
      Log.Info("Empty Callback");
      return;
    } 

    // Initialization
    message = msg;
    connection = conn;

    try {
      run();
    }
    catch (ExampleException e) {
      e.printStackTrace();
    }
    finally {
      Util.cleanup(this);
    }

  }

  public boolean run() throws ExampleException
  {
    // Construct Reply subject.
    String component = getStringField("COMPONENT");
    if (component == null)
      throw new ExampleException("reply missing COMPONENT");

    String msg_id = getStringField("MSG-ID");
    if (msg_id == null)
      throw new ExampleException("reply missing MSG-ID");
 

    // Set Status Code.  
    // See API Interface Specification for available status codes.
    GMSEC_String status_code = new GMSEC_String("1");

    String replySubject = "GMSEC.MISSION.SAT_ID.RESP." + component + "." +
                          msg_id + "."+ status_code;

    // Create Reply
    reply = new Message();
    checker.calling("CreateMessage:reply")
      .check(connection.CreateMessage(replySubject, GMSEC_MSG_REPLY, reply));

    // Add Fields to replay
    Field field = new Field();
    field.SetFieldValueSTRING("ANSWER", "Sure looks like it!");
    checker.calling("AddField:ANSWER").check(reply.AddField(field));

    // Add the name of the component sending the Reply.
    field.SetFieldValueSTRING("COMPONENT", "GMRPL");
    checker.calling("AddField:COMPONENT").check(reply.AddField(field));

    // Dump Request
    GMSEC_String replyMsg = new GMSEC_String();
    checker.calling("reply.ToXML").check(reply.ToXML(replyMsg));
    Log.Info(replyMsg.Get());

    // Send Reply
    checker.calling("Connection.Reply").check(connection.Reply(message, reply));

    // Do not destroy the message here
    return true;
  }

  public boolean cleanup() throws ExampleException
  {
    // No need to disconnect for this callback 

    return false;
  }


  private String getStringField(String key)
  {
    String value = null;
    Field field = new Field();

    Status status = message.GetField(key, field);
    if (Util.checkNoThrow("GetField:" + key, status))
    { 
      GMSEC_String tmp = new GMSEC_String();
      status = field.GetValueSTRING(tmp);
      if (Util.checkNoThrow("Field.GetValuesSTRING:" + key, status))
        value = tmp.Get();
    }
 
    return value;
  } 
}


public class gmrpl_cb implements Example
{
  static final String DEFAULT_SUBJECT = "GMSEC.>";

  StatusChecker checker = new StatusChecker();
  Config config;
  Connection connection;
  Message message;
  Message reply;

  gmrpl_cb(Config config) throws ExampleException
  {
    this.config = config;
    Util.initialize(config);

    if (!Util.hasConnectionType(config)) {
      printUsage();
      throw new ExampleException("Invalid option");
    }
  } 

  private void printUsage() {
    Log.Error("\nusage: java gmrpl_cb connectionType=<middleware> " +
        "[ <parameter>=<value> ]\n" +
        "\n\tNote that the parameter 'connectiontype' is required. "  +
        "\n\tThe rest of other parameters are optional.\n" +
        "\n\tserver=<server name> " +
        "(required if middleware is not bolt/MB locally)" +
        "\n\tsubject=<subject name>" +
        "\n\tmsg_timeout_ms=<timeout period (milliseconds)>" +
        "\n\tprog_timeout_s=<timeout period (seconds)>" +
		"\n\t\tNote: msg_timeout_ms must be specified for prog_timeout_s" +
        "\n\tloglevel=<log level>" +
        "\n\tcfgfile=<config_filepath.xml>" +
        "\n\nFor more information, see API User's Guide\n\n");
  }

  public boolean run() throws ExampleException
  {
    Status status; 
    String subject = Util.get(config, "SUBJECT", DEFAULT_SUBJECT);
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

    connection = Util.openConnection(config);

    // Output middleware version
    Log.Info("Middleware version = " + connection.GetLibraryVersion());

    // Subscribe
    Log.Info("Subscribing to " + subject);
    checker.calling("Subscribe")
     .check(connection.Subscribe(subject, new MyReqReplyCallback()));

    Log.Info("Subscribing to GMSEC.TERMINATE");
    checker.calling("Subscribe")
     .check(connection.Subscribe("GMSEC.TERMINATE", new MyReqReplyCallback()));


    // Listen
    
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
    
      Message msg = new Message();

      status = connection.GetNextMsg(msg, msg_timeout_ms);
      
      nextTime = System.currentTimeMillis();
	  elapsedTime += (int) (nextTime - prevTime);
	  prevTime = nextTime;
  
      if (status.isError()) 
      {
        if (status.GetCode() != GMSEC_TIMEOUT_OCCURRED)
        {
          Util.check("GetNextMsg", status);
        }

        // This will never print if the parameter is not provided (default 
        // msg_timeout_ms causes GetNextMsg to wait forever)
        Log.Info("timeout occurred");
      }
      else
      {
        GMSEC_String tempSubject = new GMSEC_String();
        checker.calling("GetSubject").check(msg.GetSubject(tempSubject));
        if (tempSubject.Get().equals("GMSEC.TERMINATE"))
          done = true;
 
       checker.calling("DispatchMsg").check(connection.DispatchMsg(msg));

        // Destroy Message
        Message tmp = msg;
        msg = null;
        checker.calling("DestroyMessage").check(connection.DestroyMessage(tmp));
      }

    }

    return true;

  }

  public boolean cleanup() throws ExampleException
  {
		if (connection != null)
			Util.closeConnection(connection);

    return false;
  }

  public static void main(String args[]) 
  {
    Example example = null;

    try {
      Config config = new Config(args);

      Util.addToConfigFromFile(config);

      example =  new gmrpl_cb(config);
      example.run();
    }
    catch (ExampleException e) {
      e.printStackTrace();
    }
    finally {
      if (example != null )
        Util.cleanup(example);
    }

  } //..main()
} //..class gmrpl_cb()
