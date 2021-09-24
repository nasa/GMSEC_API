 
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






 
/*
 * @file gmpub_gmd.java
 *
 * A simple example of SmartSockets GMD subscribing/publishing.
 *
 * For more information about SmartSockets GMD please refer to the
 * SmartSockets User's Guide, 
 */    

import gov.nasa.gsfc.gmsecapi.Config;
import gov.nasa.gsfc.gmsecapi.Connection;
import gov.nasa.gsfc.gmsecapi.ConnectionFactory;
import gov.nasa.gsfc.gmsecapi.ErrorCallback;
import gov.nasa.gsfc.gmsecapi.Field;
import gov.nasa.gsfc.gmsecapi.GMSEC_String;
import gov.nasa.gsfc.gmsecapi.Message;
import gov.nasa.gsfc.gmsecapi.Status;
import static gov.nasa.gsfc.gmsecapi.gmsecAPI.*;
import gov.nasa.gsfc.gmsecapi.util.*;


class GMDCallback implements ErrorCallback
{
  // Variable used for system dependent new line character
  static final String NEW_LINE = System.getProperty("line.separator");

  public void OnError(Connection conn, Message msg, Status status)
  {
    Log.Info("\n-- GMD Message Failure Notification Recieved --\n\n");

    Status result = new Status();

    GMSEC_String temp = new GMSEC_String();
    msg.GetSubject(temp);
    Log.Info("GMD failure: " + temp + " " + status.Get() + NEW_LINE);

    // Display received GMD Failure Message.
    msg.ToXML(temp);
    Log.Info("received GMD Failure Message =>\n" + temp);

    // *--------------------------------------------------*
    //                        NOTE:
    // *--------------------------------------------------*
    //
    //  Put Code HERE to process a GMD Timeout Failure
    //
    // *--------------------------------------------------*
  }
}

class DispatchCallback implements ErrorCallback
{
  // Variable used for system dependent new line character
  static final String NEW_LINE = System.getProperty("line.separator");

  public void OnError(Connection conn, Message msg, Status status)
  {
    Log.Info("Dispatch failure: " + status.Get() + NEW_LINE);
  }
}

public class gmpub_gmd implements Example
{
  // Variable used for system dependent new line character
  static final String NEW_LINE = System.getProperty("line.separator");

  static final String DEFAULT_SUBJECT = "GMSEC.TEST.PUBLISH";

  Config config;
  Config msg_cfg;
  Connection connection;
  Message message;
  StatusChecker checker = new StatusChecker();

  gmpub_gmd(Config config) throws ExampleException
  {
    this.config = config;
    Util.initialize(config);

    if (!Util.hasConnectionType(config)) {
      printUsage();
      throw new ExampleException("Invalid option");
    }
  }

  private void printUsage() {
    Log.Error("\nusage: java gmpub_gmd connectionType=<middleware> " +
        "server=<server name> " +
        "[ <parameter>=<value> ]\n" +
        "\n\tNote that the parameter 'connectiontype' " +
        "and 'server' are required. "  +
        "\n\tThe rest of other parameters are optional.\n" +
        "\n\tsubject=<subject name>" +
        "\n\titerations=<iterations>" +
        "\n\tinterval_ms=<interval (milliseconds)>" +
        "\n\tloglevel=<log level>" +
        "\n\tcfgfile=<config_filepath.xml>" +
        "\n\nFor more information, see API SmartSockets User's Guide\n\n");
  }

  public boolean run() throws ExampleException
  {
    /* Not needed in production, but it allows you to see what
     * options actually get set in SS (as a sanity check)
     * See GMSEC API Users Guide for other settings.
     * 
     * config.AddValue("COMMAND_FEEDBACK","interactive");
     * config.AddValue("COMMAND_FEEDBACK","never");  
     */
    config.AddValue("COMMAND_FEEDBACK","always");


    /* Add the gmdSubject to the connection configuration.
     * This subject needs to be static yet unique amongst GMD clients so
     * that the RTServer can associate a connection with each client.
     * In other words, the GMD subject used here can not be used by any
     * other client in the system.
     */
    config.AddValue("gmdSubject","GMSEC.GMD_Subject_publisher");

    
    /* Turn on ether Memory OR File based GMD
     * 
     * config.AddValue("IPC_GMD_TYPE","default");  // File based.
     */
    config.AddValue("IPC_GMD_TYPE","memory");

    /* Set the Server mode, so it 'remembers' if you leave and come back.
     * See GMSEC API Users Guide for other settings.
     * 
     * config.AddValue("SERVER_DISCONNECT_MODE","gmd_success");
     * config.AddValue("SERVER_DISCONNECT_MODE","gmd_failure");
     */
    config.AddValue("SERVER_DISCONNECT_MODE","warm");
    

    // Output GMSEC API version
    Log.Info(ConnectionFactory.GetAPIVersion());

    String subject = Util.get(config, "SUBJECT", DEFAULT_SUBJECT);
    long iterations = Util.get(config, "ITERATIONS", 1);
    int interval_ms = Util.get(config, "INTERVAL_MS", 1000);

    // Establish a connection
    connection = Util.openConnection(config);

    // Output middleware version
    Log.Info("Middleware version = " + connection.GetLibraryVersion());

    GMDCallback gmdcb = new GMDCallback();
    DispatchCallback dispcb = new DispatchCallback();
    
    Log.Info(NEW_LINE + "Registering error callback" + NEW_LINE);
    checker.calling("RegisterErrorCallback(CONNECTION_DISPATCHER_ERROR)")
      .check(connection.RegisterErrorCallback("CONNECTION_DISPATCHER_ERROR", 
                                              new DispatchCallback()));

    Log.Info(NEW_LINE + "Registering error callback" + NEW_LINE);
    checker.calling("Registering error callback")
      .check(connection.RegisterErrorCallback("SS_GMD_ERROR", 
                                              new GMDCallback()));

    connection.StartAutoDispatch();
    
    msg_cfg = new Config();
  
    /* Required to be set on the Message for GMD.  The T_IPC_DELIVERY_ALL
     * setting says to assure delivery to ALL subscribers before sending back
     * an ACK.
     * 
     * See GMSEC API Users Guide for other settings.
     * 
     * msg_cfg.AddValue("SETDELIVERYMODE","T_IPC_DELIVERY_BEST_EFFORT");
     * msg_cfg.AddValue("SETDELIVERYMODE","T_IPC_DELIVERY_ORDERED");
     * msg_cfg.AddValue("SETDELIVERYMODE","T_IPC_DELIVERY_SOME");  
     */
    msg_cfg.AddValue("SETDELIVERYMODE", "T_IPC_DELIVERY_ALL");

    /* Set Delivery Timeout to 15 seconds.  The Delivery Timeout determines
     * how long the publisher is willing to wait for all subscribers to
     * acknowledge the messaage.  It also determines the size (in units of
     * seconds) of the resend queue.  In this scenario, the resend queue
     * would contain 15 seconds worth messages.
     */
    msg_cfg.AddValue("SETDELIVERYTIMEOUT", "15.0");

    
    // Publish 
    for (long i=0; i < iterations; ++i)
    {
      publish(i, subject);
      if (i < iterations - 1)
        Util.millisleep(interval_ms);
    }
    
    return true;
  }

  void publish(long iteration, String subject) throws ExampleException
  {
    message = new Message();
    long value = iteration + 1;

    checker.calling("CreateMessage(" + subject + ")")
      .check(connection.CreateMessage(subject, GMSEC_MSG_PUBLISH,
                                      message, msg_cfg));
 
    Field field = new Field();
    field.SetFieldValueU32("sequence number", value);
    checker.calling("AddField(<GMSEC_U32>)").check(message.AddField(field));

    // Publish
    checker.calling("Publish").check(connection.Publish(message));
    Log.Info("Published sequence->" + value + NEW_LINE);

  }

  public boolean cleanup() throws ExampleException
  {
    if (message != null)
      Util.checkNoThrow("DestroyMessage",
                        connection.DestroyMessage(message));

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

      example = new gmpub_gmd(config);
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
