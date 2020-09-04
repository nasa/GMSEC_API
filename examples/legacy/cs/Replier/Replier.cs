
/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/*! \file Replier.cs
 * 
 * This a sample application built with the C# GMSEC API. This application
 * demonstrates several key components within the GMSEC API including 
 * loading from a xml file, producing a heartbeat, and request/reply
 * functionality using callbacks. This works in sync with Requestor.cs
 * 
*/

using System;

namespace Replier
{
    using Nasa.Gmsec.Net;
    using Gmsec.Example.Common;

    class RequestReplyCallback : Callback
    {
        public ConfigFile cfgFile { get; set; }

        public override void OnMessage(Connection conn, Message msg)
        {
            try
            {
                OnMessageAux(conn, msg);
            }
            catch(Exception e)
            {
                Console.Out.WriteLine("[RequestReplyCallback::OnMessage]" + e.ToString());
            }
        }
        private void OnMessageAux(Connection conn, Message message)
        {
            Status result;
		    Field tempField;
		    Message replyMessage;
		    Message logMessage ;
		    string subject;
            GMSECMsgKindDefs msgKind;
		    string tempString;
            GmsecExample myEx = new GmsecExample();

		    /* We don't care about anything but request messages */
		    message.GetKind(out msgKind);
		    if (msgKind != GMSECMsgKindDefs.REQUEST)
			{
                return;
            }

		    /* Obtain the DIRECTIVE-KEYWORD field from the message.  It holds the request count. */
		    message.GetField("DIRECTIVE-KEYWORD", out tempField);
		    tempField.GetValue(out tempString);

		    /* Copy and output the request number which has been placed in the DIRECTIVE-KEYWORD field */
            Console.Out.WriteLine("\n\nReceived Request (" + tempString + "): " + DateTime.Now.ToString());

		    /* Create a generic message container for the directive response message */
		    result = conn.CreateMessage(out replyMessage);
		    myEx.check("CreateMessage", result);

		    /* Find and load the config file directive response message definition */
		    result = cfgFile.LookupMessage("DIRECTIVE-RESPONSE", replyMessage);
            myEx.check("LookupMessage", result);

		    /* Replace the DATA field with the request number we are responding to */
		    tempField.SetType(GMSECTypeDefs.STRING);
		    tempField.SetName("DATA");
		    tempField.SetValue(tempString);
		    result = replyMessage.AddField(tempField);
            myEx.check("AddField", result);

		    /* Replace the TIME-COMPLETE field with the correct time */
		    tempField.SetType(GMSECTypeDefs.STRING);
		    tempField.SetName("TIME-COMPLETED");
		    tempField.SetValue(DateTime.Now.ToString());
		    result = replyMessage.AddField(tempField);
            myEx.check("AddField", result);

		    /* Set the reply subject to the request message's subject */
		    message.GetSubject(out subject);
		    replyMessage.SetSubject(subject);

		    /* Send the reply */
		    result = conn.Reply(message,replyMessage);
            myEx.check("Replying", result);

            Console.Out.WriteLine("\nReply Sent ("+tempString+"): "+DateTime.Now.ToString());

		    /* Create a generic message container for the log message */
		    result = conn.CreateMessage(out logMessage);
            myEx.check("CreateMessage", result);

		    /* Find and load the config file log message definition */
		    result = cfgFile.LookupMessage("LOG-REP", logMessage);
            myEx.check("LookupMessage", result);

		    /* Fill the log message time */
		    tempField.SetType(GMSECTypeDefs.STRING);
		    tempField.SetName("EVENT-TIME");
		    tempField.SetValue(DateTime.Now.ToString());
		    result = logMessage.AddField(tempField);
            myEx.check("AddField", result);

		    /* Fill the log message message text */
		    tempField.SetType(GMSECTypeDefs.STRING);
		    tempField.SetName("MSG-TEXT");
		    tempField.SetValue(tempString);
		    result = logMessage.AddField(tempField);
            myEx.check("AddField", result);

		    // Publish the log message
		    result = conn.Publish(logMessage);
            myEx.check("Publish", result);

		    /* Clean up */
		    conn.DestroyMessage(logMessage);
		    conn.DestroyMessage(replyMessage);
        }
    }

    class Replier : GmsecExample
    {
        private Status result;
        private Connection conn;
        private ConfigFile cfgFile;

        public Replier()
        {
        }

        private void printUsage()
        {
            Console.Out.WriteLine("usage: Replier <filename>\n");
        }

        private void Run(string[] args)
        {
            if (args.Length < 1)
            {
                printUsage();
                return;
            }
            cfgFile = new ConfigFile(args[0]);

            Config connectionConfig;
            Config programConfig;
            Message heartbeatMessage;
            Field tempField;
            string tempSubject;
            //Config Variables
            short globalHeartbeatCount = 0;
            short heartbeatHolder = 0;
            short heartbeatCountdown = 0;
            short loopCountdown = 0;
            short updateRate = 0;

            //Subscription Callback
            RequestReplyCallback cb = new RequestReplyCallback();    

            //Load Config File
            result = cfgFile.Load();
            check("ConfigFileLoad", result);

            cb.cfgFile = cfgFile;

            //Look up configuration for the middleware
            result = cfgFile.LookupConfig("connection-config", out connectionConfig);
            check("LookUpConfig", result);

            //Create connection from configuration
            result = ConnectionFactory.Create(connectionConfig, out conn);
            check("Creating Connection", result);

            //Connect to the Network
            result = conn.Connect();
            check("Connect", result);

            //Output Info
            Console.Out.WriteLine(ConnectionFactory.GetAPIVersion());
            Console.Out.WriteLine("Middleware version= " + conn.GetLibraryVersion());

            //Lookup additional program configuration info
            result = cfgFile.LookupConfig("program-config", out programConfig);
            check("LookUpConfig", result);

            //Set Program Config Values
            updateRate = (short) get(programConfig, "update-rate", 1);
            loopCountdown = (short) get(programConfig, "loop-time", 30);

            //Create Subscriptions from subscriptions in configFile
            result = cfgFile.LookupSubscription("DIRECTIVE-REQUEST", out tempSubject);
            check("LookUpSubscription", result);
            result = conn.Subscribe(tempSubject, cb);
            check("Subscribe", result);

            //Create a generic message container for the heartbeat message
            result = conn.CreateMessage(out heartbeatMessage);
            check("CreateMessage", result);

            //Load Heartbeat Definition
            result = cfgFile.LookupMessage("C2CX-HEARTBEAT-REP", heartbeatMessage);
            check("LookUpMessage", result);

            //Obtain publish rate field from heartbeat message
            result = heartbeatMessage.GetField("PUB-RATE", out tempField);
            check("GetField", result);
            result = tempField.GetValue(out heartbeatHolder);
            check("GetValue", result);

            //Ouput some Program Info
            Console.Out.WriteLine("Publishing for " + loopCountdown + " seconds.");
            Console.Out.WriteLine("Publishing Heartbeat Messages every " + heartbeatHolder
                + " seconds.");

            result = conn.StartAutoDispatch();
            check("Starting AutoDispatch", result);
            Console.Out.WriteLine("Start Time: " + DateTime.Now.ToString());

            //Publish Loop
            for (; loopCountdown > 0; loopCountdown--)
            {
                //When Countdown reaches 0 publish heartbeat & reset
                if (heartbeatCountdown < 1)
                {
                    globalHeartbeatCount++;

                    //Update Message Counter
                    tempField.SetType(GMSECTypeDefs.I32);
                    tempField.SetName("COUNTER");
                    tempField.SetValue(globalHeartbeatCount);
                    result = heartbeatMessage.AddField(tempField);
                    check("AddField", result);

                    //Publish Heartbeat Message
                    result = conn.Publish(heartbeatMessage);
                    check("Publish", result);

                    //Output Heartbeat marker and reset the counter
                    Console.Out.WriteLine("Published Heartbeat");
                    heartbeatCountdown = heartbeatHolder;
                }
                //Decrement the Counters
                heartbeatCountdown -= updateRate;
                //Sleep for 1 second
                System.Threading.Thread.Sleep(updateRate * 1000);
            }
            //EndTime
            Console.Out.WriteLine("End Time: " + DateTime.Now.ToString());
            result = conn.StopAutoDispatch();
            check("StopAutoDispatch", result);
            result = conn.DestroyMessage(heartbeatMessage);
            check("Destory Message", result);
        }


        static void Main(string[] args)
        {
            try
            {
                Replier appReplier = new Replier();
                appReplier.Run(args);
            }
            catch (Exception e)
            {
                Console.Out.WriteLine(e.ToString());
            }
        }
    }
}
