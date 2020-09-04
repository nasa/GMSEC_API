 
/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/**
 * @file Recorder.java
 *
 * A simple examples of various GMSEC API functionality.
 *
 * Contained within this example:
 *
 * - Loading an XML configuration file
 * - Populating a Config object using the XML file
 * - Creating a connection using the Config object
 * - Connecting to the network
 * - Using the XML file to store and set various other program configurations
 * - Subscribing to specific subjects to receieve messages through a callback
 * - Starting the automatic message dispatching thread
 * - Creating a heartbeat message from an XML definition
 * Displaying message XML
 * - Publishing the heartbeat message using the Publish call
 * - Programatically altering a field in the heartbeat message
 * Cleanup
 * - Message memory cleanup
 * - Connection cleanup including disconnecting from the network
 *
 *  Any item with a * next to it is specific to this file in the overall example.
 */


import gov.nasa.gsfc.gmsecapi.Callback;
import gov.nasa.gsfc.gmsecapi.Config;
import gov.nasa.gsfc.gmsecapi.ConfigFile;
import gov.nasa.gsfc.gmsecapi.Connection;
import gov.nasa.gsfc.gmsecapi.ConnectionFactory;
import gov.nasa.gsfc.gmsecapi.Field;
import gov.nasa.gsfc.gmsecapi.GMSEC_I16;
import gov.nasa.gsfc.gmsecapi.GMSEC_String;
import gov.nasa.gsfc.gmsecapi.Message;
import gov.nasa.gsfc.gmsecapi.Status;
import static gov.nasa.gsfc.gmsecapi.gmsecAPI.*;
import gov.nasa.gsfc.gmsecapi.util.*;


/** 
 * Callback Class for Subscriptions
 */
class LogCallback implements Callback
{
  public void OnMessage(Connection conn, Message msg)
  {
    // Print the message
    GMSEC_String prtMsg = new GMSEC_String();
    msg.ToXML(prtMsg);
    Log.Info(prtMsg.Get());
  }
}


public class Recorder implements Example
{
  // Variable used for system dependent new line character
  static final String NEW_LINE = System.getProperty("line.separator");

  Config config;
  Config programConfig;
  ConfigFile configFile;
  Connection connection;
  Message heartbeatMessage;
  StatusChecker checker = new StatusChecker();

  Recorder(Config config, ConfigFile configFile)
  {
    this.config = config;
    this.configFile = configFile;
    programConfig = new Config();
    Util.initialize(config);

  }

  public boolean run() throws ExampleException
  {
    // Establish a connection 
    connection = Util.openConnection(config);

    // output GMSEC API and Middleware info, just for display purposes
    Log.Info(ConnectionFactory.GetAPIVersion()  + NEW_LINE);
    Log.Info(connection.GetLibraryVersion() + NEW_LINE);

    // Additional configuration
    // Look up additional program configutation information
    checker.calling("configFile.LookupConfig(programConfig)")
      .check(configFile.LookupConfig("program-config", programConfig));

    // Set program config values
    GMSEC_String configValue = new GMSEC_String();
    checker.calling("programConfig.GetValue(update-rate)")
      .check(programConfig.GetValue("update-rate", configValue));

    // Update rate
    short updateRate = 0;
    updateRate = Short.parseShort(configValue.Get());

    // Loop time
    checker.calling("programConfig.GetValue(loop-time)")
      .check(programConfig.GetValue("loop-time", configValue));

    short loopCountdown = Short.parseShort(configValue.Get());

    // Create subscriptions from subscription templates in the config file
    GMSEC_String tempSubject = new GMSEC_String();
    checker.calling("configFile.LookupSubscription(RECEIVE-LOG)")
      .check(configFile.LookupSubscription("RECEIVE-LOG", tempSubject));

    // Subscribe
    Log.Info("subscribing to " + tempSubject.Get());
    checker.calling("Subscribe("+ tempSubject.Get() + ")")
      .check(connection.Subscribe(tempSubject.Get(), new LogCallback()));

    checker.calling("configFile.LookupSubscription(SEND-LOG)")
      .check(configFile.LookupSubscription("SEND-LOG", tempSubject));

    Log.Info("subscribing to " + tempSubject.Get());
    checker.calling("Subscribe("+ tempSubject.Get() + ")")
      .check(connection.Subscribe(tempSubject.Get(), new LogCallback()));
    
    // Create Heartbeat Message
    // Create a generic message container for the heartbeat message
    heartbeatMessage = new Message();
    checker.calling("CreateMessage(heartbeatMessage)")
      .check(connection.CreateMessage(heartbeatMessage));

    // Find and load the config file heartbeat message definition
    checker.calling("configFile.LookupMessage(C2CX-HEARTBEAT-REC)")
      .check(configFile.LookupMessage("C2CX-HEARTBEAT-REC", heartbeatMessage));

    Status status = new Status();

    // Obtain the publish rate field, by name, from the heartbeat message
    Field tempField = new Field();
    short heartbeatHolder = 0; 
    status = heartbeatMessage.GetField("PUB-RATE", tempField); 
    if (!status.isError())
    {
      // Obtain the value from the field
      GMSEC_I16 tempShort = new GMSEC_I16();
      checker.calling("tempField.GetValue()")
        .check(tempField.GetValueI16(tempShort));

      heartbeatHolder = tempShort.Get();
    }
    else
    {
      // Default value
      heartbeatHolder = 30;
    }

    // Main Publishing Loop
    // Output some general program information
    Log.Info("Publishing for " + loopCountdown + " seconds." + NEW_LINE);
    Log.Info("Publishing Heartbeat Messages every " + heartbeatHolder +
             " seconds." + NEW_LINE);

    // Call to create the thread that will automatically retrieve messages off
    // the message queue.
    connection.StartAutoDispatch();

    // Start time
    Log.Info("Start Time: " + Message.GetTime() + NEW_LINE + NEW_LINE);
  
    // Publishing loop
    short heartbeatCount = 0;
    short heartbeatCountdown = 0;
    for (; loopCountdown > 0; loopCountdown--)
    {
      // When the countdown reaches 0, we publish a heartbeat message and
      //  reset the counter
      if (heartbeatCountdown < 1)
      {
        heartbeatCount++;

        // Update the message counter
        tempField.SetFieldValueI16("COUNTER", heartbeatCount);
        checker.calling("heartbeatMessage.AddField()")
          .check(heartbeatMessage.AddField(tempField));

        // Publish the heartbeat message
        checker.calling("connection.Publish")
          .check(connection.Publish(heartbeatMessage));

        // Ouput a heartbeat marker and reset the counter
        Log.Info((char)0x03 + NEW_LINE);
        heartbeatCountdown = heartbeatHolder;
      }

      // Decrement the counters
      heartbeatCountdown = (short)(heartbeatCountdown - updateRate);

      // Sleep for 1 second
      Util.millisleep(1000);
    }

    // End time
    Log.Info(NEW_LINE + NEW_LINE+ "End Time: " + Message.GetTime() + NEW_LINE);

    // Stop auto dispatcher
    checker.calling("connection.StopAutoDispatch")
     .check(connection.StopAutoDispatch());

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
                         "Usage:  java Recorder <XML config filename>" +
                         NEW_LINE + NEW_LINE);
      System.exit(-1);
    }

    Example example = null;
    StatusChecker checker = new StatusChecker();

    try {
      // Create the config file and associate the config filename
      ConfigFile configFile = new ConfigFile(argv[0]);
      Config config = new Config();

      checker.calling("configFile.Load").check(configFile.Load());

      // Look up the config file value that contains the configuration for 
      // the middleware.
      checker.calling("configFile.LookupConfig")
        .check(configFile.LookupConfig("connection-config", config));

      example = new Recorder(config, configFile);
      example.run();
    }
    catch(ExampleException e) {
      e.printStackTrace();
    }
    finally {
      if (example != null)
        Util.cleanup(example);
    }
  }
}
