/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/* This file is used for generating Doxygen documentation; it only contains
   stubs of constructors and methods associated with the class.
*/


namespace GMSEC.API5 {

/// <summary>Error/Status Class codes (used with Status and GmsecException classes)</summary>

public enum StatusClass {
  /// <summary>No error</summary>
  NO_ERROR_CLASS = 0,

  /// <summary>Error creating Connection object</summary>
  FACTORY_ERROR = 1,

  /// <summary>Error within Connection class</summary>
  CONNECTION_ERROR = 2,

  /// <summary>Error within Config class</summary>
  CONFIG_ERROR = 3,

  /// <summary>Middleware specific error</summary>
  MIDDLEWARE_ERROR = 4,

  /// <summary>Error within Message class</summary>
  MSG_ERROR = 5,

  /// <summary>Error within Field related class</summary>
  FIELD_ERROR = 6,

  /// <summary>Error using a callback</summary>
  CALLBACK_ERROR = 7,

  /// <summary>Error referencing a callback object</summary>
  CALLBACK_LOOKUP_ERROR = 8,

  /// <summary>Error within a ConfigFile class</summary>
  CONFIGFILE_ERROR = 9,

  /// <summary>Error using an iterator class</summary>
  ITERATOR_ERROR = 10,

  /// <summary>Error with policy</summary>
  POLICY_ERROR = 11,

  /// <summary>Error with AutoDispatcher</summary>
  DISPATCHER_ERROR = 12,

  /// <summary>Resource Generator error</summary>
  RESOURCE_GENERATOR_ERROR = 13,

  /// <summary>Heartbeat Generator error</summary>
  HEARTBEAT_GENERATOR_ERROR = 14,

  /// <summary>Specification error</summary>
  SPECIFICATION_ERROR = 15,

  /// <summary>Message Factory error</summary>
  MSG_FACTORY_ERROR = 16,

  /// <summary>Message Template error</summary>
  MSG_TEMPLATE_ERROR = 17,

  /// <summary>Field Template error</summary>
  FIELD_TEMPLATE_ERROR = 18,

  /// <summary>Custom error</summary>
  CUSTOM_ERROR = 49,

  /// <summary>Other error</summary>
  OTHER_ERROR = 50
}

}
