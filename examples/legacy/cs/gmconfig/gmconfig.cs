/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/*! \file gmconfig.cs
*
*  A C# example program demonstrating the use of a ConfigFile.
*  (Loading form a XML file, Creating a Connection, Subscribing & Publishing)
*/

using System;

namespace gmconfig
{
    using Nasa.Gmsec.Net;
    using Gmsec.Example.Common;

    class gmconfig : GmsecExample
    {
        private ConfigFile cfgFile;
        private Connection conn;
        private Message message;

        public gmconfig()
        {
        }

        private bool printUsage(int numberArgs)
        {
            if (numberArgs < 1)
            {
                Console.Out.WriteLine("\nusage: gmconfig_cs <filename>\n");
                return true;
            }
            return false;
        }

        private void Run(string[] args)
        {
            if (printUsage(args.Length))
            {
                return;
            }
            Status result = new Status();
            
            //Create the ConfigFile
            cfgFile = new ConfigFile(args[0]);

            //Load Config
            result = cfgFile.Load();
            check("ConfigFileLoad", result);

            Config config1;
            Config config2;
            result = cfgFile.LookupConfig("config1", out config1);
            check("LookupConfig", result);
            result = cfgFile.LookupConfig("config2", out config2);
            check("LookupConfig", result);
            
            //Dump Configs
            string xml;
            result = config1.ToXML(out xml);
            check("ToXml", result);
            Console.Out.WriteLine("Config 1=>\n " +xml);

            result = config2.ToXML(out xml);
            check("ToXml", result);
            Console.Out.WriteLine("Config 2=>\n " + xml);

            //Create Connection & Connect
            result = ConnectionFactory.Create(config2, out conn);
            check("Create Connection", result);
            result = conn.Connect();
            check("Connect", result);

            //LookUp Subscription & Subscribe
            string pattern1;
            string pattern2;
            result = cfgFile.LookupSubscription("events", out pattern1);
            check("LookUpSubscription", result);
            result = cfgFile.LookupSubscription("custom1", out pattern2);
            check("LookUpSubscription", result);
            Console.Out.WriteLine("Subscribing to: " + pattern1 + "\n");
            result = conn.Subscribe(pattern1);
            check("Subscribe", result);
            Console.Out.WriteLine("Subscribing to: " + pattern2 + "\n");
            result = conn.Subscribe(pattern2);
            check("Subscribe", result);
                
            //Create Message from Config
            result = conn.CreateMessage(out message);
            check("CreateMessage", result);
            result = cfgFile.LookupMessage("msg1", message);
            check("LookUpMessage", result);

            //Dump Message
            result = message.ToXML(out xml);
            check("ToXML", result);
            Console.Out.WriteLine("Message 1=>\n " + xml);
            //Publishing
            Console.Out.WriteLine("Publishing Message...");
            result = conn.Publish(message);
            check("Publish", result);
            
            //Clean UP
            result = conn.DestroyMessage(message);
            check("Destroy Message", result);
            Console.Out.WriteLine("Disconnecting...");
            result = conn.Disconnect();
            check("Disconnecting", result);
            result = ConnectionFactory.Destroy(conn);
            check("Destroying Connection", result);

        }

        static void Main(string[] args)
        {
            try
            {
                gmconfig exProg_gmconfig = new gmconfig();
                exProg_gmconfig.Run(args);
            }
            catch (Exception e)
            {
                Console.Out.WriteLine(e.ToString());
            }
        }
    }
}
