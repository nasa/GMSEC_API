/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/*! \file gmreq.cs
 * 
 * A C# example demonstrating the GMSEC request/reply functionality.
 * The associated example gmrpl_cs will provide the reply.
 * gmrpl_cs should be run before gmreq_cs
 * 
*/

using System;

namespace gmreq
{
    using Nasa.Gmsec.Net;
    using Gmsec.Example.Common;

    class gmreq : GmsecExample
    {
        private Config config;
        private Connection conn;
        private Message request;
        private Message reply;

        public gmreq()
        {
        }

        private void printUsage()
        {
            Console.Out.WriteLine(
                "\nusage: gmreq_cs connectiontype=<middleware> " +
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
            fd.SetValue("GMREQ");
            result = request.AddField(fd);
            check("AddField(<GMREQ>)", result);

            //Add the Message ID of this Request.  In theory the MSG-ID should be unique,
            // but here it is not for the sake of simplicity.  See API Interface Specification
            // for suggetions on creating a MSG-ID.
            fd.SetName("MSG-ID");
            fd.SetValue("GMREQ-MSG");
            result = request.AddField(fd);
            check("AddField(<GMREQ-MSG>)", result);

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
            result = conn.Request(request, msg_timeout_ms, out reply);
            check("Request", result);

            /* Dump Reply */
            string rplxml;
            result = reply.ToXML(out rplxml);
            check("reply ToXML", result);
            Console.Out.WriteLine("Received Reply:\n" + rplxml);

            /* Destroy the Reply */
            result = conn.DestroyMessage(reply);
            check("DestroyMessage", result);

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
            Console.Out.WriteLine("Middleware Version = " + conn.GetLibraryVersion());

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
                gmreq exProg_gmreq = new gmreq();
                exProg_gmreq.Run(args);
            }
            catch (Exception e)
            {
                Console.Out.WriteLine(e.ToString());
            }
        }
    }
}
