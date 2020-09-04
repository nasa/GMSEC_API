
/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/*! \file Requestor.cs
 * 
 * This a sample application built with the C# GMSEC API. This application
 * demonstrates several key components within the GMSEC API including loading
 * from an xml file, producing a heartbeat, and request/reply functionality
 * using callbacks. This works in sync with Replier.cs
 * 
*/

using System;

namespace Requestor
{
    using Nasa.Gmsec.Net;
    using Gmsec.Example.Common;

    class RequestReplyCallback : ReplyCallback
    {
        public ConfigFile cfgFile { get; set; }
        public override void OnReply(Connection conn, Message request_msg, Message reply_msg)
        {
            try
            {
                OnReplyAux(conn, request_msg, reply_msg);
            }
            catch (Exception e)
            {
                Console.Out.WriteLine("[RequestReplyCallback::OnMessage]" + e.ToString());
            }
        }

        private void OnReplyAux(Connection conn, Message reqMsg, Message rplMsg)
        {
            Status result;
		    Field tempField;
		    Message logMessage;
            short tempStatus;

		    string tempString = "";
            GmsecExample myEx = new GmsecExample();

            //Get the status of the response (successful, failed, etc) act accordingly
            result = rplMsg.GetField("RESPONSE-STATUS", out tempField);
            if (!myEx.checkNoThrow("GetField(RESPONSE-STATUS)", result))
            {
                return;
            }
            result = tempField.GetValue(out tempStatus);
            if (!myEx.checkNoThrow("GetValue(RESPONSE-STATUS)", result))
            {
                return;
            }
            switch (tempStatus)
            {
                case 1:
                    Console.Out.WriteLine("\nAcknowledgement");
                    break;
                case 2:
                    Console.Out.WriteLine("\nWorking/keep alive");
                    break;
                case 3:
                    //Obtain the DATA field which was filled with what request this reply
                    //was generated from
                    result = rplMsg.GetField("DATA", out tempField);
                    if (!myEx.checkNoThrow("GetField(DATA)", result))
                    {
                        return;
                    }
                    result = tempField.GetValue(out tempString);
                    if(!myEx.checkNoThrow("GetValue(DATA", result))
                    {
                        return;
                    }
                    if (tempString.Length != 0)
                    {
                        //Output that we'v received a reply
                        Console.Out.WriteLine("\nReceived Reply (DATA=" + tempString
                            + "): " + DateTime.Now.ToString());
                    }
                    break;
                case 4:
                    Console.Out.WriteLine("\nFailed completion");
                    break;
                case 5:
                    Console.Out.WriteLine("\nInvalid Request");
                    break;
                default:
                    break;
            }

            //Create a generic message container for the log message
            result = conn.CreateMessage(out logMessage);
            myEx.check("CreateMessage", result);
            //Find and load the config file logmessage definition
            result = cfgFile.LookupMessage("LOG-REQ", logMessage);
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
            myEx.check("AddField(MSG-TEXT)", result);

            // Publish the log message
            result = conn.Publish(logMessage);
            myEx.check("Publish", result);
            
            conn.DestroyMessage(logMessage);
        }
    }

    class Requestor : GmsecExample
    {
        private Status result;
        private Connection conn;
        private ConfigFile cfgFile;

        public Requestor()
        {
        }

        private void printUsage()
        {
            Console.Out.WriteLine("usage: Requestor <filename>\n");
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
            Message directiveRequestMessage;
            Message heartbeatMessage;
            Field tempField;
            short requestCount = 0;
            short heartbeatCount = 0;
            //Config Variables
            short heartbeatCountdown = 0;
            short heartbeatHolder = 0;
            short directiveCountdown = 0;
            short directiveHolder = 0;
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
            Console.Out.WriteLine("Middleware version= "+conn.GetLibraryVersion());

            //Lookup additional program configuration info
            result = cfgFile.LookupConfig("program-config", out programConfig);
            check("LookUpConfig", result);

            //Set Program Config Values
            updateRate = (short) get(programConfig, "update-rate", 1);
            loopCountdown = (short) get(programConfig, "loop-time", 30);
            directiveHolder = (short) get(programConfig, "send-directive-rate", 5);

            //Create a generic message container for the heartbeat message
            result = conn.CreateMessage(out heartbeatMessage);
            check("CreateMessage", result);

            //Load Heartbeat Definition
            result = cfgFile.LookupMessage("C2CX-HEARTBEAT-REQ", heartbeatMessage);
            check("LookUpMessage", result);

            //Obtain publish rate field from heartbeat message
            result = heartbeatMessage.GetField("PUB-RATE", out tempField);
            check("GetField", result);
            result = tempField.GetValue(out heartbeatHolder);
            check("GetValue", result);

            //Create generic message container for the Directive Request Message
            result = conn.CreateMessage(out directiveRequestMessage);
            check("CreatMessage", result);
            result = cfgFile.LookupMessage("DIRECTIVE-REQUEST", directiveRequestMessage);
            check("LookupMessage", result);

            //Ouput some Program Info
            Console.Out.WriteLine("Publishing for " + loopCountdown + " seconds.");
            Console.Out.WriteLine("Publishing Directive Request Messages every "
                + directiveHolder + " seconds.");
            Console.Out.WriteLine("Publishing Heartbeat Messages every " + heartbeatHolder
                + " seconds.");

            Console.Out.WriteLine("Start Time: " + DateTime.Now.ToString());

            //Publish Loop
            for (; loopCountdown > 0; loopCountdown--)
            {
                //When Countdown reaches 0 publish a directive message & reset
                if (directiveCountdown < 1)
                {
                    requestCount++;
                    //Alter the directive request to a unique setting
                    tempField.SetType(GMSECTypeDefs.STRING);
                    tempField.SetName("DIRECTIVE-KEYWORD");
                    tempField.SetValue(requestCount.ToString());
                    result = directiveRequestMessage.AddField(tempField);
                    check("AddField", result);

                    //Alter the directive request to a unique string
                    tempField.SetType(GMSECTypeDefs.STRING);
                    tempField.SetName("DIRECTIVE-STRING");
                    tempField.SetValue("DIRECTIVE REQUEST MESSAGE " + requestCount.ToString());
                    result = directiveRequestMessage.AddField(tempField);
                    check("AddField", result);
                    
                    //Publsih
                    Console.Out.WriteLine("\n\nPublishing Directive Request Message ("
                        + requestCount + "): " + DateTime.Now.ToString());
                    //Send the request with the reply being sent to the callback
                    result = conn.Request(directiveRequestMessage,(int)GMSECWaitDefs.WAIT_FOREVER,cb);
                    check("Requesting", result);
                    
                    //Reset the countdown
                    directiveCountdown = directiveHolder;
                }

                //When the countdown reaches 0, we publish a heartbeat message & reset
                if (heartbeatCountdown < 1)
                {
                    heartbeatCount++;

                    //Update Message Counter
                    tempField.SetType(GMSECTypeDefs.STRING);
                    tempField.SetName("COUNTER");
                    tempField.SetValue(heartbeatCount);
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
                directiveCountdown -= updateRate;

                //Sleep for update # seconds
                System.Threading.Thread.Sleep(updateRate * 1000);
            }
            //EndTime
            Console.Out.WriteLine("End Time: " + DateTime.Now.ToString());
            result = conn.DestroyMessage(heartbeatMessage);
            check("Destory Message", result);
            result = conn.DestroyMessage(directiveRequestMessage);
            check("Destory Message", result);
        }


        static void Main(string[] args)
        {
            try
            {
                Requestor appRequestor = new Requestor();
                appRequestor.Run(args);
            }
            catch (Exception e)
            {
                Console.Out.WriteLine(e.ToString());
            }
        }
    }
}
