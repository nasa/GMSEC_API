/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/* This file is used for generating Doxygen documentation; it only contains
   stubs of constructors and methods associated with the class.
*/


namespace GMSEC.API {

/// <summary>The class that is passed to a LogHandler object when a message is logged.</summary>
public class LogEntry : global::System.IDisposable {

  /// <summary>The filename from where the log message originated</summary>
  public string file {
  }

  /// <summary>The line number within the file from where the log message originated</summary>
  public int line {
  }

  /// <summary>The logging level associated with the log message</summary>
  public LogLevel level {
  }

  /// <summary>The time when the log message was issued</summary>
  public GMSEC_TimeSpec time {
  }

  /// <summary>The message associated with the log message</summary>
  public string message {
  }

  /// <summary>Standard constructor</summary>
  public LogEntry() {
  }

}

}
