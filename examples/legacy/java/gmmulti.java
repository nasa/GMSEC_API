 
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






 
/** 
 * @file gmmulti.java
 *
 * An example Java GMSEC multi-connections
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

class TestConstant 
{
  public static final String SUBJECT1 = "GMSEC.TEST.CONNECT.AND.SEND";
  public static final String SUBJECT2 = "GMSEC.TEST.CONNECT.AND.SEND.2";
}

class PublishFinalCB implements Callback 
{
  public void OnMessage(Connection conn, Message message) 
  {
    // Dump Message
    GMSEC_String xml = new GMSEC_String();
    message.ToXML(xml);
    Log.Info("PublishFinalCB's received \n" + xml.Get());
  }
}

class ConnectAndSendCB implements Example, Callback 
{
  StatusChecker newChecker;
  Config newConfig;
  Connection newConnection;

  public ConnectAndSendCB(Config config) 
  {
    newChecker = new StatusChecker();
    newConfig = config;
    Util.initialize(newConfig);
  }


  public void OnMessage(Connection old_conn, Message old_message) 
  {
    // Dump Message
    GMSEC_String xml = new GMSEC_String();
    old_message.ToXML(xml);
    Log.Info("ConnectAndSendCB's received \n" + xml.Get());

    if (this == null)
    {
      Log.Warning("Empty Callback");
      return;
    } 

    try {
      run();
    }
    catch(ExampleException e) {
      e.printStackTrace();
    }
    finally {
      if (this != null)
        Util.cleanup(this);
    }
    Log.Info("--- END CALLBACK ---");
  }

  public boolean run() throws ExampleException
  {
    // Create the connection
    newConnection = Util.openConnection(newConfig);

    // create message to send
    Message newMessage = new Message();

    newChecker.calling("NewConnection.CreateMessage")
      .check(newConnection.CreateMessage(TestConstant.SUBJECT2,
                                         GMSEC_MSG_PUBLISH, newMessage));

    newChecker.calling("NewConnection.Publish")
      .check(newConnection.Publish(newMessage)); 

    GMSEC_String xml = new GMSEC_String();
    newChecker.calling("NewMessage.ToXML").check(newMessage.ToXML(xml));
    Log.Info("prepared from callback \n" + xml.Get());

    return true;
  }

  public boolean cleanup() throws ExampleException
  {
    Log.Info("--- CLOSE CONNECTION ---");
    if (newConnection != null)
      Util.closeConnection(newConnection);

    return true;
  }

}

public class gmmulti implements Example {

  StatusChecker checker;
  Config config;
  Connection connection;

  gmmulti(Config config) throws ExampleException
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
    Log.Error("\nusage: java gmmulti connectionType=<middleware> " +
        "[ <parameter>=<value> ]\n" +
        "\n\tNote that the parameter 'connectiontype' is required. "  +
        "\n\tThe rest of other parameters are optional.\n" +
        "\n\tserver=<server name> " +
        "(required if middleware is not bolt/MB locally)" +
        "\n\tloglevel=<log level>" +
		"\n\tcfgfile=<config_filepath.xml>" +
        "\n\nFor more information, see API User's Guide\n\n");
  }

  public boolean run() throws ExampleException 
  {
    // Output GMSEC API version
    Log.Info(ConnectionFactory.GetAPIVersion());

    // Establish a connection
    connection = Util.openConnection(config);

    // Output middleware version
    Log.Info("Middleware version = " + connection.GetLibraryVersion());

    // Subscribe
    Log.Info("Callback's subscribing to " + TestConstant.SUBJECT1);
    checker.calling("Subscribe(" + TestConstant.SUBJECT1 + ")") 
      .check(connection.Subscribe(TestConstant.SUBJECT1,
                                  new ConnectAndSendCB(config)));

    Log.Info("Callback's subscribing to " + TestConstant.SUBJECT2);
    checker.calling("Subscribe(" + TestConstant.SUBJECT2 + ")")
       .check(connection.Subscribe(TestConstant.SUBJECT2,
                                   new PublishFinalCB()));

    // start autodispatcher
    Log.Info("Start AutoDispatcher");
    checker.calling("StartAutoDispatch")
      .check(connection.StartAutoDispatch());

    // create message to send
    Message message = new Message();
    checker.calling("CreateMessage")
      .check(connection.CreateMessage(TestConstant.SUBJECT1,
                                      GMSEC_MSG_PUBLISH, message));

    // send message (xN)
    // Publish Message
    for (short i = 0; i < 10; i++) {
      Field field = new Field();

      checker.calling("SetFieldValueI16")
       .check(field.SetFieldValueI16("count", i));

      checker.calling("AddField(<I16>)")
        .check(message.AddField(field));

      // Publish Message
      checker.calling("Publish").check(connection.Publish(message));
    }
    // wait
    Util.millisleep(20000);

    // stop autodispatcher
    Log.Info("Stop AutoDispatcher");
    checker.calling("StopAutoDispatch").check(connection.StopAutoDispatch());

    return true;  
  }

  public boolean cleanup() throws ExampleException 
  {
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

      example = new gmmulti(config);
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
