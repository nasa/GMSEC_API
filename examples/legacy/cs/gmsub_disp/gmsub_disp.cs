/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/*! \file gmsub_disp.cs
 *  A C# example demonstrating the subscribe capabilities of the
 *  GMSEC API using the Autodispatch routine.
*/

using System;
using System.Collections.Generic;

namespace gmsub_disp
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
            Console.Out.WriteLine("Press Enter to Exit");
        }
    }

    class gmsub_disp : GmsecExample
    {
        private Config config;
        private Connection conn;
        private List<string> subjects;

        public gmsub_disp()
        {
        }

        private void printUsage()
        {
            Console.Out.WriteLine(
                "\nusage: gmsub_cs_disp connectiontype=<middleware> " +
	            "[ <parameter>=<value> ]\n" + 
	            "\n\tNote that the parameter 'connectiontype' is required. " +
	            "\n\tThe rest of other parameters are optional.\n" +
	            "\n\tserver=<server name> " +
	            "(required if middleware is not bolt/MB locally)" +	
	            "\n\tsubject=<subject name>" +
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
            check("Start AutoDispatch", result);

            Console.Out.WriteLine("Press Enter to Exit");

            //Wait for User Input
            Console.ReadKey();

            Console.Out.WriteLine("Stopping AutoDispatch");
            result = conn.StopAutoDispatch();
            check("Stop AutoDispatch", result);

            cleanUp();
        }

        static void Main(string[] args)
        {
            try
            {
                gmsub_disp exProg_gmsub_disp = new gmsub_disp();
                exProg_gmsub_disp.Run(args);
            }
            catch (Exception e)
            {
                Console.Out.WriteLine(e.ToString());
            }
        }
    }
}
