 
/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






 
/**
 * @file Requestor.java
 *
 * A simple examples of various GMSEC API functionality.
 *
 * Contained within this example:
 *
 * - Loading an XML configuration file
 * - Populating a Config object using the XML file
 * - Establish a connection
 * - Using the XML file to store and set various other program configurations
 * - Creating a heartbeat message from an XML definition
 * - Publishing the heartbeat message using the Publish call
 * - Programatically altering a field in the heartbeat message
 * Creating a directive request message from an XML definition
 * Publishing the directive request message using the Request call
 * Using a callback class object to receieve a response from the request call
 * - Creating a log message from an XML definition
 * - Programatically altering a field in the log message
 * - Publishing the log message using the Publish call
 * Cleanup
 * - Message memory cleanup
 * - Connection cleanup including Disconnecting from the network
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
import gov.nasa.gsfc.gmsecapi.ReplyCallback;
import gov.nasa.gsfc.gmsecapi.Status;
import static gov.nasa.gsfc.gmsecapi.gmsecAPI.*;
import gov.nasa.gsfc.gmsecapi.util.*;


class RequestInnerExample implements Example
{
  static final String NEW_LINE = System.getProperty("line.separator");

  ConfigFile configFile;
  Connection connection;
  Message message;
  Message logMessage;
  StatusChecker statusChecker;

  RequestInnerExample(ConfigFile configFile, Connection connection, Message message,
               StatusChecker statusChecker)
  {
    this.configFile = configFile;
    this.connection= connection;
    this.message = message;
    this.statusChecker = statusChecker;
    logMessage = new Message();
  }

  public boolean run() throws ExampleException
  {
    Field tempField = new Field();
    GMSEC_I16 tempStatus = new GMSEC_I16();
    GMSEC_String tempString = new GMSEC_String();
    GMSEC_String stringHolder = new GMSEC_String();
 
    // Get the status of the response (successful, failed, ect) and act accordingly
    message.GetField("RESPONSE-STATUS", tempField);
    tempField.GetValueI16(tempStatus);

    switch(tempStatus.Get())
    {
      case 1 :
        Log.Info(NEW_LINE + "Acknowledgement");
        break;
      case 2 :
        Log.Info(NEW_LINE + "Working/keep alive");
        break;
      case 3 :
        // Obtain the DATA field which was filled with what request this 
        // reply was generated from
        message.GetField("DATA", tempField);
        tempField.GetValueSTRING(tempString);
        stringHolder = tempString;

        // Output that we've recieved a reply
        Log.Info(NEW_LINE + "Recieved Reply (" + tempString + "): " + 
                 Message.GetTime());
        break;
      case 4 :
        Log.Info(NEW_LINE + "Failed completion");
        break;
      case 5 :
        Log.Info(NEW_LINE + "Invalid Request");
        break;
      default :
        break;
    }

    // Create a generic message container for the log message
    statusChecker.calling("connection.CreateMessage(logMessage)")
      .check(connection.CreateMessage(logMessage));

    // Find and load the config file log message definition
    statusChecker.calling("configFile.LookupMessage(LOG-REQ)")
      .check(configFile.LookupMessage("LOG-REQ", logMessage));

    // Fill the log message time
    tempField.SetFieldValueSTRING("EVENT-TIME", Message.GetTime());
    statusChecker.calling("logMessage.AddField(EVENT-TIME)")
      .check(logMessage.AddField(tempField));

    // Fill the log message message text
    tempField.SetFieldValueSTRING("MSG-TEXT", stringHolder.Get());
    statusChecker.calling("logMessage.AddField(MSG-TEXT)")
      .check(logMessage.AddField(tempField));

    // Publish the log message
    statusChecker.calling("connection.Publish")
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

    return true;
  } 
};


// Callback Class for Request-Reply ///
class RequestorReplyCallback implements ReplyCallback
{
  static final String NEW_LINE = System.getProperty("line.separator");

  // Holds the config file used to create new messages
  private ConfigFile configFile;
  private StatusChecker checker;

  public RequestorReplyCallback(ConfigFile configFile, StatusChecker checker)
  {
    this.configFile = configFile;
    this.checker = checker;
  }
  
  public void OnReply(Connection conn, Message request, Message reply)
  {
    // We don't care about anything but reply messages
    GMSEC_UShort msgKind = new GMSEC_UShort();
    reply.GetKind(msgKind);
    if (msgKind.Get() != GMSEC_MSG_REPLY)
      return;

    Example innerExample = null;
    try {
      innerExample = new RequestInnerExample(configFile, conn, reply, checker);
      innerExample.run();
    }
    catch (ExampleException e) {
      e.printStackTrace();
    }
    finally {
      if (innerExample != null)
        Util.cleanup(innerExample);
    }
  }

  public void OnError(Connection conn, Message request, Status status)
  {
    Log.Warning("OnError -> \n" + status.Get());
  }

};

public class Requestor implements Example
{
  static final String NEW_LINE = System.getProperty("line.separator");

  Config config;
  ConfigFile configFile;
  Connection connection;
  Message heartbeatMessage;
  Message directiveRequestMessage;
  StatusChecker checker;
 
  Requestor(Config config, ConfigFile configFile)
  {
    this.config = config;
    this.configFile = configFile;
    checker = new StatusChecker();
    Util.initialize(config);
  }

  public boolean run() throws ExampleException
  {
    // Variable declarations
    Status result = new Status();
    Field tempField = new Field();
    short requestCount = 0;
    short heartbeatCount = 0;

    // Config variables
    GMSEC_I16 heartbeatHolder = new GMSEC_I16();
    short heartbeatCountdown = 0;    
    short directiveHolder = 0;    
    short directiveCountdown = 0;    
    short loopCountdown = 0;      
    short updateRate = 0;        

    // Establish a connection
    connection = Util.openConnection(config);

    // output GMSEC API and Middleware info, just for display purposes
    Log.Info(ConnectionFactory.GetAPIVersion());
    Log.Info(connection.GetLibraryVersion());

    // Additional Configuration
    Config programConfig = new Config();

    // Look up additional program configutation information
    checker.calling("LookupConfig(program-config)")
      .check(configFile.LookupConfig("program-config", programConfig));

    // Set program config values
    GMSEC_String configValue = new GMSEC_String();
    checker.calling("programConfig.GetValue(update-rate)")
      .check(programConfig.GetValue("update-rate", configValue));

    updateRate = Short.parseShort(configValue.Get());

    checker.calling("programConfig.GetValue(loop-time)") 
      .check(programConfig.GetValue("loop-time", configValue));

    loopCountdown = Short.parseShort(configValue.Get());

    checker.calling("programConfig.GetValue(send-directive-rate)")
      .check(programConfig.GetValue("send-directive-rate", configValue));

    directiveHolder = Short.parseShort(configValue.Get());

    // Create Heartbeat Message
    heartbeatMessage = new Message();


    // Create a generic message container for the heartbeat message
    checker.calling("connection.CreateMessage(heartbeatMessage)")
      .check(connection.CreateMessage(heartbeatMessage));

    // Find and load the config file heartbeat message definition
    checker.calling("configFile.LookupMessage(C2CX-HEARTBEAT-REQ)")
      .check(configFile.LookupMessage("C2CX-HEARTBEAT-REQ", heartbeatMessage));

    // Obtain the publish rate field, by name, from the heartbeat message
    checker.calling("heartbeatMessage.GetField(PUB-RATE)")
      .check(heartbeatMessage.GetField("PUB-RATE", tempField));

    // Obtain the value from the field
    checker.calling("tempField.GetValueI16(heartbeatHolder)")
      .check(tempField.GetValueI16(heartbeatHolder));


    // Create a generic message container for the Directive Request Message
    directiveRequestMessage = new Message();
    checker.calling("connection.CreateMessage(directiveRequestMessage)")
      .check(connection.CreateMessage(directiveRequestMessage));

    // Find and load the config file Directive Request Message definition
    checker.calling("configFile.LookupMessage(DIRECTIVE-REQUEST)")
      .check(configFile.LookupMessage("DIRECTIVE-REQUEST", directiveRequestMessage));


    // Main Publishing Loop
    // Output some general program information
    Log.Info("Publishing for " + loopCountdown + " seconds.");
    Log.Info("Publishing Directive Request Messages every " + directiveHolder + 
             " seconds.");
    Log.Info("Publishing Heartbeat Messages every " + heartbeatHolder + " seconds.");

    // Start time
    Log.Info("Start Time: " + Message.GetTime());

    // Publishing loop
    for (; loopCountdown > 0; loopCountdown--)
    {
      // When the countdown reaches 0, we publish a directive message and
      //  reset the counter
      if (directiveCountdown < 1)
      {
        requestCount++;

        // Alter the directive request to a unique string
        tempField.SetFieldValueSTRING("DIRECTIVE-KEYWORD", 
                                      Short.toString(requestCount));
        checker.calling("directiveRequestMessage.AddField(tempField)")
          .check(directiveRequestMessage.AddField(tempField));

        // Alter the directive request to a unique string
        tempField.SetFieldValueSTRING("DIRECTIVE-STRING", 
                                      "DIRECTIVE REQUEST MESSAGE" + requestCount);
        checker.calling("directiveRequestMessage.AddField(tempField)")
          .check(directiveRequestMessage.AddField(tempField));
 
        // Publish time
        Log.Info(NEW_LINE + NEW_LINE + "Publishing Directive Request Message (" + 
                 requestCount + "): " + Message.GetTime());

        // Send the request with the reply being send to the callback we created
        checker.calling("connection.Request(directiveRequestMessage)")
          .check(connection.Request(directiveRequestMessage, GMSEC_WAIT_FOREVER, 
                                    new RequestorReplyCallback(configFile, checker)));

        // Reset the countdown
        directiveCountdown = directiveHolder;
      }
    
      // When the countdown reaches 0, we publish a heartbeat message and
      //  reset the counter
      if (heartbeatCountdown < 1)
      {
        heartbeatCount++;

        // Update the message counter field
        tempField.SetFieldValueI16("COUNTER", heartbeatCount);
        checker.calling("heartbeatMessage.AddField(tempField)")
          .check(heartbeatMessage.AddField(tempField));

        // Publish the heartbeat message
        checker.calling("connection.Publish")
          .check(connection.Publish(heartbeatMessage));

        // Ouput a heartbeat marker and reset the counter
        Log.Info(" " + (char)0x03);
        heartbeatCountdown = heartbeatHolder.Get();
      }

      // Decrement the counters
      heartbeatCountdown = (short)(heartbeatCountdown - updateRate);
      directiveCountdown = (short)(directiveCountdown - updateRate);

      // Sleep for 1 second
      Util.millisleep(1000);
    }

    // End time
    Log.Info(NEW_LINE + NEW_LINE + "End Time: " + Message.GetTime());

    return true;
  }

  public boolean cleanup() throws ExampleException
  {
    if (heartbeatMessage != null && heartbeatMessage.isValid())
    {
      Status status = connection.DestroyMessage(heartbeatMessage);
      Util.checkNoThrow("DestoryMessage", status);
    }

    if (directiveRequestMessage != null && directiveRequestMessage.isValid())
    {
      Status status = connection.DestroyMessage(directiveRequestMessage);
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
                         "Usage:  java Requestor <XML config filename>" + 
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

      example = new Requestor(config, configFile);
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
