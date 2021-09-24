/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







package gov.nasa.gsfc.gmsecapi;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintWriter;

/**
 *
 * @deprecated API 3.x has been deprecated; use API 4.x instead.
 *
 * @brief Basic logging class. This class supports basic logging to any output
 *        stream. This class is thread-safe.
 *        
 *        Log does not use lazy initialization to avoid multi-threading issues related 
 *  to the singleton pattern.  Solving these problems would require frequent 
 *  synchronization that wouldn't be worthwhile considering the small size of the Log
 *  class.
 * 
 */
@Deprecated
public class Log
{
	protected static Log instance = new Log();

	protected PrintWriter fLogger;

	protected FileOutputStream fFileOut;

	/**
	 * Create a new instance of Log. Initializes the logger to null, signifying
	 * that no logging is currently being performed. In order to use logging,
	 * call createLog on the Log returned by getLog().  Also installs a shutdown
	 * hook to ensure resources are cleaned up on close.
	 * 
	 */
	protected Log()
	{
		fLogger = null;
		fFileOut = null;

		Runtime.getRuntime().addShutdownHook(new Thread()
		{
			public void run()
			{
				destroyLog();
			}
		});
	}

	/**
	 * @return the singleton instance of Log
	 */
	public static Log getLog()
	{
		return instance;
	}

	/**
	 * @deprecated
	 * Internal function used to activate the logger.
	 *
	 * @param os
	 *            the OutputStream to log messages on.
	 * @param autoFlush
	 *            whether to write automatically on a newline
	 */
	protected void setLogger(OutputStream os, boolean autoFlush)
	{
		notifyDeprecatedMethod("setLogger");
		fLogger = new PrintWriter(new BufferedOutputStream(os), autoFlush);
	}

	/**
	 * @deprecated
	 * Sets where log output goes.
	 *
	 * @param logFile
	 *            the file to log messages to
	 * @return whether the operation succeeded
	 */
	public Status createLog(File logFile)
	{
		notifyDeprecatedMethod("createLog");
		synchronized (Log.class)
		{
			Status status = new Status();
			if (logFile == null)
			{
				status.Set(gmsecAPI.GMSEC_STATUS_OTHER_ERROR, gmsecAPI.GMSEC_OTHER_ERROR, "User passed null log file.");
				return status;
			}

			try
			{

				if (fLogger == null)
				{
					fFileOut = new FileOutputStream(logFile);
					setLogger(fFileOut, false);
				}
				else
				{
					status.Set(gmsecAPI.GMSEC_STATUS_OTHER_ERROR, gmsecAPI.GMSEC_OTHER_ERROR, "Log already created.");
				}

			}
			catch (FileNotFoundException fnfException)
			{
				status.Set(gmsecAPI.GMSEC_STATUS_OTHER_ERROR, gmsecAPI.GMSEC_OTHER_ERROR, "Error opening log file.  Using console for debug messages.");
				setLogger(System.out, true);
			}
			return status;
		}
	}

	/**
	 * @deprecated
	 * Sets where log output goes.
	 * 
	 * @param os
	 *            the output stream for log messages.
	 * @param autoFlush
	 *            whether the logger should automatically flush on a newline
	 * @return whether the operation succeeded
	 */
	public Status createLog(OutputStream os, boolean autoFlush)
	{
		notifyDeprecatedMethod("createLog");
		synchronized (Log.class)
		{
			Status status = new Status();
			if (os == null)
			{
				status.Set(gmsecAPI.GMSEC_STATUS_OTHER_ERROR, gmsecAPI.GMSEC_OTHER_ERROR, "User passed null output stream.");
				return status;
			}

			if (fLogger == null)
			{
				setLogger(os, autoFlush);
			}
			else
			{
				status.Set(gmsecAPI.GMSEC_STATUS_OTHER_ERROR, gmsecAPI.GMSEC_OTHER_ERROR, "Log already created.");
			}
			return status;
		}
	}

	/**
	 * @deprecated
	 * Flushes all buffers and cleans up any resources associated with the Log
	 * 
	 * @return Status object concerning the success or failure of the operation 
	 */
	public Status destroyLog()
	{
		notifyDeprecatedMethod("destroyLog");
		Status stat = new Status();
		synchronized (Log.class)
		{
			if (fLogger != null)
			{
				fLogger.flush();
				fLogger = null;

				if (fFileOut != null)
				{
					try
					{
						fFileOut.close();
						fFileOut = null;
					}
					catch (IOException ioException)
					{
						stat.Set(gmsecAPI.GMSEC_STATUS_OTHER_ERROR, gmsecAPI.GMSEC_OTHER_ERROR, "Error closing Log file.");
					}
				}
			}
			else
			{
				stat.Set(gmsecAPI.GMSEC_STATUS_OTHER_ERROR, gmsecAPI.GMSEC_OTHER_ERROR, "No Log created yet.");
			}
		}
		return stat;
	}

	/**
	 * @return Whether we are currently logging debug messages.
	 */
	public boolean isDebug()
	{
		return fLogger != null;
	}

	/**
	 * Send a message to the logger.
	 * 
	 * @param msg
	 *            the message to log
	 */
	public void debug(String msg)
	{
		if (fLogger != null)
		{
			synchronized (Log.class)
			{
				fLogger.println(msg);
			}
		}
	}

	/**
	 * @deprecated
	 * Send a message to the logger
	 * 
	 * @param caller
	 *            String identifying the class calling this debug
	 * @param msg
	 *            the message to log
	 */
	public void debug(String caller, String msg)
	{
		notifyDeprecatedMethod("debug");
		if (fLogger != null)
		{
			String outMsg = "\t[" + caller + "]\t" + msg;
			synchronized (Log.class)
			{
				fLogger.println(outMsg);
			}
		}
	}
	
	private void notifyDeprecatedMethod(String method)
	{
		if (fLogger != null)
		{
			synchronized (Log.class)
			{
				fLogger.println("The method " + method + " is being deprecated.");
			}	
		}
	}	
}
