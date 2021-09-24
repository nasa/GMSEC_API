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

/// <summary>
/// Class implements the Resource Acquisition Is Initialization (RAII) pattern where the resource is ownership of a mutex.
/// </summary>

public class AutoMutex : global::System.IDisposable {

  /// <summary>Standard constructor</summary>
  ///
  /// <param name="m">Mutex to associate with the %AutoMutex</param>
  /// <param name="acquire">Flag to indicate whether the Mutex should be locked</param>
  public AutoMutex(Mutex m, bool acquire) {
  }

  /// <summary>Standard constructor that automatically locks the Mutex</summary>
  ///
  /// <param name="m">Mutex to associate with the %AutoMutex</param>
  public AutoMutex(Mutex m) {
  }

  /// <summary>Locks the Mutex</summary>
  public void Enter() {
  }

  /// <summary>Releases the Mutex</summary>
  public void Leave() {
  }

}

}
