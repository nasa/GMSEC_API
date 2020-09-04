/*
 * Copyright 2007-2017 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/*! \file Example.cs 
 *   This file contains a GMSECExample class, which contains common
 *  functionality found in the C# example programs.
 */


namespace GMSEC.API.Example.Common
{
	using GMSEC.API;
	using System;
	using System.IO;
	using System.Collections.Generic;


	class ExampleException : Exception
	{
		public ExampleException(string tag)
			:base(tag)
		{
		}
	}


	public class GmsecExample
	{
		public void Initialize(Config config)
		{
			Config tmpConfig = new Config(config);
			string logLevel  = tmpConfig.GetValue("LOGLEVEL");
			string logFile   = tmpConfig.GetValue("LOGFILE");

			if (logLevel == null)
			{
				tmpConfig.AddValue("LOGLEVEL", "INFO");
			}
			if (logFile == null)
			{
				tmpConfig.AddValue("LOGFILE", "STDERR");
			}
		}


		public void AddToConfigFromFile(Config config)
		{
			string cfgfilename = Get(config, "CFGFILE");
			// This assumes, now, that there is only _one_
			// CFGFILE=filename.xml arg specified.

			string loglevel = Get(config, "LOGLEVEL");

			if (cfgfilename != null)
			{
				if (File.Exists(cfgfilename))
				{
					try
					{    
						string file_text_contents = System.IO.File.ReadAllText(cfgfilename);

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
						bool havePair = config_from_file.GetFirst(out name,  out value);

						while (havePair)
						{
							if (loglevel.Equals("DEBUG", StringComparison.OrdinalIgnoreCase))
							{
								Console.Out.WriteLine("[DEBUG]:  Adding ("
									+ name + ", " + value
									+ ") from config file "
									+ cfgfilename);
							}

							config.AddValue(name, value);

							havePair = config_from_file.GetNext(out name, out value);
						}
					}
					catch (Exception e)
					{
						if (loglevel.Equals("WARNING", StringComparison.OrdinalIgnoreCase) ||
						    loglevel.Equals("INFO", StringComparison.OrdinalIgnoreCase) ||
						    loglevel.Equals("VERBOSE", StringComparison.OrdinalIgnoreCase) ||
						    loglevel.Equals("DEBUG", StringComparison.OrdinalIgnoreCase))
						{
							Console.Out.WriteLine("[WARNING]:  Error reading file " + cfgfilename + ":  {0}", e);
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
						Console.Out.WriteLine("[WARNING]:  Non-valid-filepath config argument " + cfgfilename + " seen; ignoring it.");
					}
				}
			}// if there is a config file argument at all
		}


		//Are the user arguments Valid
		public bool IsOptionInValid(Config cfg, string[] args, bool checkProgTimeout)
		{
			if (checkProgTimeout)  //must define msg_timeout for prog_timeout
			{
				if (Get(cfg, "MSG_TIMEOUT_MS").Length == 0 && Get(cfg, "PROG_TIMEOUT_S").Length > 0)
				{
					return true;
				}
			}

			int conn_type_length = Get(cfg, "connectiontype").Length;
			int mw_id_length     = Get(cfg, "mw-id").Length;

			if (args.Length < 1 || (conn_type_length == 0 && mw_id_length == 0))
			{
				return true;
			}
			return false;
		}


		public string Get(Config cfg, string key)
		{
			string value = cfg.GetValue(key);
			if (value != null)
			{
				return value;
			}
			value = "";
			return value;
		}


		public string Get(Config cfg, string key, string xdefault)
		{
			string value = Get(cfg, key);
			if (value.Length == 0)
			{
				value = xdefault;
			}
			return value;
		}


		public int Get(Config cfg, string key, int xdefault)
		{
			string s = Get(cfg, key);
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


		public double Get(Config cfg, string key, double xdefault)
		{
			string s = Get(cfg, key);
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


		public List<string> DetermineSubjects(Config cfg)
		{
			List<string> subjects = new List<string>();
			string s = Get(cfg, "SUBJECT");
			if (s.Length != 0)
			{
				subjects.Add(s);
			}
			else
			{
				for (int i = 1; true; ++i)
				{
					s = Get(cfg, "SUBJECT." + i.ToString());
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
