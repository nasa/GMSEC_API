
/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





 
/*! \file Recorder.cs
 *  A simple application built with the C# GMSEC API. This
 *  application demonstrates several key components within the GMSEC API
 *  including loading from an xml file, producing a heartbeat, and publishing
*/

using System;

namespace Recorder
{
    using Nasa.Gmsec.Net;
    using Gmsec.Example.Common;

    class LogCallback : Callback
    {
        public override void OnMessage(Connection conn, Message msg)
        {
            Status result;
            string xml;
            result = msg.ToXML(out xml);
            Console.Out.WriteLine(xml);

        }
    }

    class Recorder : GmsecExample
    {
        private ConfigFile cfgFile;
        private Status result;
        private Connection conn;

        public Recorder()
        {
        }

        private void printUsage()
        {
            Console.Out.WriteLine("usage: Recorder <filename>\n");
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
            string configValue;
            string tempSubject;
            //Config Variables
            short globalHeartbeatCount = 0;
            short heartbeatHolder = 0;
            short heartbeatCountdown = 0;
            short loopCountdown = 0;
            short updateRate = 0;

            //Subscription Callback
            LogCallback cb = new LogCallback();

            //Load Config File
            result = cfgFile.Load();
            check("ConfigFileLoad", result);

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
            result = programConfig.GetValue("update-rate", out configValue);
            check("GetValue", result);
            updateRate = Convert.ToInt16(configValue);
            result = programConfig.GetValue("loop-time", out configValue);
            check("GetValue", result);
            loopCountdown = Convert.ToInt16(configValue);

            //Create Subscriptions from subscriptions in configFile
            result = cfgFile.LookupSubscription("RECEIVE-LOG", out tempSubject);
            check("LookUpSubscription", result);
            result = conn.Subscribe(tempSubject, cb);
            check("Subscribe", result);

            result = cfgFile.LookupSubscription("SEND-LOG", out tempSubject);
            check("LookUpSubscription", result);
            result = conn.Subscribe(tempSubject, cb);
            check("Subscribe", result);

            //Create a generic message container for the heartbeat message
            result = conn.CreateMessage(out heartbeatMessage);
            check("CreateMessage", result);

            //Load Heartbeat Definition
            result = cfgFile.LookupMessage("C2CX-HEARTBEAT-REC", heartbeatMessage);
            check("LookUpMessage", result);

            //Obtain publish rate field from heartbeat message
            result = heartbeatMessage.GetField("PUB-RATE", out tempField);
            if (!result.IsError())
            {
                result = tempField.GetValue(out heartbeatHolder);
                check("GetValue", result);
            }
            else
            {
                //Default Value
                heartbeatHolder = 30;
            }

            //Ouput some Program Info
            Console.Out.WriteLine("Publishing for " + loopCountdown + " seconds.");
            Console.Out.WriteLine("Publishing Heartbeat Messages every "+heartbeatHolder
                +" seconds.");

            result = conn.StartAutoDispatch();
            check("Starting AutoDispatch", result);
            Console.Out.WriteLine("Start Time: " + DateTime.Now.ToString());

            //Publish Loop
            for (; loopCountdown > 0; loopCountdown-- )
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
                Recorder appRecorder = new Recorder();
                appRecorder.Run(args);
            }
            catch (Exception e)
            {
                Console.Out.WriteLine(e.ToString());
            }
        }
    }
}
