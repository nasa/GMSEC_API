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

/// <summary>%Condition support for managing dependent threads.</summary>

public class Condition : global::System.IDisposable {

  /// <summary>Standard constructor</summary>
  public Condition() {
  }

  /// <summary>Standard constructor that initializes with an externally managed mutex</summary>
  /// <param name="mutex">External Mutex object</param>
  public Condition(Mutex mutex) {
  }

  /// <summary>Returns a reference to the Mutex held by the object</summary>
  public Mutex GetMutex() {
  }

  /// <summary>Will block until the thread is signaled</summary>
  public int Wait() {
  }

  /// <summary>
  /// Will block until the thread is signaled or the time specified has expired. If millis is not
  /// positive, timeout immediately.
  /// </summary>
  ///
  /// <param name="millis">The length in time in milliseconds to wait for the thread to be signaled before continuing</param>
  public int Wait(int millis) {
  }

  /// <summary>
  /// The function will cause the calling thread to wait until the condition is signalled or the time specified has been reached
  /// </summary>
  ///
  /// <param name="t">The absolute timeout</param>
  public int WaitUntil(GMSEC_TimeSpec t) {
  }

  /// <summary>Signals any waiting threads to continue with their activity</summary>
  ///
  /// <param name="reason">The signal to emit to threads that are waiting</param>
  public void Signal(int reason) {
  }

  /// <summary>Signals all waiting threads to continue with their activity</summary>
  ///
  /// <param name="reason">The signal to emit to threads that are waiting</param>
  public void Broadcast(int reason) {
  }

  public static readonly int INVALID = 0;
  public static readonly int TIMEOUT = 1;
  public static readonly int USER    = 2;

}

}
