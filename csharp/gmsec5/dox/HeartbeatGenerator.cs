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
/// This class can be used to start a thread that will continuously publish HB (Heartbeat) messages.
///
/// <seealso cref="Config"/>
/// </summary>
public class HeartbeatGenerator : global::System.IDisposable
{
	/// <summary>
	/// Basic class constructor.
	/// </summary>
	///
	/// <param name="config">The Config object to configure the Connection that is used to publish messages</param>
	/// <param name="hbPubRate">The message publish rate (in seconds)</param>
	///
	/// <exception cref="GmsecException">Thrown if a Connection object cannot be created</exception>
	///
	/// <remarks>
	/// <b>Note</b>
	///     - If the publish rate is set to 0 (zero), then the heartbeat generator will only publish one message.
	/// </remarks>
	///
	/// <seealso cref="SetField"/>
	public HeartbeatGenerator(Config config, ushort hbPubRate) {
	}


	/// <summary>
	/// Class constructor where a list of message fields can be provided.
	/// </summary>
	///
	/// <param name="config">The Config object to configure the Connection that is used to publish messages</param>
	/// <param name="hbPubRate">The message publish rate (in seconds)</param>
	/// <param name="fields">Fields to add to the heartbeat message</param>
	///
	/// <exception cref="GmsecException">Thrown if a Connection object cannot be created</exception>
	///
	/// <remarks>
	/// <b>Note</b>
	///     - If the publish rate is set to 0 (zero), then the heartbeat generator will only publish one message.
	/// </remarks>
	///
	/// <seealso cref="SetField"/>
	public HeartbeatGenerator(Config config, ushort hbPubRate, FieldList fields) {
	}


	/// <summary>Starts the heartbeat generator thread.</summary>
	///
	/// <returns>True if the heartbeat generator thread has started; false otherwise</returns>
	///
	/// <exception cref="GmsecException">Thrown if a connection cannot be established with the GMSEC Bus</exception>
	/// <exception cref="GmsecException">Thrown if message validation is enabled and a valid HB message cannot be created</exception>
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
	///     - This method relies on Message.SetFieldValue(string, long).
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
	///     - This method relies on Message.SetFieldValue(string, double).
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
	///     - This method relies on Message.SetFieldValue(string, string).
	///     - If the PUB-RATE field is provided, then the publish rate for the heartbeat generator will be updated
	///       to publish at the (new) rate.
	/// </remarks>
	public bool SetField(string fieldName, string fieldValue) {
	}
}

}
