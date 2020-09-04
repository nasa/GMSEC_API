/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/*! \file gmrpl.cs
 *
 * A C# example demonstrating the request/reply functionality.
 * The supplied gmreq_cs will provide the request.
 * gmrpl_cs  should be started before gmreq_cs.
*/

using System;
using System.Diagnostics;
using System.Collections.Generic;

namespace gmrpl
{
    using Nasa.Gmsec.Net;
    using Gmsec.Example.Common;

    class gmrpl : GmsecExample
    {
        private Config config;
        private Connection conn;
        private Message message;
        private Message reply;
        private List<string> subjects;


        public gmrpl()
        {
        }

        private void printUsage()
        {
            Console.Out.WriteLine(
                "\nusage: gmrpl_cs connectiontype=<middleware> " +
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
            Console.Out.WriteLine("Middleware Version = "+conn.GetLibraryVersion());

            //Subscribe
            subjects = determineSubjects(config);
            for (int i = 0; i < subjects.Count; i++)
            {
                Console.Out.WriteLine("Subscribing to " + subjects[i]);
                result = conn.Subscribe(subjects[i]);
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
                    else if (stopWatch.ElapsedMilliseconds >= (long)(prog_timeout_s * 1000))
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
                    string xml;
                    result = message.ToXML(out xml);
                    check("ToXML", result);
                    if (xml.Length != 0)
                    {
                        Console.Out.WriteLine("Recieved\n" + xml);
                    }
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
                    //Check For Request
                    GMSECMsgKindDefs kind;
                    message.GetKind(out kind);
                    if (kind == GMSECMsgKindDefs.REQUEST)
                    {
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
                        check("CreateMessage", result);
                        //Add Fields
                        Field fd = new Field();
                        fd.SetName("COMPONENT");
                        fd.SetValue("GMRPL");
                        result = reply.AddField(fd);
                        check("AddField(COMPONENT)", result);

                        fd.SetName("ANSWER");
                        fd.SetValue("Sure looks like it");
                        result = reply.AddField(fd);
                        check("AddField(ANSWER)", result);
                        if (config.CheckBoolValue("ENCRYPT", false))
                        {
                            fd.SetName("SEC_ENCRYPT");
                            fd.SetValue(GMSECBooleanDefs.GMSEC_TRUE_Net.ToString());
                            result = reply.AddField(fd);
                            check("AddField(SEC-ENCRYPT)", result);
                        }
                        //Dump Reply
                        result = reply.ToXML(out xml);
                        check("ToXML", result);
                        Console.Out.WriteLine("Prepared Reply:\n" + xml);
                        //Send Reply
                        result = conn.Reply(message, reply);
                        check("Reply Sent", result);
                    }
                    //Destroy messages
                    result = conn.DestroyMessage(message);
                    check("DestroyMessage", result);
                    result = conn.DestroyMessage(reply);
                    check("DestroyMessage", result);
                }

            }

            cleanUp();
        }

        static void Main(string[] args)
        {
            try
            {
                gmrpl exProg_gmrpl = new gmrpl();
                exProg_gmrpl.Run(args);
            }
            catch (Exception e)
            {
                Console.Out.WriteLine(e.ToString());
            }
        }
    }
}
