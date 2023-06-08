/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/* This file is used for generating Doxygen documentation; it only contains
   stubs of constructors and methods associated with the class.
*/


namespace GMSEC.API5 {

/// <summary>
/// This class can be used to start a thread that will continuously publish RSRC (Resource) messages.
///
/// <seealso cref="Config"/>
/// </summary>
public class ResourceGenerator : global::System.IDisposable
{
	/// <summary>
	/// Basic class constructor.
	/// </summary>
	///
	/// <param name="config">The Config object to configure the Connection that is used to publish messages</param>
	/// <param name="pubRate">The message publish rate (in seconds)</param>
	/// <param name="sampleInterval">The time distance between sample collection (in seconds)</param>
	/// <param name="averageInterval">The window over which to make a moving average of samples (in seconds)</param>
	///
    /// <exception cref="GmsecException">Thrown if a Connection object cannot be created</exception>
	///
	/// <remarks>
	/// <b>Note</b>
	///     - If the publish rate is set to 0 (zero), then the resource generator will only publish one message.
	/// </remarks>
	///
	/// <seealso cref="SetField"/>
	public ResourceGenerator(Config config, ushort pubRate, ushort sampleInterval, ushort averageInterval) {
	}


	/// <summary>
	/// Class constructor where a list of message fields can be provided.
	/// </summary>
	///
	/// <param name="config">The Config object to configure the Connection that is used to publish messages</param>
	/// <param name="pubRate">The message publish rate (in seconds)</param>
	/// <param name="sampleInterval">The time distance between sample collection (in seconds)</param>
	/// <param name="averageInterval">The window over which to make a moving average of samples (in seconds)</param>
	/// <param name="fields">Fields to add to the resource message</param>
	///
    /// <exception cref="GmsecException">Thrown if a Connection object cannot be created</exception>
	///
	/// <remarks>
	/// <b>Note</b>
	///     - If the publish rate is set to 0 (zero), then the resource generator will only publish one message.
	/// </remarks>
	///
	/// <seealso cref="SetField"/>
	public ResourceGenerator(Config config, ushort pubRate, ushort sampleInterval, ushort averageInterval, FieldList fields) {
	}


	/// <summary>Starts the resource generator thread.</summary>
	///
	/// <returns>True if the resource generator thread has started; false otherwise</returns>
	///
	/// <exception cref="GmsecException">Thrown if a connection cannot be established with the GMSEC Bus</exception>
	/// <exception cref="GmsecException">Thrown if message validation is enabled and a valid HB message cannot be created</exception>
	///
	/// <remarks>
	/// <b>Note</b>
	///     - If the publish rate is set to 0 (zero), only one resource message will be published, and
	///       the resource generator thread will continue to run.
	/// </remarks>
	///
	/// <seealso cref="Stop"/>
	public bool Start() {
	}


	/// <summary>Stops the resource generator thread.</summary>
	///
	/// <returns>True if the resource generator thread has stopped; false otherwise</returns>
	///
	/// <seealso cref="Start"/>
	public bool Stop() {
	}


	/// <summary>Accessor that can be used to query whether the resource generator thread is running.</summary>
	///
	/// <returns>True if the resource generator thread is running; false otherwise</returns>
	///
	/// <seealso cref="Start"/>
	/// <seealso cref="Stop"/>
	public bool IsRunning() {
	}


	/// <summary>Accessor that will add the given field to the resource message.</summary>
	///
	/// <param name="field">The Field to add to the resource message</param>
	///
	/// <returns>True if an existing field was overwritten; false otherwise</returns>
	///
	/// <exception cref="GmsecException">Thrown if for whatever reason the field cannot be added to the message.</exception>
	///
	/// <remarks>
	/// <b>Note</b>
	///     - If the PUB-RATE field is provided, then the publish rate for the resource generator will be updated
	///       to publish at the (new) rate.
	/// </remarks>
	public bool SetField(Field field) {
	}


	/// <summary>Creates/returns a Resource Message using the given MessageFactory.</summary>
	///
	/// <param name="factory">The MessageFactory to reference when creating the resource message</param>
	/// <param name="sampleInterval">The time distance between sample collection (in seconds)</param>
	/// <param name="averageInterval">Time window over which to make a moving average of samples (in seconds)</param>
	///
	/// <returns>A Message object representing a Resource Message</returns>
	///
	/// <exception cref="GmsecException">Thrown if sampleInterval is less than 0 (zero), or if averageInterval is less than sampleInterval</exception>
	///
	/// <seealso cref="Connection.getMessageFactory()"/>
	public Message CreateResourceMessage(MessageFactory factory, ushort sampleInterval, ushort averageInterval) {
	}
}

}
