/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/*! \file gmsub_disp_rr.cs
 *  A C# example subscriber using the GMSEC API dispatcher. The callback
 *  is registered with the connection. A seperate thread, transmits a request
 *  and waits for a reply. THe Dispatcher is then started, which launches its
 *  own thread.
*/

using System;
using System.Diagnostics;
using System.Collections.Generic;

namespace gmsub_disp_rr
{
    using Nasa.Gmsec.Net;
    using Gmsec.Example.Common;

    class PublishCallback : Callback
    {
        public override void OnMessage(Connection conn, Message message)
        {
            Status result;
            string xml;
            result = message.ToXML(out xml);
            Console.Out.WriteLine("[PublishCallback::OnMessage] Received=>\n" + xml);
        }
    }

    class MyReqReplyCallback : ReplyCallback
    {
        public override void OnReply(Connection conn, Message reqmsg, Message rplmsg)
        {
            Status result;
            string xml;
            result = rplmsg.ToXML(out xml);
            Console.Out.WriteLine("[ReplyCallback::OnReply] Replied=>\n" + xml);
        }
        public override void OnError(Connection conn, Message message, Status result, string eventStr)
        {
            Console.Out.WriteLine("[ErrorCallback:OnError] Status=" + result.Get()
                + " Event="+eventStr);
        }
    }

    class gmsub_disp_rr : GmsecExample
    {
        private Config config;
        private Connection conn;
        private Message reqmsg;
        private List<string> subjects;

        public gmsub_disp_rr()
        {
        }

        private void printUsage()
        {
            Console.Out.WriteLine(
                "\nusage: gmsub_cs_disp_rr connectiontype=<middleware> " +
	            "[ <parameter>=<value> ]\n" + 
	            "\n\tNote that the parameter 'connectiontype' is required. " +
	            "\n\tThe rest of other parameters are optional.\n" +
	            "\n\tserver=<server name> " +
	            "(required if middleware is not bolt/MB locally)" +	
	            "\n\tsubject=<subject name>" +
	            "\n\tmsg_timeout_ms=<timeout period (milliseconds)>" +
	            "\n\tprog_timeout_s=<timeout period (seconds)>" +
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

            result = config.AddValue("isThreaded", "true");
            check("AddValue", result);

            // Output GMSEC API version
            Console.Out.WriteLine(ConnectionFactory.GetAPIVersion());

            // Create the connection
            result = ConnectionFactory.Create(config, out conn);
            check("Error creating connection", result);

            // Connect
            result = conn.Connect();
            check("Error connecting", result);

            //Determine the subjects to listen too
            subjects = determineSubjects(config);
            //Determine Timeout Periods
            int msg_timeout_ms = get(config, "MSG_TIMEOUT_MS", EX_MSG_TIMEOUT_MS);
            int prog_timeout_s = get(config, "PROG_TIMEOUT_S", EX_PROG_TIMEOUT_S);

            Console.Out.WriteLine("Middleware version = " + conn.GetLibraryVersion());

            //Subscribe
            PublishCallback cb = new PublishCallback();
            for (int i = 0; i < subjects.Count; i++)
            {
                Console.Out.WriteLine("Subscribing to "+subjects[i]);
                result = conn.Subscribe(subjects[i], cb);
                check("Subscribe", result);
            }

            Console.Out.WriteLine("Starting AutoDispatch");
            result = conn.StartAutoDispatch();
            check("StartAutoDispatch", result);

            string subject = "GMSEC.TEST.PUBLISH.NOREPLY";
            if (subjects[0] != "GMSEC.>")
            {
                subject = subjects[0];
            }

            //Console.Out.WriteLine("Creating a Request Message");
            result = conn.CreateMessage(subject, GMSECMsgKindDefs.REQUEST, out reqmsg);
            check("Creating Request Message", result);

            MyReqReplyCallback rcb = new MyReqReplyCallback();

            //Console.Out.WriteLine("Registering error callback");
            result = conn.RegisterErrorCallback("CONNECTION_REQUEST_TIMEOUT", rcb);
            check("Register Error Callback", result);

            Console.Out.WriteLine("Sending a Request\n");
            result = conn.Request(reqmsg, msg_timeout_ms, rcb);
            check("Request", result);

            if (prog_timeout_s == (int)GMSECWaitDefs.WAIT_FOREVER)
            {
                Console.Out.WriteLine("Press the Enter Key to Exit");
                Console.ReadKey();
            }
            else
            {
                System.Threading.Thread.Sleep(prog_timeout_s * 1000);
                Console.Out.WriteLine("Program Timeout Reached!");
            }

            Console.Out.WriteLine("Stopping AutoDispatch");
            result = conn.StopAutoDispatch();
            check("StopAutoDispatch", result);

            cleanUp();
        }

        static void Main(string[] args)
        {
            try
            {
                gmsub_disp_rr exProg_gmsub_disp_rr = new gmsub_disp_rr();
                exProg_gmsub_disp_rr.Run(args);
            }
            catch (Exception e)
            {
                Console.Out.WriteLine(e.ToString());
            }
        }
    }
}
