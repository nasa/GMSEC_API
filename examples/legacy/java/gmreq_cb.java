 
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






 
/** 
 * @file gmreq_cb.java
 *
 * An example requestor with calling back using the Java GMSEC API.
 */

import gov.nasa.gsfc.gmsecapi.Callback;
import gov.nasa.gsfc.gmsecapi.Config;
import gov.nasa.gsfc.gmsecapi.Connection;
import gov.nasa.gsfc.gmsecapi.ConnectionFactory;
import gov.nasa.gsfc.gmsecapi.Field;
import gov.nasa.gsfc.gmsecapi.GMSEC_String;
import gov.nasa.gsfc.gmsecapi.Message;
import gov.nasa.gsfc.gmsecapi.ReplyCallback;
import gov.nasa.gsfc.gmsecapi.Status;
import static gov.nasa.gsfc.gmsecapi.gmsecAPI.*;
import gov.nasa.gsfc.gmsecapi.util.*;
import java.io.*;

class MyReplyCallback implements ReplyCallback 
{
	public void OnReply(Connection conn, Message request, Message reply) 
	{
		// Dump Reply
		GMSEC_String replyMsg = new GMSEC_String("");
		reply.ToXML(replyMsg);
		Log.Info("OnReply:reply =>\n" + replyMsg.Get());

		gmreq_cb.gReplyCallbackFlag = 1;
		// Do not destroy the message here
	}

	public void OnError(Connection conn, Message msg, Status status) {
		// Dump Error
		Log.Warning("OnError:error =>\n" + status.Get());
	}
};


public class gmreq_cb implements Example
{
  public static int gReplyCallbackFlag = 0;
  static final String DEFAULT_SUBJECT = "GMSEC.MISSION.SAT_ID.REQ.CMD.GMRPL";

  StatusChecker checker;
  Config config;
  Connection connection;
  Message request;

  gmreq_cb(Config config) throws ExampleException
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
    Log.Error("\nusage: java gmreq_cb connectionType=<middleware> " +
        "[ <parameter>=<value> ]\n" +
        "\n\tNote that the parameter 'connectiontype' is required. "  +
        "\n\tThe rest of other parameters are optional.\n" +
        "\n\tserver=<server name> " +
        "(required if middleware is not bolt/MB locally)" +
        "\n\tsubject=<subject name>" +
        "\n\tmsg_timeout_ms=<timeout period (milliseconds)>" +
        "\n\tloglevel=<log level>" +
        "\n\tcfgfile=<config_filepath.xml>" +
        "\n\nFor more information, see API User's Guide\n\n");
  }


  public boolean run() throws ExampleException
  {
    // Output GMSEC API version
    Log.Info(ConnectionFactory.GetAPIVersion());

    String subject = Util.get(config, "SUBJECT", DEFAULT_SUBJECT);
    int msg_timeout_ms = Util.get(config, "MSG_TIMEOUT_MS",
                                   Util.EX_MSG_TIMEOUT_MS);

    connection = Util.openConnection(config);
 
    // Output middleware version
    Log.Info("Middleware version = " + connection.GetLibraryVersion());

    // Request - Reply
    request = new Message();
    checker.calling("CreateMessage")
      .check(connection.CreateMessage(subject, GMSEC_MSG_REQUEST, request));

    Field field = new Field();

    // Add fields
    checker.calling("SetFieldValuesSTRING")
      .check(field.SetFieldValueSTRING("QUESTION",
             "Does the request/reply functionality still work?"));

    checker.calling("AddField(QUESTION)").check(request.AddField(field));

    // Add the name of the component making the Request.
    field.SetFieldValueSTRING("COMPONENT", "GMREQ");
    checker.calling("AddField(COMPONENT)").check(request.AddField(field));

    // Add the Message ID of this Request.  In theory the MSG-ID should be unique,
    // but here it is not for the sake of simplicity.  See API Interface Specification
    // for suggetions on creating a MSG-ID.
    field.SetFieldValueSTRING("MSG-ID", "GMREQ-MSG");
    checker.calling("AddField(MSG-ID)").check(request.AddField(field));

    // Dump Reply
    GMSEC_String requestMsg = new GMSEC_String();
    checker.calling("Request.ToXML").check(request.ToXML(requestMsg));
    Log.Info("built request =>\n" + requestMsg.Get());

    Log.Info("StartAutoDispatch");
    checker.calling("StartAutoDispatch").check(connection.StartAutoDispatch());

    Log.Info("Connection.Request");
    checker.calling("Connection.Request")
      .check(connection.Request(request, msg_timeout_ms, 
                                new MyReplyCallback()));

    int deltaTime = 0;
    Log.Info("Waiting for response...");
    while (gReplyCallbackFlag == 0)
    {
      Util.millisleep(100);
      if(msg_timeout_ms != GMSEC_WAIT_FOREVER)
      {
		deltaTime += 100;
		if(deltaTime >= msg_timeout_ms)
		{
			break;
		}
      }
    }

    if (gReplyCallbackFlag == 1)
    {
      Log.Info("Response Received!");
    }
    else
    {
      Log.Info("No response received");
    }
    Log.Info("Stoppping AD");
	connection.StopAutoDispatch();
    return true;
  }

  public boolean cleanup() throws ExampleException
  {
    //  Destroy the Request
    if (request != null && request.isValid()) {
      Status status = connection.DestroyMessage(request);
      Util.checkNoThrow("DestroyMessage:request", status);
    }

    if (connection != null)
      Util.closeConnection(connection);

    return true;
  }

  public static void main(String args[]) throws IOException
  {
    Example example = null;
    try {
      Config config = new Config(args);

      Util.addToConfigFromFile(config);

      example = new gmreq_cb(config);
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
