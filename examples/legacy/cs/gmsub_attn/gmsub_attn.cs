/*
 * Copyright 2007-2014 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * All Rights Reserved.
 */

/*! \file gmsub_attn.cs
    This file contains a simple example subscriber using the GMSEC .NET API.

*/

using System;
using System.Linq;
using System.IO;
using System.Diagnostics;
using System.Collections.Generic;
using System.Configuration;
using System.Management;

using System.Net.Sockets;

namespace gmsub_attn
{
    using Nasa.Gmsec.Net;
    using Gmsec.Example.Common;

    class gmsub_attn : GmsecExample
    {
        private readonly string ATTENTION_DELIMITER_DEFAULT = "***";

        private Config config;
        private Connection conn;
        private Message message;
        private String delimiterString;
        private String logLevel = "INFO";
        private List<string> subjects;
        private Queue<string> includeFields;

        public gmsub_attn()
        {
        }

        private void printUsage()
        {
            Console.Out.WriteLine(
                "\nusage: gmsub_attn_cs connectiontype=<middleware> " +
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
				"\n\tattn-port=<Attention! server port>" +
				"\n\tattn-server=<Attention! server host>" +
				"\n\tinclude-fields=<comma-separated list of field names," +
				"\n\t\tthe content of which will be sent to the Attention!" +
				"\n\t\tserver>" +
				"\n\tdelimiter-string=<Attention! server string delimiter>" +
				"\n\nFor more information, see API User's Guide\n\n");
        }

        private void cleanUp()
        {
            if (conn.IsConnected())
            {
                checkNoThrow("Disconnect", conn.Disconnect());
            }
            checkNoThrow("Destroying Connection", ConnectionFactory.Destroy(conn));
        }

        private void Run(string[] args)
        {
            config = new Config(args);

            addToConfigFromFile(config);

            string strServer, strPort;

            if (isOptionInValid(config, args, true))
            {
                printUsage();
                return;
            }

            Status status = new Status();

            // Output GMSEC API version
            Console.Out.WriteLine(ConnectionFactory.GetAPIVersion());

            // Create the connection
            status = ConnectionFactory.Create(config, out conn);
            check("Error creating connection", status);

            // Connect
            status = conn.Connect();
            check("Error connecting", status);

            // Determine the log level
            config.GetValue("LOGLEVEL", out logLevel);

            //Determine the subjects to listen to
            subjects = determineSubjects(config);

            // Determine the message field names to insert into the delimited string
            // in the case of an overflow situation (>1000 characters)
            includeFields = determineIncludeFields(config);

            delimiterString = determineDelimiterString(config);

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
                Console.Out.WriteLine("Subscribing to " + subjects[i]);
                status = conn.Subscribe(subjects[i]);
                check("Subscribe", status);
            }

            bool done = false;
            int count = 0;
            int intPort = 0;

            Stopwatch stopWatch = new Stopwatch();

            try
            {
                strPort = get(config, "ATTN-PORT", "4999");
                intPort = Convert.ToInt32(strPort);
            }
            catch (FormatException fe)
            {
                Console.WriteLine("FormatException : {0}", fe.ToString());
                Console.WriteLine("Value input for ATTN-PORT is not a valid sequence of digits.");
                Environment.Exit(1);
            }
            catch (OverflowException oe)
            {
                Console.WriteLine("OverflowException : {0}", oe.ToString());
                Console.WriteLine("The number cannot fit in an Int32.");
                Environment.Exit(1);
            }

            strServer = get(config, "ATTN-SERVER", "127.0.0.1");

            int connectionAttempts = 0;

            while (connectionAttempts < 3)
            {
                System.Net.IPHostEntry ipHostInfo = System.Net.Dns.GetHostEntry(strServer);
                System.Net.IPAddress ipAddress = ipHostInfo.AddressList[0];
                System.Net.IPEndPoint ipEndPoint = new System.Net.IPEndPoint(ipAddress, intPort);

                Socket socket = new Socket(ipAddress.AddressFamily, SocketType.Stream, ProtocolType.Tcp);

                try
                {
                    socket.Connect(ipEndPoint);
                }
                catch (ArgumentNullException ae)
                {
                    Console.WriteLine("ArgumentNullException : {0}", ae.ToString());
                    Environment.Exit(1);
                }
                catch (SocketException se)
                {
                    Console.WriteLine("SocketException : {0}", se.ToString());
                    Console.WriteLine("Trying to connect again (Attempt " + ++connectionAttempts + " of 3)", se.ToString());
                    System.Threading.Thread.Sleep(500);
                    //Environment.Exit(1);
                    continue;
                }
                catch (Exception e)
                {
                    Console.WriteLine("Unexpected exception : {0}", e.ToString());
                    Environment.Exit(1);
                }

                connectionAttempts = 0;

                //strServer = ConfigurationManager.AppSettings["ATTNSRV"];
                Console.Out.WriteLine("Connected to Server: " + strServer + ":" + intPort);

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

                    status = conn.GetNextMsg(out message, msg_timeout_ms);

                    if (status.IsError())
                    {
                        //do not exit on timeouts
                        if (status.GetCode() == GMSECErrorCodes.TIMEOUT_OCCURRED)
                        {
                            Console.Out.WriteLine("Timeout Occurred");
                            continue;
                        }
                        else
                        {
                            check("GetNextMsg", status);
                        }
                    }

                    if (message.IsValid())
                    {
                        count++;

                        if (iterations > 0 && count >= iterations)
                        {
                            done = true;
                        }

                        string xml;
                        string subject;
                        string attnArgs;
						
						// Get the subject of the message
                        status = message.GetSubject(out subject);
                        check("GetSubject", status);

                        // Convert the GMSEC message to an XML String
                        status = message.ToXML(out xml);
                        check("ToXML", status);

                        if (xml.Length != 0)
                        {
                            if (logLevel.Equals("DEBUG", StringComparison.CurrentCultureIgnoreCase)
							&& (subject.Equals("GMSEC.GPM.GPM1.MSG.LOG.WDOG", StringComparison.CurrentCultureIgnoreCase)
							|| subject.Equals("GMSEC.GPM.GPM1.MSG.C2CX.LSIMSS.HB", StringComparison.CurrentCultureIgnoreCase)))
                            {
                            	Console.Out.WriteLine("Received Message:\n" + xml);
                            }

                            // Create the delimited string that will be sent to the Attention Generic Agent
                            attnArgs = getAttnArgString(message);

                            /*
                            if (logLevel.Equals("DEBUG", StringComparison.CurrentCultureIgnoreCase))
                            {
                                Console.Out.WriteLine("Sending the string to the Attention! Server:\n" + attnArgs);
                            }
                            */

                            byte[] data = System.Text.Encoding.UTF8.GetBytes(attnArgs);
                            try
                            {
                                int bytesSent = 0;

                                while (bytesSent < data.Length)
                                {
                                    // Blocks until send returns.
                                    int result = socket.Send(data, bytesSent, data.Length - bytesSent, SocketFlags.None);

                                    if (result > 0)
                                    {
                                        bytesSent += result;
                                    }
                                    else if (result < 0)
                                    {
                                        Console.Out.WriteLine("Tried to write more data than was in the buffer!");
                                    }
                                }

                                // Wait for an ack (Operation blocks until data is received, no need for other logic)
                                int bytesRec = socket.Receive(data);
                            }
                            catch (SocketException e)
                            {
                                Console.WriteLine("{0} Error code: {1}.", e.Message, e.ErrorCode);
                                break;
                            }

                            /*
                            //strPort = My.settings.ATTNPRT;
                            Environment.SetEnvironmentVariable("ATTNSRV", strServer, EnvironmentVariableTarget.Process);
                            Environment.SetEnvironmentVariable("ATTNPRT", strPort, EnvironmentVariableTarget.Process);
                            Process attn = new Process();
                            attn.StartInfo.FileName = "attn.exe";
                            attn.StartInfo.Arguments = attnArgs;
                            attn.StartInfo.WindowStyle = System.Diagnostics.ProcessWindowStyle.Hidden;
                            attn.Start();
                            */
                        }
						else
						{
							Console.WriteLine("Unable to get the XML-format of a message with the subject: {0}" + subject);
						}

                        if (subject.Length != 0)
                        {
                            if (subject.Equals("GMSEC.TERMINATE"))
                            {
                                done = true;
                            }
                        }

                        status = conn.DestroyMessage(message);
                        check("Destroy Message", status);
                    }
					else
					{
						Console.WriteLine("Retrieved an invalid message from the GMSEC Bus");
					}
                }
            }

            cleanUp();
        }

        private string getAttnArgString(Message message)
        {
            Field tmpField;
            string tmpFieldValue;
            // 5.0;gpm-auto1|<First field>|3***<Rest of the message fields>
            string attnArgs = "5.0;gpm-auto1|";
            int counter = 0;
            bool firstTime = true;

            foreach (string fieldName in includeFields)
            {

                if (fieldName == "SUBJECT")
                {
                    message.GetSubject(out tmpFieldValue);
                }
                else
                {
                    GMSECTypeDefs fType;
                    message.GetField(fieldName, out tmpField);
                    tmpField.GetType(out fType);

                    switch (fType)
                    {
                        case GMSECTypeDefs.BOOL:
                            bool boolValue;
                            tmpField.GetValue(out boolValue);
                            tmpFieldValue = boolValue.ToString();
                            break;
                        case GMSECTypeDefs.CHAR:
                            sbyte charValue;
                            tmpField.GetValue(out charValue);
                            tmpFieldValue = charValue.ToString();
                            break;
                        case GMSECTypeDefs.F32:
                            float floatValue;
                            tmpField.GetValue(out floatValue);
                            tmpFieldValue = floatValue.ToString();
                            break;
                        case GMSECTypeDefs.F64:
                            double doubleValue;
                            tmpField.GetValue(out doubleValue);
                            tmpFieldValue = doubleValue.ToString();
                            break;
                        case GMSECTypeDefs.I16:
                            short shortValue;
                            tmpField.GetValue(out shortValue);
                            tmpFieldValue = shortValue.ToString();
                            break;
                        case GMSECTypeDefs.I32:
                            int intValue;
                            tmpField.GetValue(out intValue);
                            tmpFieldValue = intValue.ToString();
                            break;
                        case GMSECTypeDefs.I64:
                            long longValue;
                            tmpField.GetValue(out longValue);
                            tmpFieldValue = longValue.ToString();
                            break;
                        case GMSECTypeDefs.STRING:
                            tmpField.GetValue(out tmpFieldValue);
                            break;
                        case GMSECTypeDefs.BIN:
                        case GMSECTypeDefs.COMPOUND:
                        case GMSECTypeDefs.I8:
                        case GMSECTypeDefs.U16:
                        case GMSECTypeDefs.U32:
                        case GMSECTypeDefs.U64:
                        case GMSECTypeDefs.U8:
                        case GMSECTypeDefs.UNSET:
                            tmpFieldValue = "NO " + fieldName;
                            break;
                        // TODO: Unsigned short, int, long, unset, bin, compound
                    }
                    tmpField.GetValue(out tmpFieldValue);
                }

                //if (tmpFieldValue != null || tmpFieldValue.Length != 0)
                if (!String.IsNullOrEmpty(tmpFieldValue))
                {
                    attnArgs += tmpFieldValue;
                }
                else
                {
                    attnArgs += "NO " + fieldName;
                }

                if (firstTime)
                {
                    attnArgs += "|3";
                    firstTime = false;
                }

                if (++counter < includeFields.Count)
                {
                    attnArgs += delimiterString;
                }
            }

            return attnArgs;
        }

        // Parse out the overflow field names from the comma-delimited string
        public Queue<string> determineIncludeFields(Config cfg)
        {
            Queue<string> fields = new Queue<string>();
            string inclFields = get(cfg, "INCLUDE-FIELDS");
            string[] inclFieldsSplit = inclFields.Split(',');

            Console.Out.WriteLine("\nThe following fields will be concatenated:");
            foreach (string fieldName in inclFieldsSplit)
            {
                if (fieldName.Length != 0)
                {
                    Console.Out.WriteLine(fieldName);
                    fields.Enqueue(fieldName);
                }
            }
            Console.Out.WriteLine();

            return fields;
        }

        // Determine the delimiter used in the formatted string
        public string determineDelimiterString(Config cfg)
        {
            string delimiterString = get(cfg, "DELIMITER-STRING");

            if (delimiterString == null)
            {
                delimiterString = ATTENTION_DELIMITER_DEFAULT;
            }

            Console.Out.WriteLine("Delimiter string is set to: " + delimiterString + "\n");
            return delimiterString;
        }

        static void Main(string[] args)
        {
            //if (Properties.Settings.Default.CONSOLE)
            //{
            //}
            //else
            //{
            //    TextWriterTraceListener myWriter = new
            //    TextWriterTraceListener(System.Console.Out);
            //    Debug.Listeners.Add(myWriter);
            //    Debug.WriteLine("Test output 1 ");
            //    Stream myFile = File.Create("output.txt");
            //    TextWriterTraceListener myTextListener = new
            //    TextWriterTraceListener(myFile);
            //    Debug.Listeners.Add(myTextListener);
            //    Debug.WriteLine("Test output 2 ");
            //    Debug.WriteLine("Test output 3 ");
            //    myWriter.Flush();
            //    myWriter.Close();
            //    myFile.Flush();
            //    myFile.Close();
            //}

            try
            {
                gmsub_attn exProg_gmsub_attn = new gmsub_attn();
                exProg_gmsub_attn.Run(args);
            }

            catch (Exception e)
            {
                Console.Out.WriteLine(e.ToString());
            }

            Console.Out.WriteLine("Press <enter> to exit");
            Console.Read();
        }
    }
}

