/*
 * Copyright 2007-2013 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * All Rights Reserved.
 */


/*! \file Example.cs 
 *   This file contains a GMSECExample class, which contains common
 *  functionality found in the C# example programs.
*/

namespace Gmsec.Example.Common
{
    using System;
    using System.IO;
    using System.Collections.Generic;
    using Nasa.Gmsec.Net;
    class GmsecException : Exception
    {
        public GmsecException(string tag)
            :base(tag)
        {
        }
    }
    public class GmsecExample
    {

        public const int EX_MSG_TIMEOUT_MS = (int)GMSECWaitDefs.WAIT_FOREVER;
        public const int EX_PROG_TIMEOUT_S = (int)GMSECWaitDefs.WAIT_FOREVER;

        public void addToConfigFromFile(Config config)
        {
                string cfgfilename = get(config, "CFGFILE");
		// This assumes, now, that there is only _one_
		// CFGFILE=filename.xml arg specified.

		string loglevel = get(config, "LOGLEVEL");

                if (cfgfilename != null)
                {
                    if (File.Exists(cfgfilename))
                    {
                        try
                        {    
                            string file_text_contents =
                              System.IO.File.ReadAllText(cfgfilename);

                            Config config_from_file = new Config();

                            config_from_file.FromXML(file_text_contents);

                            string name  = null;
                            string value = null;

                            //
                            // Read all the config file name, value pairs
                            // and add them to the current base
                            // configuration (which can have its
                            // (name, value) pairs from both command-line
                            // pairs and from the input current config
                            // file that was read).
                            //
                            Status status = config_from_file.GetFirst(out name,  out value);

                            while (!status.IsError())
                            {
                                if (loglevel.Equals("DEBUG", StringComparison.OrdinalIgnoreCase))
                                {
                                    Console.Out.WriteLine("[DEBUG]:  Adding ("
                                                          + name + ", " + value
                                                          + ") from config file "
                                                          + cfgfilename);
                                }

                                config.AddValue(name, value);

                                status = config_from_file.GetNext(out name, out value);
                            }
                        }
                        catch (Exception e)
                        {
                            if (loglevel.Equals("WARNING", StringComparison.OrdinalIgnoreCase) ||
                                loglevel.Equals("INFO", StringComparison.OrdinalIgnoreCase) ||
                                loglevel.Equals("VERBOSE", StringComparison.OrdinalIgnoreCase) ||
                                loglevel.Equals("DEBUG", StringComparison.OrdinalIgnoreCase))
                            {
                                Console.Out.WriteLine("[WARNING]:  Error reading file "
                                                      + cfgfilename + ":  {0}",
                                                      e);
                            }
                        }
                    }
                    else
                    {
                        if (loglevel.Equals("WARNING", StringComparison.OrdinalIgnoreCase) ||
                            loglevel.Equals("INFO", StringComparison.OrdinalIgnoreCase) ||
                            loglevel.Equals("VERBOSE", StringComparison.OrdinalIgnoreCase) ||
                            loglevel.Equals("DEBUG", StringComparison.OrdinalIgnoreCase))
                        {
                            Console.Out.WriteLine("[WARNING]:  Non-valid-filepath config argument "
                                                  + cfgfilename
                                                  + " seen; ignoring it.");
                        }
                    }

                }// if there is a config file argument at all

        }

        //Are the user arguments Valid
        public bool isOptionInValid(Config cfg, string[] args, bool checkProgTimeout)
        {
            if (checkProgTimeout)  //must define msg_timeout for prog_timeout
            {
                if (get(cfg, "MSG_TIMEOUT_MS").Length == 0
                        && get(cfg, "PROG_TIMEOUT_S").Length > 0)
                {
                    return true;
                }
            }
            int conn_type_length = get(cfg, "connectiontype").Length;
            int mw_id_length = get(cfg, "mw-id").Length;

            if (args.Length < 1 || (conn_type_length == 0 && mw_id_length == 0))
            {
                return true;
            }
            return false;
        }

        public bool checkNoThrow(string tag, Status status)
        {
            if (!status.IsError())
            {
                return true;
            }
            Console.Out.WriteLine("[WARNING]"+tag +": " + status.Get());
            return false;
        }

        public bool check(string tag, Status status)
        {
            if (!status.IsError())
            {
                return true;
            }
            throw new GmsecException(tag + ": " + status.Get());
        }

        public bool check(string tag, bool condition)
        {
            if (!condition)
            {
                throw new GmsecException(tag);
            }
            return true;
        }

        public string get(Config cfg, string key)
        {
            string value;
            Status status;
            status = cfg.GetValue(key, out value);
            if (!status.IsError() && value.Length != 0)
            {
                return value;
            }
            value = "";
            return value;
        }

        public string get(Config cfg, string key, string xdefault)
        {
            string value = get(cfg, key);
            if (value.Length == 0)
            {
                value = xdefault;
            }
            return value;
        }

        public int get(Config cfg, string key, int xdefault)
        {
            string s = get(cfg, key);
            if (s.Length != 0)
            {
                int val;
                bool success = Int32.TryParse(s, out val);
                if (success)
                {
                    return val;
                }
            }
            return xdefault;
        }

        public double get(Config cfg, string key, double xdefault)
        {
            string s = get(cfg, key);
            if(s.Length != 0)
            {
                double val;
                bool success = Double.TryParse(s, out val);
                if(success)
                {
                    return val;
                }
            }
            return xdefault;
        }

        public List<string> determineSubjects(Config cfg)
        {
            List<string> subjects = new List<string>();
            string s = get(cfg, "SUBJECT");
            if (s.Length != 0)
            {
                subjects.Add(s);
            }
            else
            {
                for (int i = 1; true; ++i)
                {
                    s = get(cfg, "SUBJECT." + i.ToString());
                    if (s.Length != 0)
                    {
                        subjects.Add(s);
                    }
                    else
                    {
                        break;
                    }
                }
            }
            if (subjects.Count == 0)
            {
                subjects.Add("GMSEC.>");
            }
            else
            {
                subjects.Add("GMSEC.TERMINATE");
            }
            return subjects;
        }
    }
}

















