 
/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






 
/** 
 * @file Replier.java
 *
 *  A simple examples of various GMSEC API functionality.
 *
 * Contained within this example:
 *
 * - Loading an XML configuration file
 * - Populating a Config object using the XML file
 * - Establish a connection
 * - Using the XML file to store and set various other program configurations
 * - Subscribing to specific subjects to receieve messages through a callback
 * - Creating a heartbeat message from an XML definition
 * - Starting the automatic message dispatching thread
 * - Publishing the heartbeat message using the Publish call
 * - Programatically altering a field in the heartbeat message
 * Creating a response message from an XML definition upon receiving a request
 * Programatically altering a field in the response message
 * Using the Reply call to send a reply message
 * - Creating a log message from an XML definition
 * - Programatically altering a field in the log message
 * - Publishing the log message using the Publish call
 * Cleanup
 * - Message memory cleanup
 * - Connection cleanup disconnectiong from the network
 *
 * Any item with a * next to it is specific to this file in the overall example.
 */


import gov.nasa.gsfc.gmsecapi.Callback;
import gov.nasa.gsfc.gmsecapi.Config;
import gov.nasa.gsfc.gmsecapi.ConfigFile;
import gov.nasa.gsfc.gmsecapi.Connection;
import gov.nasa.gsfc.gmsecapi.ConnectionFactory;
import gov.nasa.gsfc.gmsecapi.Field;
import gov.nasa.gsfc.gmsecapi.GMSEC_I16;
import gov.nasa.gsfc.gmsecapi.GMSEC_String;
import gov.nasa.gsfc.gmsecapi.GMSEC_UShort;
import gov.nasa.gsfc.gmsecapi.Message;
import gov.nasa.gsfc.gmsecapi.Status;
import static gov.nasa.gsfc.gmsecapi.gmsecAPI.*;
import gov.nasa.gsfc.gmsecapi.util.*;


class ReplyInnerExample implements Example
{
  static final String NEW_LINE = System.getProperty("line.separator");

  ConfigFile configFile;
  Connection connection;
  Message message;
  Message logMessage;
  Message replyMessage;
  StatusChecker statusChecker;

  ReplyInnerExample(ConfigFile configFile, Connection connection,
               Message message, StatusChecker statusChecker)
  {
    this.configFile = configFile;
    this.connection = connection;
    this.message = message;
    this.statusChecker = statusChecker;
  } 
  
  public boolean run() throws ExampleException
  {
    Field tempField = new Field();
    GMSEC_String tempString = new GMSEC_String();
    GMSEC_String stringHolder = new GMSEC_String();

    message.GetField("DIRECTIVE-KEYWORD", tempField);
    tempField.GetValueSTRING(tempString);

    // Copy and output the request number which has been placed in the 
    // DIRECTIVE-KEYWORD field
    stringHolder.Set(tempString.Get());
    Log.Info(NEW_LINE + NEW_LINE + 
             "Received Request (" + stringHolder + "): " + Message.GetTime());

    // Create a generic message container for the directive response message
    replyMessage = new Message();
    statusChecker.calling("connection.CreateMessage(replyMessage)")
      .check(connection.CreateMessage(replyMessage));

    // Find and load the config file directive response message definition
    statusChecker.calling("Callback.LookupMessage(DIRECTIVE-RESPONE)")
      .check(configFile.LookupMessage("DIRECTIVE-RESPONSE", replyMessage));

    // Replace the DATA field with the request number we are responding to
    tempField.SetFieldValueSTRING("DATA", stringHolder.Get());
    statusChecker.calling("replyMessage.AddField")
      .check(replyMessage.AddField(tempField)); 

    // Replace the TIME-COMPLETE field with the correct time
    tempField.SetName("TIME-COMPLETED");
    tempField.SetValueSTRING(Message.GetTime());
    statusChecker.calling("replyMessage.AddField for TIME-COMPLETE")
      .check(replyMessage.AddField(tempField)); 

    // Set the reply subject to the request message's subject
    GMSEC_String subject = new GMSEC_String();
    message.GetSubject(subject);
    replyMessage.SetSubject(subject.Get());

    // Send the reply
    statusChecker.calling("connection.Reply")
      .check(connection.Reply(message, replyMessage));

    Log.Info(NEW_LINE + "Reply Sent (" + stringHolder + "): " + 
             Message.GetTime());

    // Create a generic message container for the log message
    logMessage = new Message();
    statusChecker.calling("connection.CreateMessage(logMessage)")
      .check(connection.CreateMessage(logMessage));

    // Find and load the config file log message definition
    statusChecker.calling("configFile.LookupMessage(LOG-REP)")
      .check(configFile.LookupMessage("LOG-REP", logMessage));

    // Fill the log message time
    tempField.SetName("EVENT-TIME");
    tempField.SetValueSTRING(Message.GetTime());
    statusChecker.calling("logMessage.AddField for EVENT-TIME")
      .check(logMessage.AddField(tempField));

    // Fill the log message message text
    tempField.SetFieldValueSTRING("MSG-TEXT", stringHolder.Get());
    statusChecker.calling("logMessage.AddField for MSG-TEXT")
      .check(logMessage.AddField(tempField));

    // Publish the log message
    statusChecker.calling("connection.Publish(logMessage)")
      .check(connection.Publish(logMessage));

    return true;
  }

  public boolean cleanup() throws ExampleException
  {
    if (logMessage != null && logMessage.isValid())
    {
      Status status = connection.DestroyMessage(logMessage);
      Util.checkNoThrow("DestoryMessage(logMessage)", status);
    }

    if (replyMessage != null && replyMessage.isValid())
    {
      Status status = connection.DestroyMessage(replyMessage);
      Util.checkNoThrow("DestoryMessage(replyMessage)", status);
    }

    return true;
  }  
};


// Callback Class for Request-Reply
class RequestReplyCallback implements Callback
{
  static final String NEW_LINE = System.getProperty("line.separator");

  private ConfigFile configFile;
  private StatusChecker statusChecker;

  public RequestReplyCallback(ConfigFile cfilePass, StatusChecker checker)
  {
    configFile = cfilePass;
    statusChecker = checker;
  }

  public void OnMessage(Connection conn, Message msg)
  {
    Field tempField = new Field();
    GMSEC_UShort msgKind = new GMSEC_UShort();

    // We don't care about anything but request messages
    msg.GetKind(msgKind);
    if (msgKind.Get() != GMSEC_MSG_REQUEST)
      return;

    Example example = null;
    try {
     example = new ReplyInnerExample(configFile, conn, msg, statusChecker);
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
};


public class Replier implements Example
{
  static final String NEW_LINE = System.getProperty("line.separator");

  Config config;
  Config programConfig;
  ConfigFile configFile;
  Connection connection;
  Message heartbeatMessage;
  StatusChecker checker;
  
  Replier(Config config, ConfigFile configFile)
  {
    this.config = config; 
    this.configFile = configFile;
    programConfig = new Config();
    checker = new StatusChecker();
    Util.initialize(config);
  }

  public boolean run() throws ExampleException
  {
    Status status = new Status();

    // Config variables
    short heartbeatHolder = 0;
    short heartbeatCount = 0;
    short heartbeatCountdown = 0;
    short loopCountdown = 0;
    short updateRate = 0;

    // Establish a connection object off of the configuration
    connection = Util.openConnection(config); 
  
    // output GMSEC API and Middleware info, just for display purposes
    Log.Info(ConnectionFactory.GetAPIVersion());
    Log.Info(connection.GetLibraryVersion());

    // Additional Configuration ///

    // Look up additional program configutation information
    checker.calling("configFile.LookupConfig(program-config)")
      .check(configFile.LookupConfig("program-config", programConfig));

    // Set program config values
    GMSEC_String configValue = new GMSEC_String();
    checker.calling("programConfig.GetValue(update-rate)")
      .check(programConfig.GetValue("update-rate", configValue));

    updateRate = Short.parseShort(configValue.Get());

    checker.calling("programConfig.GetValue(loop-time)")
      .check(programConfig.GetValue("loop-time", configValue));
  
    loopCountdown = Short.parseShort(configValue.Get());

    GMSEC_String tempSubject = new GMSEC_String();

    // Create subscriptions from subscription templates in the config file
    checker.calling("configFile.LookupSubscription(DIRECTIVE-REQUEST)")
      .check(configFile.LookupSubscription("DIRECTIVE-REQUEST", tempSubject));

    // Subscribe
    checker.calling("connection.Subscribe(" + tempSubject.Get() + ")")
      .check(connection.Subscribe(tempSubject.Get(), 
             new RequestReplyCallback(configFile, checker)));

    // Create Heartbeat Message

    // Create a generic message container for the heartbeat message
    heartbeatMessage = new Message();
    checker.calling("CreateMessage(heartbeatMessage)")
      .check(connection.CreateMessage(heartbeatMessage));

    // Find and load the config file heartbeat message definition
    checker.calling("configFile.Lookup(C2CX-HEARTBEAT-REP)")
      .check(configFile.LookupMessage("C2CX-HEARTBEAT-REP", heartbeatMessage));

    // Obtain the publish rate field, by name, from the heartbeat message
    Field tempField = new Field();
    checker.calling("heartbeatMessage.GetField(PUB-RATE)")
      .check(heartbeatMessage.GetField("PUB-RATE", tempField));

    // Obtain the value from the field
    GMSEC_I16 tempShort = new GMSEC_I16();
    checker.calling("tempField.GetValueI16()")
      .check(tempField.GetValueI16(tempShort));
    
    heartbeatHolder = tempShort.Get();

    // Main Publishing Loop

    // Output some general program information
    Log.Info("Publishing for " + loopCountdown + " seconds.");
    Log.Info("Publishing Heartbeat Messages every " + heartbeatHolder + 
             " seconds.");

    // Call to create the thread that will automatically retrieve messages 
    // off the message queue
    Log.Info("start auto dispatch...");
    connection.StartAutoDispatch();

    // Start time
    Log.Info("Start Time: " + Message.GetTime() + NEW_LINE);
  
    // Publishing loop
    for (;loopCountdown > 0; loopCountdown--)
    {
      // When the countdown reaches 0, we publish a heartbeat message and
      //  reset the counter
      if (heartbeatCountdown < 1)
      {
        heartbeatCount++;

        // Update the message counter
        tempField.SetFieldValueI16("COUNTER", heartbeatCount);
        checker.calling("heartbeatMessage.AddField")
          .check(heartbeatMessage.AddField(tempField));

        // Publish the heartbeat message
        checker.calling("connection.Publish()")
          .check(connection.Publish(heartbeatMessage));

        // Ouput a heartbeat marker and reset the counter
        Log.Info(" " + (char)0x03);

        heartbeatCountdown = heartbeatHolder;
      }

      // Decrement the counters
      heartbeatCountdown = (short)(heartbeatCountdown - updateRate);

      // Sleep for 1 second
      Util.millisleep(1000);
    }

    // End time
    Log.Info(NEW_LINE + NEW_LINE + "End Time: " + Message.GetTime());

    // Stop auto dispatcher
    checker.calling("StopAutoDispatch").check(connection.StopAutoDispatch());

    return true;
  }

  public boolean cleanup() throws ExampleException
  {
    if (heartbeatMessage != null && heartbeatMessage.isValid())
    {
      Status status = connection.DestroyMessage(heartbeatMessage);
      Util.checkNoThrow("DestoryMessage", status);
    }

    if (connection != null)
      Util.closeConnection(connection);

    return true;
  }


  public static void main(String argv[])
  {
    // Simple check for proper usage
    if (argv.length != 1)
    {
      System.err.println(NEW_LINE +
                         "Usage:  java Replier <XML config filename>" +
                         NEW_LINE);
      return;
    }

    Example example = null;
    StatusChecker checker = new StatusChecker();

    try {
      // Create the config file and associate the config filename
      ConfigFile configFile = new ConfigFile(argv[0]);

      checker.calling("configFile.Load").check(configFile.Load());

      // Look up the config file value that contains the configuration for the 
      // middleware
      Config config = new Config(); 
      checker.calling("configFile.LookupConfig")
        .check(configFile.LookupConfig("connection-config", config));

      example = new Replier(config, configFile);
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
