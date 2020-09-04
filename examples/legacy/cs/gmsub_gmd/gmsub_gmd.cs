/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/*! \file gmsub_gmd.cs
 * 
 * This file contains a simple example of SmartSockets GMD subscribing
 * using the C# GMSEC API. For more information about SmartSockets GMD
 * please refer to the SmartSockets User's Guide.
 * 
*/

using System;

namespace gmsub_gmd
{
    using Nasa.Gmsec.Net;
    using Gmsec.Example.Common;

    class gmsub_gmd : GmsecExample
    {
        private Config config;
        private Connection conn;
        private Message message;

        public gmsub_gmd()
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
                Console.Out.WriteLine("\nUsage: gmsub_cs_gmd "
                    + "connectiontype=<middleware> server=<server>"
                    + " [ msg_timeout_ms=<timeout (ms)> ]"
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

            if (printUsage(args.Length))
            {
                return;
            }
            Status result = new Status();

            /* Not needed in production, but it allows you to see what
	        options actually get set in SS (as a sanity check)
	        See GMSEC API Users Guide for other settings. */
            config.AddValue("COMMAND_FEEDBACK", "always");

            /* Add the gmdSubject to the connection configuration. */
            config.AddValue("gmdSubject", "GMSEC.GMD_Subject_subscriber");

            /* Set the Server mode, so it 'remembers' if you leave
             and come back.  See GMSEC API Users Guide for other settings. */
            config.AddValue("SERVER_DISCONNECT_MODE", "warm");

            // Output GMSEC API version
            Console.Out.WriteLine(ConnectionFactory.GetAPIVersion());

            int msg_timeout_ms = get(config, "MSG_TIMEOUT_MS", EX_MSG_TIMEOUT_MS);

            // Create the connection
            result = ConnectionFactory.Create(config, out conn);
            check("Error creating connection", result);

            // Connect
            result = conn.Connect();
            check("Error connecting", result);

            //Subscribe
            Console.Out.WriteLine("Subscribing->GMSEC.TEST.PUBLISH\n");
            result = conn.Subscribe("GMSEC.TEST.PUBLISH");
            check("Subscribe", result);

            Field fd = new Field();
            
            //Listen
            bool done = false;
            while (!done)
            {
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

                uint seq;
                result = message.GetField("sequence number", out fd);
                check("GetField", result);

                if (GMSECErrorClasses.STATUS_NO_ERROR == result.GetClass())
                {
                    fd.GetValue(out seq);
                    Console.Out.WriteLine("Sequence number-> " + seq);
                }
                //Must destroy the incoming message to acknowledge the GMD message
                //was recieved
                result = conn.DestroyMessage(message);
                check("DestroyMessage", result);
            }
            return;
        }

        static void Main(string[] args)
        {
            try
            {
                gmsub_gmd exProg_gmsub_gmd = new gmsub_gmd();
                exProg_gmsub_gmd.Run(args);
            }
            catch (Exception e)
            {
                Console.Out.WriteLine(e.ToString());
            }
        }
    }
}
