
/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





 
/*! \file gmpub_gmd.cs
 *  This file contains a simple example of SmartSockets GMD publishing
 *  using the C# GMSEC API. For more information about SmartSockets
 *  GMS please refer to the SmartSockets User's Guide.
 */

using System;

namespace gmpub_gmd
{
    using Nasa.Gmsec.Net;
    using Gmsec.Example.Common;

    class GMDCallback : ErrorCallback
    {
        public override void OnError(Connection conn, Message msg, Status status, string event_str)
        {
            Status result;
            GmsecExample myEx = new GmsecExample();
            Console.Out.WriteLine("\n-- GMD Message Failure Notification Recieved --");

            string tmp;
            result = msg.GetSubject( out tmp);
            if (!myEx.checkNoThrow("GetSubject", result))
            {
                return;
            }
            Console.Out.WriteLine("GMD failure: " + tmp + " " + status.Get());

            //Display recieved GMD Failure Message
            string xml;
            result = msg.ToXML(out xml);
            if (myEx.checkNoThrow("ToXML", result))
            {
                if (xml.Length != 0)
                {
                    Console.Out.WriteLine(xml);
                }
            }
        }
    }

    class DispatchCallback : ErrorCallback
    {
        public override void OnError(Connection conn, Message msg, Status status, string event_str)
        {
            Console.Out.WriteLine("-- Entered DispatchCallback::OnError() --");
            Console.Out.WriteLine("Dispatch failure: " + status.Get());
            Console.Out.WriteLine("-- Exited DispatchCallback::OnError() --");
        }
    }

    class gmpub_gmd : GmsecExample
    {
        private Config config;
        private Connection conn;
        private Message message;

        public gmpub_gmd()
        {
        }

        private bool printUsage(int numberArgs)
        {
            int conn_type_length = get(config, "connectionType").Length;
            int mw_id_length = get(config, "mw-id").Length;
            int server_length = get(config, "server").Length;

            if (numberArgs < 2 || (conn_type_length == 0 && mw_id_length == 0)
                || server_length == 0)
            {
                Console.Out.WriteLine("\nUsage: gmpub_cs_gmd "
                    +"connectiontype=<middleware> server=<server>"
                    + "\n\tsubject=<subject name>"
                    + "\n\titerations=<iterations>"
                    + "\n\tinterval_ms=<interval (milliseconds)>"
                    + "\n\tloglevel=<log level>"
                    + "\n\tcfgfile=<config_filename.xml>"
                    + "\n\nFor more information, see API SmartSockets User's Guide\n");
                return true;
            }
            return false;
        }

        private void Run(string[] args)
        {
            config = new Config(args);

            addToConfigFromFile(config);

            if(printUsage(args.Length))
            {
                return;
            }

            Status result = new Status();

            /* Not needed in production, but it allows you to see what
	        options actually get set in SS (as a sanity check)
	        See GMSEC API Users Guide for other settings. */
            config.AddValue("COMMAND_FEEDBACK", "always");

            /* Add the gmdSubject to the connection configuration.
             This subject needs to be static yet unique amongst GMD clients so
             that the RTServer can associate a connection with each client.
             In other words, the GMD subject used here can not be used by any
             other client in the system. */
            config.AddValue("gmdSubject", "GMSEC.GMD_Subject_publisher");

            /* Set the Server mode, so it 'remembers' if you leave
             and come back.  See GMSEC API Users Guide for other settings. */
            config.AddValue("SERVER_DISCONNECT_MODE", "warm");

            /* Turn on ether Memory OR File based GMD */
            //cfg.AddValue("IPC_GMD_TYPE","default");  // File based.
            config.AddValue("IPC_GMD_TYPE", "memory");

            // Output GMSEC API version
            Console.Out.WriteLine(ConnectionFactory.GetAPIVersion());

            // Create the connection
            result = ConnectionFactory.Create(config, out conn);
            check("Error creating connection", result);

            // Connect
            result = conn.Connect();
            check("Error connecting", result);

            GMDCallback gmdcb = new GMDCallback();
            DispatchCallback dispcb = new DispatchCallback();

            Console.Out.WriteLine("Registering Error Callbacks");
            result = conn.RegisterErrorCallback("CONNECTION_DISPATCHER_ERROR", dispcb);
            check("RegisterErrorCallback", result);
            result = conn.RegisterErrorCallback("SS_GMD_ERROR", gmdcb);
            check("REgisterErrorCallback", result);

            Console.Out.WriteLine("Starting AutoDispatcher\n");
            result = conn.StartAutoDispatch();
            check("startAutoDispatch", result);

            Config msgCfg = new Config();

            /* Required to be set on the Message for GMD.  The T_IPC_DELIVERY_ALL setting says to
	        assure delivery to ALL subscribers before sending back an ACK.
	        See GMSEC API Users Guide for other settings. */
            msgCfg.AddValue("SETDELIVERYMODE", "T_IPC_DELIVERY_ALL");

            /* Set Delivery Timeout to 15 seconds.  The Delivery Timeout determines how long the
             publisher is willing to wait for all subscribers to acknowledge the messaage.  It
             also determines the size (in units of seconds) of the resend queue.  In this scenario
             the resend queue would contain 15 seconds worth messages. */
            msgCfg.AddValue("SETDELIVERYTIMEOUT", "15.0");

            result = conn.CreateMessage("GMSEC.TEST.PUBLISH", GMSECMsgKindDefs.PUBLISH, out message,
                msgCfg);
            check("CreateMessage", result);

            uint j = 1;
            Field fd = new Field();
            fd.SetName("sequence number");
            fd.SetValue(j);
            result = message.AddField(fd);
            check("AddField", result);

            //Publish Message
            result = conn.Publish(message);
            check("Publish", result);
            Console.Out.WriteLine("Published sequence-> " + j);
            while (!result.IsError() ||
                (result.GetClass() != GMSECErrorClasses.STATUS_MIDDLEWARE_ERROR &&
                result.GetCode() != GMSECErrorCodes.FIELD_TYPE_MISMATCH))
            {
                j++;
                fd.SetValue(j);
                message.AddField(fd);
                //Publish
                result = conn.Publish(message);
                check("Publish", result);
                Console.Out.WriteLine("Published sequence-> " + j);
                //Sleep a second
                System.Threading.Thread.Sleep(1000);
            }

            //Destroy Message
            result = conn.DestroyMessage(message);
            check("DestroyMessage", result);

            // Disconnect
            result = conn.Disconnect();
            check("Error Disconnecting", result);

            // Destroy the Connection
            result = ConnectionFactory.Destroy(conn);
            check("Error Destroying Connection", result);
            return;
        }

        static void Main(string[] args)
        {
            try
            {
                gmpub_gmd exProg_gmpub_gmd = new gmpub_gmd();
                exProg_gmpub_gmd.Run(args);
            }
            catch (Exception e)
            {
                Console.Out.WriteLine(e.ToString());
            }
        }
    }
}
