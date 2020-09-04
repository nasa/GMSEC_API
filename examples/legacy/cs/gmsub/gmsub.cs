/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/*! \file gmsub.cs

    This file contains a simple example subscriber using the GMSEC .NET API.
*/

using System;
using System.Diagnostics;
using System.Collections.Generic;

namespace gmsub
{
    using Nasa.Gmsec.Net;
    using Gmsec.Example.Common;

    class gmsub : GmsecExample
    {
        private Config config;
        private Connection conn;
        private Message message;
        private List<string> subjects;

        public gmsub()
        {
        }

        private void printUsage()
        {
            Console.Out.WriteLine(
                "\nusage: gmsub_cs connectiontype=<middleware> " +
	            "[ <parameter>=<value> ]\n" + 
	            "\n\tNote that the parameter 'connectiontype' is required. " +
	            "\n\tThe rest of other parameters are optional.\n" +
	            "\n\tserver=<server name> " +
	            "(required if middleware is not bolt/MB locally)" +	
	            "\n\tsubject=<subject name>" +
	            "\n\tsubject.<N>=<subject name>" +
	            "\n\t\tNote: N must be 1 or greater, and be consecutive." +
	            "\n\t\tFor example: subject.1=GMSEC.A subject.2=GMSEC.B ..." +
	            "\n\titerations=<iterations>" +
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

            //Determine the subjects to listen too
            subjects = determineSubjects(config);
            //Determine how many messages to recieve
            int iterations = get(config, "ITERATIONS", 0);
            if (iterations > 0)
            {
                Console.Out.WriteLine("Waiting for up to " + iterations + " messages");
            }
            //Determine Timeout Periods
            int msg_timeout_ms = get(config, "MSG_TIMEOUT_MS", EX_MSG_TIMEOUT_MS);
            int prog_timeout_s = get(config, "PROG_TIMEOUT_S", EX_PROG_TIMEOUT_S);

            Console.Out.WriteLine("Middleware version = " + conn.GetLibraryVersion());

            //Subscribe
            for (int i = 0; i < subjects.Count; i++)
            {
                Console.Out.WriteLine("Subscribing to "+subjects[i]);
                result = conn.Subscribe(subjects[i]);
                check("Subscribe", result);
            }

            //Wait & Print Out Messages
            bool done = false;
            int count = 0;
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
                    count++;
                    if(iterations > 0 && count >= iterations)
                    {
                        done = true;
                    }

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
                        if(subject.Equals("GMSEC.TERMINATE"))
                        {
                            done = true;
                        }
                    }
                    result = conn.DestroyMessage(message);
                    check("Destroy Message", result);

                }
            }
            cleanUp();
        }

        static void Main(string[] args)
        {
            try
            {
                gmsub exProg_gmsub = new gmsub();
                exProg_gmsub.Run(args);
            }
            catch (Exception e)
            {
                Console.Out.WriteLine(e.ToString());
            }
        }
    }
}
