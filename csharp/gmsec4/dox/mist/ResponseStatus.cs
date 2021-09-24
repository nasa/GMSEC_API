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

/// <summary>Enumerated values that can be used to indicate the status of a request.</summary>

public class ResponseStatus : global::System.IDisposable {

  /// <summary>Enumerated values that can be used to indicate the status of a request.</summary>
  public enum Response {

    /// <summary>Acknowledge request</summary>
    ACKNOWLEDGEMENT = 1,

    /// <summary>Still processing request</summary>
    WORKING_KEEP_ALIVE = 2,

    /// <summary>Successful completion of request</summary>
    SUCCESSFUL_COMPLETION = 3,

    /// <summary>Failed to complete request</summary>
    FAILED_COMPLETION = 4,

    /// <summary>Invalid request</summary>
    INVALID_REQUEST = 5,

    /// <summary>Final reply message related to request</summary>
    FINAL_MESSAGE = 6
  }

}

}
