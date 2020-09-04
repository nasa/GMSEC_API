
/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





 
/*! \file gmpub.cs
    
    This file contains a simple example publisher using the GMSEC.NET API.
*/

using System;

namespace gmpub
{
    using Nasa.Gmsec.Net;
    using Gmsec.Example.Common;

    class gmpub : GmsecExample
    {
        private Config config;
        private Connection conn;

        public gmpub()
        {
        }

        private void printUsage()
        {
            Console.Out.WriteLine(
                "\nusage: gmpub_cs connectiontype=<middleware> " +
                "[ <parameter>=<value> ]\n" +
                "\n\tNote that the parameter 'connectiontype' is required. " +
                "\n\tThe rest of other parameters are optional.\n" +
                "\n\tserver=<server name> " +
                "(required if middleware is not bolt/MB locally)" +
                "\n\tsubject=<subject name>" +
                "\n\titerations=<iterations>" +
                "\n\tinterval_ms=<interval (milliseconds)>" +
                "\n\tloglevel=<log level>" +
                "\n\tcfgfile=<config_filename.xml>" +
                "\n\nFor more information, see API User's Guide\n");
        }
        private void Publish(int iteration, String subject)
        {
            Status result = new Status();
            Message msg;
            result = conn.CreateMessage(subject, GMSECMsgKindDefs.PUBLISH, out msg);
            check("Error Creating Message", result);

            int value = iteration + 1;

            // Add Fields
            Field fd = new Field();

            fd.SetName("C");
            fd.SetValue((SByte)'c');
            msg.AddField(fd);

            fd.SetName("T");
            fd.SetValue(true);
            msg.AddField(fd);

            fd.SetName("F");
            fd.SetValue(false);
            msg.AddField(fd);

            fd.SetName("J");
            fd.SetValue((Int16) value);
            msg.AddField(fd);

            fd.SetName("K");
            fd.SetValue((UInt16) value);
            msg.AddField(fd);

            fd.SetName("I");
            fd.SetValue((Int32) value);
            msg.AddField(fd);

            fd.SetName("COUNT");
            fd.SetValue((Int32) iteration);
            msg.AddField(fd);

            fd.SetName("U");
            fd.SetValue((UInt32) value);
            msg.AddField(fd);

            fd.SetName("S");
            fd.SetValue("This is a test");
            msg.AddField(fd);

            fd.SetName("E");
            fd.SetValue((Single)(1 + 1.0 / value));
            msg.AddField(fd);

            fd.SetName("D");
            fd.SetValue((Double)(1 + 1.0 / value));
            msg.AddField(fd);

            string str = "JLMNOPQ";
            byte[] bytes = System.Text.Encoding.ASCII.GetBytes(str);
            fd.SetName("X");
            fd.SetValue(bytes, (uint) bytes.Length);
            msg.AddField(fd);

            if (config.CheckBoolValue("ENCRYPT", false)) {

                fd.SetName("SEC-ENCRYPT");
                fd.SetValue(true);
                msg.AddField(fd);

            }

            
            // Dump Message
            string xml = null;
            msg.ToXML(out xml);
            Console.WriteLine("Published GMSEC Message::" + xml);

            // Publish Message
            result = conn.Publish(msg);
            Console.WriteLine("Published!");
            check("Error Publishing Message", result);
            // Destroy the Message
            result = conn.DestroyMessage(msg);
            check("Error Destroying Message", result);
            return;
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

            //Get Command Line Information
            String subject = get(config, "SUBJECT", "GMSEC.TEST.PUBLISH");
            int iterations = get(config, "ITERATIONS", 1);
            int interval_ms = get(config, "INTERVAL_MS", 1000);
            
            // Output middleware version & Info
            Console.Out.WriteLine("Middleware version = " + conn.GetLibraryVersion());
            Console.Out.WriteLine("Using Subject '" + subject + "'");
            Console.Out.WriteLine("Publishing " + iterations + " message(s)");
            
            if(iterations > 1)
            {
                Console.Out.WriteLine("Publishing Interval "+interval_ms+" [ms]");
            }
            
            for(int i = 0; i < iterations; i++)
            {
                Publish(i, subject);
                if(i < iterations - 1)
                {
                    System.Threading.Thread.Sleep(interval_ms);
                }
            }
            
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
                gmpub exProg_gmpub = new gmpub();
                exProg_gmpub.Run(args);
            }
            catch (Exception e)
            {
                Console.Out.WriteLine(e.ToString());
            }
        }
    }
}
