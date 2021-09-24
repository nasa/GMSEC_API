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
/// This class can be used to start a thread that will continuously publish C2CX HB messages.
///
/// The Heartbeat Generator creates a C2CX HB message using MistMessage. It then uses a
/// ConnectionManager to publish this message on a periodic basis.
///
/// <seealso cref="Config"/>
/// <seealso cref="ConnectionManager"/>
/// <seealso cref="MistMessage"/>
/// </summary>
public class HeartbeatGenerator : global::System.IDisposable
{
	/// <summary>
	/// Basic class constructor.
	/// </summary>
	///
	/// <param name="config">The Config object to configure the ConnectionManager that is used to publish messages</param>
	/// <param name="hbMsgSubject">The subject to include with the published heartbeat messages</param>
	/// <param name="hbPubRate">The message publish rate (in seconds)</param>
	///
	/// <exception cref="GmsecException">Thrown if the message subject is null or contains an empty string</exception>
	/// <exception cref="GmsecException">Thrown if the ConnectionManager cannot be created or connect to the GMSEC Bus</exception>
	/// <exception cref="GmsecException">Thrown if message validation is enabled and a valid C2CX HB message cannot be created</exception>
	///
	/// <remarks>
	/// <b>Note</b>
	///     - If the publish rate is set to 0 (zero), then the heartbeat generator will only publish one message.
	/// </remarks>
	///
	/// <seealso cref="SetField"/>
	/// <seealso cref="MistMessage.SetStandardFields"/>
	public HeartbeatGenerator(Config config, string hbMsgSubject, ushort hbPubRate) {
	}


	/// <summary>
	/// Class constructor where a list of message fields can be provided.
	/// </summary>
	///
	/// <param name="config">The Config object to configure the ConnectionManager that is used to publish messages</param>
	/// <param name="hbMsgSubject">The subject to include with the published heartbeat messages</param>
	/// <param name="hbPubRate">The message publish rate (in seconds)</param>
	/// <param name="fields">Fields to add to the heartbeat message</param>
	///
	/// <exception cref="GmsecException">Thrown if the message subject is null or contains an empty string</exception>
	/// <exception cref="GmsecException">Thrown if the ConnectionManager cannot be created or connect to the GMSEC Bus</exception>
	/// <exception cref="GmsecException">Thrown if message validation is enabled and a valid C2CX HB message cannot be created</exception>
	///
	/// <remarks>
	/// <b>Note</b>
	///     - If the publish rate is set to 0 (zero), then the heartbeat generator will only publish one message.
	/// </remarks>
	///
	/// <seealso cref="SetField"/>
	/// <seealso cref="MistMessage.SetStandardFields"/>
	public HeartbeatGenerator(Config config, string hbMsgSubject, ushort hbPubRate, FieldList fields) {
	}


	/// <summary>Starts the heartbeat generator thread.</summary>
	///
	/// <returns>True if the heartbeat generator thread has started; false otherwise</returns>
	///
	/// <remarks>
	/// <b>Note</b>
	///     - If the publish rate is set to 0 (zero), only one heartbeat message will be published, and
	///       the heartbeat generator thread will continue to run.
	/// </remarks>
	///
	/// <seealso cref="Stop"/>
	public bool Start() {
	}


	/// <summary>Stops the heartbeat generator thread.</summary>
	///
	/// <returns>True if the heartbeat generator thread has stopped; false otherwise</returns>
	///
	/// <seealso cref="Start"/>
	public bool Stop() {
	}


	/// <summary>Accessor that can be used to query whether the heartbeat generator thread is running.</summary>
	///
	/// <returns>True if the heartbeat generator thread is running; false otherwise</returns>
	///
	/// <seealso cref="Start"/>
	/// <seealso cref="Stop"/>
	public bool IsRunning() {
	}


	/// <summary>Accessor for changing the publish rate of the heartbeat message generator.</summary>
	///
	/// <param name="pubRate">The new publish rate (in seconds)</param>
	///
	/// <remarks>
	/// <b>Note</b>
	///     - If the publish rate is set to 0 (zero), only one additional heartbeat message will be published.
	/// </remarks>
	public void ChangePublishRate(ushort pubRate) {
	}


	/// <summary>Accessor that will add the given field to the heartbeat message.</summary>
	///
	/// <param name="field">The Field to add to the heartbeat message</param>
	///
	/// <returns>True if an existing field was overwritten; false otherwise</returns>
	///
	/// <exception cref="GmsecException">Thrown if for whatever reason the field cannot be added to the message.</exception>
	///
	/// <remarks>
	/// <b>Note</b>
	///     - If the PUB-RATE field is provided, then the publish rate for the heartbeat generator will be updated
	///       to publish at the (new) rate.
	/// </remarks>
	public bool SetField(Field field) {
	}


	/// <summary>Accessor that will smartly add a field to the heartbeat message.</summary>
	///
	/// <param name="fieldName">The name of the field</param>
	/// <param name="fieldValue">The value of the field</param>
	///
	/// <returns>True if an existing field was overwritten; false otherwise</returns>
	///
	/// <exception cref="GmsecException">Thrown if the field name is null or contains an empty string.</exception>
	/// <exception cref="GmsecException">Thrown if for whatever reason the field cannot be added to the message.</exception>
	///
	/// <remarks>
	/// <b>Notes</b>
	///     - This method relies on MistMessage.SetValue(string, long).
	///     - If the PUB-RATE field is provided, then the publish rate for the heartbeat generator will be updated
	///       to publish at the (new) rate.
	/// </remarks>
	public bool SetField(string fieldName, long fieldValue) {
	}


	/// <summary>Accessor that will smartly add a field to the heartbeat message.</summary>
	///
	/// <param name="fieldName">The name of the field</param>
	/// <param name="fieldValue">The value of the field</param>
	///
	/// <returns>True if an existing field was overwritten; false otherwise</returns>
	///
	/// <exception cref="GmsecException">Thrown if the field name is null or contains an empty string.</exception>
	/// <exception cref="GmsecException">Thrown if for whatever reason the field cannot be added to the message.</exception>
	///
	/// <remarks>
	/// <b>Notes</b>
	///     - This method relies on MistMessage.SetValue(string, double).
	///     - If the PUB-RATE field is provided, then the publish rate for the heartbeat generator will be updated
	///       to publish at the (new) rate.
	/// </remarks>
	public bool SetField(string fieldName, double fieldValue) {
	}


	/// <summary>Accessor that will smartly add a field to the heartbeat message.</summary>
	///
	/// <param name="fieldName">The name of the field</param>
	/// <param name="fieldValue">The value of the field</param>
	///
	/// <returns>True if an existing field was overwritten; false otherwise</returns>
	///
	/// <exception cref="GmsecException">Thrown if the field name is null or contains an empty string.</exception>
	/// <exception cref="GmsecException">Thrown if the field value is null.</exception>
	/// <exception cref="GmsecException">Thrown if for whatever reason the field cannot be added to the message.</exception>
	///
	/// <remarks>
	/// <b>Notes</b>
	///     - This method relies on MistMessage.SetValue(string, string).
	///     - If the PUB-RATE field is provided, then the publish rate for the heartbeat generator will be updated
	///       to publish at the (new) rate.
	/// </remarks>
	public bool SetField(string fieldName, string fieldValue) {
	}
}

}
