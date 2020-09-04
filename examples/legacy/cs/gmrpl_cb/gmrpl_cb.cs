/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/*! \file gmrpl_cb.cs
 *  A C# example demonstrating the reply/request abilities of GMSEC.
 *  This Reply example shows the use of callback functionality, which
 *  allows for code to be performed anytime a Message is recieved
*/

using System;
using System.Diagnostics;
using System.Collections.Generic;

namespace gmrpl_cb
{
    using Nasa.Gmsec.Net;
    using Gmsec.Example.Common;

    class RequestCallback : Callback
    {
        public override void OnMessage(Connection conn, Message message)
        {
            //Check For Request
            GMSECMsgKindDefs kind;
            message.GetKind(out kind);
            if (kind == GMSECMsgKindDefs.REQUEST)
            {
                Message reply;
                Status result;
                string xml;
                GmsecExample myEx = new GmsecExample();

                //Construct Reply Subject
                Field msg_fld;
                message.GetField("COMPONENT", out msg_fld);
                string comp_name;
                msg_fld.GetValue(out comp_name);

                Field id_fld;
                message.GetField("MSG_ID", out id_fld);
                string msg_id;
                id_fld.GetValue(out msg_id);

                string statusCode = "1";
                string subjectReply = "GMSEC.MISSION.SAT_ID.RESP." + comp_name + "." +
                    msg_id + "." + statusCode;
                //Create Reply
                result = conn.CreateMessage(subjectReply, GMSECMsgKindDefs.REPLY, out reply);
                myEx.check("CreateMessage", result);
                
                //Add Fields
                Field fd = new Field();
                fd.SetName("COMPONENT");
                fd.SetValue("gmrpl_cb");
                result = reply.AddField(fd);
                myEx.check("AddField(COMPONENT)", result);

                fd.SetName("ANSWER");
                fd.SetValue("Sure looks like it");
                result = reply.AddField(fd);
                myEx.check("AddField(ANSWER)", result);
                
                //Dump Reply
                result = reply.ToXML(out xml);
                myEx.check("ToXML", result);
                Console.Out.WriteLine("[RequestCallback::OnMessage] Sending Reply:\n" + xml);
                //Send Reply
                result = conn.Reply(message, reply);
                myEx.check("Reply Sent", result);
            }
        }
    }

    class gmrpl_cb : GmsecExample
    {
        private Config config;
        private Connection conn;
        private Message message;
        private List<string> subjects;


        public gmrpl_cb()
        {
        }

        private void printUsage()
        {
            Console.Out.WriteLine(
                "\nusage: gmrpl_cs_cb connectiontype=<middleware> " +
	            "[ <parameter>=<value> ]\n" +
                "\n\tNote that the parameter 'connectiontype' is required. " + 
                "\n\tThe rest of other parameters are optional.\n" +
                "\n\tserver=<server name> " +
                "(required if middleware is not bolt/MB locally)" +
                "\n\tsubject=<subject name>" +
                "\n\tmsg_timeout_ms=<timeout period (milliseconds)>" +
                "\n\tprog_timeout_s=<timeout period (seconds)>" +
                "\n\t\tNote: msg_timeout_ms must be defined to use prog_timeout_s" +
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

            if (isOptionInValid(config, args, true))
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
            int msg_timeout_ms = get(config, "MSG_TIMEOUT_MS", EX_MSG_TIMEOUT_MS);
            int prog_timeout_s = get(config, "PROG_TIMEOUT_S", EX_PROG_TIMEOUT_S);

            //OutPut MiddlewareVersion
            Console.Out.WriteLine("Middleware version = " + conn.GetLibraryVersion());

            //Subscribe
            subjects = determineSubjects(config);
            RequestCallback rcb = new RequestCallback();
            for (int i = 0; i < subjects.Count; i++)
            {
                Console.Out.WriteLine("Subscribing to " + subjects[i]);
                result = conn.Subscribe(subjects[i], rcb);
                check("Subscribe", result);
            }

            //Wait for messages
            bool done = false;
            Stopwatch stopWatch = new Stopwatch();
            while (!done)
            {
                if (prog_timeout_s != (int)GMSECWaitDefs.WAIT_FOREVER)
                {
                    if (!stopWatch.IsRunning)
                    {
                        stopWatch.Start();
                    }
                    else if(stopWatch.ElapsedMilliseconds >= (long)(prog_timeout_s*1000))
                    {
                        Console.Out.WriteLine("Program Timeout Reached!");
                        stopWatch.Stop();
                        break;
                    }
                }

                result = conn.GetNextMsg(out message, msg_timeout_ms);

                if (result.IsError())
                {
                    //do not exit on timeouts
                    if (result.GetCode() == GMSECErrorCodes.TIMEOUT_OCCURRED)
                    {
                        Console.Out.WriteLine("Timeout Occurred");
                        continue;
                    }
                    else
                    {
                        check("GetNextMsg", result);
                    }
                }
                if (message.IsValid())
                {
                    //Dump Message
                    string xml;
                    result = message.ToXML(out xml);
                    check("ToXML", result);
                    if (xml.Length != 0)
                    {
                        Console.Out.WriteLine("Recieved:\n" + xml);
                    }

                    result = conn.DispatchMsg(message);
                    check("DispatchMsg", result);

                    string subject;
                    result = message.GetSubject(out subject);
                    check("GetSubject", result);
                    if (subject.Length != 0)
                    {
                        if (subject.Equals("GMSEC.TERMINATE"))
                        {
                            done = true;
                        }
                    }

                    //Destroy messages
                    result = conn.DestroyMessage(message);
                    check("DestroyMessage", result);
                }
            }
            cleanUp();
        }

        static void Main(string[] args)
        {
            try
            {
                gmrpl_cb exProg_gmrpl_cb = new gmrpl_cb();
                exProg_gmrpl_cb.Run(args);
            }
            catch (Exception e)
            {
                Console.Out.WriteLine(e.ToString());
            }
        }
    }
}
