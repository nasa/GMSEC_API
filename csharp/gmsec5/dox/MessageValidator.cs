/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/* This file is used for generating Doxygen documentation; it only contains
   stubs of constructors and methods associated with the class.
*/


namespace GMSEC.API5 {

/// <summary>
/// This class is the abstract base class for performing custom message validation.
/// A user created class, derived from this class, can be passed into
/// RegisterMessageValidator() to have user code executed when a message is being validated.
/// <p>
/// This custom message validator will be used to supplement the already built-in API message
/// validator. The custom message validator will be called on first, and if the returned status
//  is nominal, then the API's message validator will be called.
/// <p>
/// Note that because users are able to create their own MessageValidator class,
/// reentrancy is not guaranteed unless if reentrancy rules are specified.
/// <p>
/// In addition, if a MessageValidator is registered to multiple Message objects,
/// ValidateMessage() can be invoked concurrently from different threads. Use of a Mutex
/// is suggested to enforce thread safety.
/// </summary>
///
/// <seealso cref="Message.RegisterMessageValidator(MessageValidator)"/>
/// <seealso cref="MessageFactory.RegisterMessageValidator(MessageValidator)"/>

public class MessageValidator : global::System.IDisposable {

    /// <summary>
	/// This method is called by the API to perform a user-defined message validation.
    /// </summary>
    ///
    /// <remarks>
    /// <b>DO NOT STORE</b> the Message object for use beyond the scope of the function.
    /// </remarks>
    ///
    /// <param name="msg">the message to validator</param>
	///
	/// <returns>The status of the operation </returns>
	public virtual Status ValidateMessage(Message msg) {
	}

	protected ValidateMessage() {
	}
}

}
