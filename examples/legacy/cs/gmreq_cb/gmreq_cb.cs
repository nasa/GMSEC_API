/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/*! \file gmreq_cb.cs
 * 
 *  A C# example program demostrating the request/reply functionality of
 *  GMSEC. This example shows the use of callback functionality, which allows
 *  for code to be performed anytime a Message is recieved.
 * 
*/

using System;
using System.Diagnostics;

namespace gmreq_cb
{
    using Nasa.Gmsec.Net;
    using Gmsec.Example.Common;

    class MyReplyCallback : ReplyCallback
    {
        public override void OnReply(Connection conn, Message reqmsg, Message rplmsg)
        {
            Used++;
            Status result;
            string xml;
            result = rplmsg.ToXML(out xml);
            Console.Out.WriteLine("[MyReplyCallback::OnReply] Recieved Reply:\n" + xml);
        }
        public override void OnError(Connection conn, Message message, Status result, string eventStr)
        {
            Console.Out.WriteLine("[MyReplyCallback::OnError] Status=" + result.Get()
                + " Event=" + eventStr);
        }
        public int Used { get; set; }
    }

    class gmreq_cb : GmsecExample
    {
        private Config config;
        private Connection conn;
        private Message request;

        public gmreq_cb()
        {
        }

        private void printUsage()
        {
            Console.Out.WriteLine(
                "\nusage: gmreq_cs_cb connectiontype=<middleware> " +
	            "[ <parameter>=<value> ]\n" +
                "\n\tNote that the parameter 'connectiontype' is required. " + 
                "\n\tThe rest of other parameters are optional.\n" +
                "\n\tserver=<server name> " +
                "(required if middleware is not bolt/MB locally)" +
                "\n\tsubject=<subject name>" +
                "\n\tmsg_timeout_ms=<timeout period (milliseconds)>" +
                "\n\tloglevel=<log level>" +
                "\n\tcfgfile=<config_filename.xml>" +
	            "\n\nFor more information, see API User's Guide\n");
        }

        private void cleanUp()
        {
            if(conn.IsConnected())
            {
                checkNoThrow("Disconnect", conn.Disconnect());
            }
            checkNoThrow("Destroying Connection", ConnectionFactory.Destroy(conn));
        }

        private void Request(int msg_timeout_ms, string subject)
        {
            Status result;
            Field fd = new Field();

            result = conn.CreateMessage(subject, GMSECMsgKindDefs.REQUEST, out request);
            check("CreateMessage", result);

            //Add Fields
            fd.SetName("QUESTION");
            fd.SetValue("Does the request/reply functionality still work?");
            result = request.AddField(fd);
            check("AddField(<GMSEC_STR>)", result);

            //Add the name of the component making the Request.
            fd.SetName("COMPONENT");
            fd.SetValue("gmreq_cb");
            result = request.AddField(fd);
            check("AddField(<gmreq_cb>)", result);

            //Add the Message ID of this Request.  In theory the MSG-ID should be unique,
            // but here it is not for the sake of simplicity.  See API Interface Specification
            // for suggetions on creating a MSG-ID.
            fd.SetName("MSG-ID");
            fd.SetValue("gmreq_cb-MSG");
            result = request.AddField(fd);
            check("AddField(<gmreq_cb-MSG>)", result);

            if (config.CheckBoolValue("ENCRYPT", false))
            {
                fd.SetName("SEC-ENCRYPT");
                fd.SetValue((int)GMSECBooleanDefs.GMSEC_TRUE_Net);
                result = request.AddField(fd);
                check("AddField(SEC-ENCRYPT)", result);
            }

            /* Dump Request */
            string reqxml;
            result = request.ToXML(out reqxml);
            check("request ToXML", result);
            Console.Out.WriteLine("Requesting:\n" + reqxml);

            /* Send Request Message */
            MyReplyCallback rcb = new MyReplyCallback();
            rcb.Used = 0;
            result = conn.Request(request, msg_timeout_ms, rcb);
            check("Request", result);

            Stopwatch stopWatch = new Stopwatch();
            while (rcb.Used == 0)
            {
                System.Threading.Thread.Sleep(100);
                if (msg_timeout_ms != (int)GMSECWaitDefs.WAIT_FOREVER)
                {
                    if (!stopWatch.IsRunning)
                    {
                        stopWatch.Start();
                    }
                    else if (stopWatch.ElapsedMilliseconds >= (long) msg_timeout_ms)
                    {
                        stopWatch.Stop();
                        break;
                    }
                }//else Wait forever
            }

            if (rcb.Used == 1)
            {
                Console.Out.WriteLine("Responce Recieved");
            }
            else //Used == 0
            {
                Console.Out.WriteLine("No responce Recieved");
            }

            /* Destroy the Request */
            result = conn.DestroyMessage(request);
            check("DestroyMessage", result);
        }

        private void Run(string[] args)
        {
            config = new Config(args);

            addToConfigFromFile(config);

            if (isOptionInValid(config, args, false))
            {
                printUsage();
                return;
            }
            Status result = new Status();

            // Output GMSEC API version
            Console.Out.WriteLine(ConnectionFactory.GetAPIVersion());

            // Create the connection
            result = ConnectionFactory.Create(config, out conn);
            check("Error creating connection", result);

            // Connect
            result = conn.Connect();
            check("Error connecting", result);
            //Get CommandLine Info
            string subject = get(config, "SUBJECT", "GMSEC.MISSION.SAT_ID.REQ.CMD.GMRPL");
            int msg_timeout_ms = get(config, "MSG_TIMEOUT_MS", EX_MSG_TIMEOUT_MS);

            //OutPut MiddlewareVersion
            Console.Out.WriteLine("Middleware version = " + conn.GetLibraryVersion());

            //OutPut Info
            Console.Out.WriteLine("Using Subject '" + subject + "'");

            //Request
            Request(msg_timeout_ms, subject);

            cleanUp();
        }

        static void Main(string[] args)
        {
            try
            {
                gmreq_cb exProg_gmreq_cb = new gmreq_cb();
                exProg_gmreq_cb.Run(args);
            }
            catch (Exception e)
            {
                Console.Out.WriteLine(e.ToString());
            }
        }
    }
}
