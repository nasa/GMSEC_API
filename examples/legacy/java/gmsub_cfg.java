 
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






 
import java.io.IOException;
import java.lang.reflect.Array;

import gov.nasa.gsfc.gmsecapi.Callback;
import gov.nasa.gsfc.gmsecapi.Config;
import gov.nasa.gsfc.gmsecapi.ConfigFile;
import gov.nasa.gsfc.gmsecapi.Connection;
import gov.nasa.gsfc.gmsecapi.ConnectionFactory;
import gov.nasa.gsfc.gmsecapi.ErrorCallback;
import gov.nasa.gsfc.gmsecapi.GMSEC_String;
import gov.nasa.gsfc.gmsecapi.Message;
import gov.nasa.gsfc.gmsecapi.Status;
import gov.nasa.gsfc.gmsecapi.gmsecAPI;
import gov.nasa.gsfc.gmsecapi.util.*;

/**
 * @file gmsub_cfg.java
 *
 * A Java demonstration example of Java subscriber using the configuration 
 * file.
 *
 */

class gmsub_cfg_cb implements Callback, ErrorCallback, Runnable 
{
  StatusChecker checker;
  Connection conn = null;
  Message msg = null;

  gmsub_cfg_cb(Connection conn, Message msg)
  {
    checker = new StatusChecker();
    this.conn = conn;
    this.msg = msg;
  }

  public void OnMessage(Connection conn, Message msg) 
  {
    GMSEC_String xml = new GMSEC_String();

    msg.ToXML(xml);
    Log.Info(xml.Get());

    Log.Info("Press the enter key to exit");
  }

  public void run() {
    // publish a message
    for(int i = 0; i < 10; i++) {
      Log.Info("sending...");

      checker.calling("Publish").checkNoThrow(conn.Publish(msg));

      try {
        Thread.sleep(5000);
      } 
      catch (InterruptedException e1) {
        Log.Info("Sleep interrupted, exiting program");
        break;
      }
    }
  }

  public void OnError(Connection conn, Message msg, Status status) 
  {
    if (status.GetCode() != gmsecAPI.GMSEC_TIMEOUT_OCCURRED && 
        status.GetCode() != gmsecAPI.GMSEC_NO_MESSAGE_AVAILABLE) 
    {
      Log.Warning("OnError: " + status.Get());
    }
  }
}

public class gmsub_cfg implements Example
{
  StatusChecker checker;
  Config config;
  ConfigFile configFile;
  Connection connection = null;
  Connection connectionTwo = null;

  public gmsub_cfg(Config config, ConfigFile configFile)
  {
    checker = new StatusChecker();
    this.config = config;
    this.configFile = configFile;
    Util.initialize(config);
  }

  public boolean run() throws ExampleException
  {
    // Output GMSEC API version
    Log.Info(ConnectionFactory.GetAPIVersion());

    // Establish a connection
    Log.Info("Creating first connection"); 
    connection = Util.openConnection(config);

    Log.Info("Creating second connection"); 
    connectionTwo = Util.openConnection(config);

    // lookup subscription
    GMSEC_String sub1 = new GMSEC_String();

    checker.calling("LookupSubscription:SUB1")
      .check(configFile.LookupSubscription("SUB1", sub1));

    // lookup message
    Message message = new Message();

    // create message
    checker.calling("CreateMessage").check(connection.CreateMessage(message));

    checker.calling("LookupMessage:MSG1")
      .check(configFile.LookupMessage("MSG1", message));

    gmsub_cfg_cb gm_callback = new gmsub_cfg_cb(connection, message);
  
    // subscribe
    Log.Info("Subscribing to first connection's SUB1: " + sub1.Get());
    checker.calling("connection.Subscribe(SUB1)")
      .check(connection.Subscribe(sub1.Get(), gm_callback));

    Log.Info("Subscribing to second connection's SUB1: " + sub1.Get());
    checker.calling("connectionTwo.Subscribe(SUB1)")
      .check(connectionTwo.Subscribe(sub1.Get(), gm_callback));

    // Register error callback
    Log.Info("Registering error callback for connection dispatcher");
    checker.calling("RegisterErrorCallback(CONNECTION_DISPATCHER_ERROR)")
      .check(connection.RegisterErrorCallback("CONNECTION_DISPATCHER_ERROR", 
                                              gm_callback));

    Log.Info("Registering error callback for SS failover");
    checker.calling("RegisterErrorCallback(SS_FAILOVER_ERROR)")
      .check(connection.RegisterErrorCallback("SS_FAILOVER_ERROR", 
                                              gm_callback));

    // Start autodispatcher
    Log.Info("Start first connection's autodispatch");
    checker.calling("connection.StartAutoDispatch")
      .check(connection.StartAutoDispatch());

    Log.Info("Start second connection's autodispatch");
    checker.calling("connectionTwo.StartAutoDispatch")
      .check(connectionTwo.StartAutoDispatch());

    // publish a message
    Thread t = new Thread(gm_callback);
    t.start();
   
    Log.Info("Press the enter key to exit");
 
    try {
      // wait for input
      int c = System.in.read();
    }
    catch (IOException e) {
      e.printStackTrace();
    }
    Log.Info("shutting down...");
    
    t.interrupt();
    
    // Stop autodispatcher
    Log.Info("Stop first connection's autodispatch");
    checker.calling("connection.StopAutoDispatch")
      .check(connection.StopAutoDispatch());

    Log.Info("Stop second connection's autodispatch");
    checker.calling("connectionTwo.StopAutoDispatch")
      .check(connectionTwo.StopAutoDispatch());

    return true;
  }

  public boolean cleanup() throws ExampleException
  {
    if (connection != null)
      Util.closeConnection(connection);

    if (connectionTwo != null)
      Util.closeConnection(connectionTwo);

    return true;
  }


  public static void main(String[] args) 
  {
    if (Array.getLength(args) != 1) {
      System.err.println("\nusage: java gmsub_cfg <config_file.xml>\n");
      return;
    }

    Example example = null;
    StatusChecker status = new StatusChecker();

    Log.Info("loading:" + args[0]);
    
    try {
      // Load configuration file
      ConfigFile configFile = new ConfigFile(args[0]);
      status.calling("Loading File").check(configFile.Load());
      
      // Look up the configuration
      Config config = new Config();
      status.calling("Lookup Config")
        .check(configFile.LookupConfig("default",config));

      example = new gmsub_cfg(config, configFile);
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
