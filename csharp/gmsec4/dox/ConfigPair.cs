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

/// <summary>Container to access individual name/value pairs that are stored in a Config object.</summary>
///
/// <seealso cref="Config.GetFirst"/>
/// <seealso cref="Config.GetNext"/>

public class ConfigPair : global::System.IDisposable {

  /// <summary>Returns the name of the Config entry.</summary>
  public string GetName() {
    string ret = GmsecPINVOKE.ConfigPair_GetName(swigCPtr);
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  /// <summary>Returns the value of the Config entry.</summary>
  public string GetValue() {
    string ret = GmsecPINVOKE.ConfigPair_GetValue(swigCPtr);
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

}

}
