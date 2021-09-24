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

/// <summary>This class contains %GMSEC-related constants.</summary>

public class Gmsec {

  /// <summary>2014 version of the %GMSEC Interface Specification Document (ISD).</summary>
  public static uint GMSEC_ISD_2014_00 = 201400;

  /// <summary>2016 version of the %GMSEC Interface Specification Document (ISD).</summary>
  public static uint GMSEC_ISD_2016_00 = 201600;

  /// <summary>2018 version of the Command and Control Message Specification (C2MS).</summary>
  public static uint GMSEC_ISD_2018_00 = 201800;

  /// <summary>2019 version of the Command and Control Message Specification (C2MS).</summary>
  public static uint GMSEC_ISD_2019_00 = 201900;

  /// <summary>Current/latest version of the message specification.</summary>
  public static uint GMSEC_ISD_CURRENT = GMSEC_ISD_2019_00;

  /// <summary>Advises methods with timeouts to not block.</summary>
  ///
  /// <seealso cref="Connection.Receive"/>
  /// <seealso cref="ConnectionManger.Receive"/>
  public static int GMSEC_NO_WAIT = 0;

  /// <summary>Advises methods with timeouts to block.</summary>
  ///
  /// <seealso cref="Connection.Receive"/>
  /// <seealso cref="ConnectionManger.Receive"/>
  public static int GMSEC_WAIT_FOREVER = -1;

  /// <summary>Advises that a request message should only be issued once should a response message not be received.</summary>
  ///
  /// <seealso cref="Connection.Request(Message, int, int"/>
  /// <seealso cref="Connection.Request(Message, int, ReplyCallback, int"/>
  /// <seealso cref="ConnectionManager.Request(Message, int, int"/>
  /// <seealso cref="ConnectionManger.Request(Message, int, ReplyCallback, int"/>
  public static int GMSEC_REQUEST_REPUBLISH_NEVER = -1;
}

}
