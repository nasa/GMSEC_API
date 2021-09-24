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

/// <summary>Defines the abstract class for the logger handler callbacks.</summary>

public class LogHandler : global::System.IDisposable {

  /// <summary>
  /// This method is called by the API in response to a log message. A class derived from
  /// %LogHandler needs to be registered with Log.RegisterHandler() in order to be called
  /// for a particular logging level.
  ///
  /// Because this class is user defined, it is not considered thread-safe unless made so by the
  /// user. The use of a AutoMutex is suggested to encapsulate any sensitive code blocks.
  /// </summary>
  ///
  /// <param name="entry">Contains information regarding the data being logged</param>
  public virtual void OnMessage(LogEntry entry) {
  }

  /// <summary>Constructor</summary>
  protected LogHandler() {
  }

}

}
