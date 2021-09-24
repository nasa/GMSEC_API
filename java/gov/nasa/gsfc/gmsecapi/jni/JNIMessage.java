/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */




package gov.nasa.gsfc.gmsecapi.jni;

import gov.nasa.gsfc.gmsecapi.gmsecAPI;
import gov.nasa.gsfc.gmsecapi.Config;
import gov.nasa.gsfc.gmsecapi.Field;
import gov.nasa.gsfc.gmsecapi.GMSEC_Long;
import gov.nasa.gsfc.gmsecapi.GMSEC_String;
import gov.nasa.gsfc.gmsecapi.GMSEC_Short;
import gov.nasa.gsfc.gmsecapi.GMSEC_ULong;
import gov.nasa.gsfc.gmsecapi.GMSEC_UShort;
import gov.nasa.gsfc.gmsecapi.Status;

/** @class Message
 *
 * @brief The Message object is a container for GMSEC Messages.  The methods of this
 * class allow the construction and manipulation of the data in a message.
 *
 */
public class JNIMessage extends gov.nasa.gsfc.gmsecapi.Message
{
	public long swigCPtr;
	public boolean swigCMemOwn;

	protected JNIMessage(long cPtr, boolean cMemoryOwn)
	{
		swigCMemOwn = cMemoryOwn;
		swigCPtr = cPtr;
	}

	/**
	 * @brief Use Message to create a container for GMSEC Messages
	 */
	public JNIMessage()
	{
		this(0, false);
	}

	protected void finalize() throws Throwable
	{
		try {
			delete();
		}
		finally {
			super.finalize();
		}
	}

	public synchronized void delete()
	{
		if (swigCPtr != 0 && swigCMemOwn)
		{
			gmsecJNI.delete_Message(swigCPtr, this);
			swigCMemOwn = false;
		}
		swigCPtr = 0;
	}

	protected static long getCPtr(JNIMessage obj)
	{
		return (obj == null) ? 0 : obj.swigCPtr;
	}

	/**
	 * @brief This method returns a boolean that is True if message is valid,
	 * False if it is not
	 *
	 * @note This method has been deprecated; use IsValid() instead.
	 *
	 * @return boolean - True if message is valid, False if it is not
	 */
	public boolean isValid()
	{
		return IsValid();
	}

	/**
	 * @brief This method returns a boolean that is True if message is valid,
	 * False if it is not
	 *
	 * @return boolean - True if message is valid, False if it is not
	 */
	public boolean IsValid()
	{
		return swigCMemOwn && gmsecJNI.Message_IsValid(swigCPtr, this);
	}

	/**
	 * @brief This method will set the GMSEC Message kind property for the Message
	 * object.  This field identifies the message as a Publish/Subscribe,
	 * Request, or Reply
	 *
	 * @param kind - this is the type of GMSEC Message
	 * @return Status - the resulting status of the operation
	 */
	public Status SetKind(int kind)
	{
		long cPtr = gmsecJNI.Message_SetKind(swigCPtr, this, kind);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief This method will get the GMSEC Message kind property for the Message
	 * object.  This field identifies the message as a Publish/Subscribe,
	 * Request, or Reply
	 *
	 * @param kind - this is the type of GMSEC Message
	 * @return Status - the resulting status of the operation
	 */
	public Status GetKind(GMSEC_UShort kind)
	{
		long cPtr = gmsecJNI.Message_GetKind(swigCPtr, this, kind);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief This method will set the GMSEC Message Subject property for the Message
	 * object
	 *
	 * @param subject - this is the subject that this message will be published to
	 * @return Status - the resulting status of the operation
	 */
	public Status SetSubject(String subject)
	{
		long cPtr = gmsecJNI.Message_SetSubject(swigCPtr, this, subject);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief This method will get the GMSEC Message Subject property for the Message
	 * object.  Subjects must be set to a value, which is compliant to the GMSEC
	 * Subject naming standards
	 *
	 * @param subject - this is the subject under which the message was published
	 * @return Status - the resulting status of the operation
	 */
	public Status GetSubject(GMSEC_String subject)
	{
		long cPtr = gmsecJNI.Message_GetSubject(swigCPtr, this, subject);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief This method will add the specified Field to the Message
	 *
	 * @param field - the Field object that is to be added to the message
	 * @return Status - the resulting status of the operation
	 */
	public Status AddField(Field field)
	{
		JNIField jfld = new JNIField(field);
		long cPtr = gmsecJNI.Message_AddField(swigCPtr, this, JNIField.getCPtr(jfld), jfld);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief This method will get the specified field object from the message
	 *
	 * @param name - the name of the field to retrieve from the message
	 * @param field - the Field object that is returned
	 * @return Status - the resulting status of the operation
	 */
	public Status GetField(String name, Field field)
	{
		JNIField jfld = new JNIField();
		long cPtr = gmsecJNI.Message_GetField(swigCPtr, this, name, JNIField.getCPtr(jfld), jfld);
		Status result = new JNIStatus(cPtr, true);
		if (!result.isError())
			field.Set(jfld);
		return result;
	}

	/**
	 * @brief This method will get the number of fields attached to this Message
	 *
	 * @param count - the number of fields in the message
	 * @return Status - the resulting status of the operation
	 */
	public Status GetFieldCount(GMSEC_Long count)
	{
		long cPtr = gmsecJNI.Message_GetFieldCount(swigCPtr, this, count);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief This method will get the first field object from the Message
	 *
	 * @param field - the Field object that is returned
	 * @return Status - the resulting status of the operation
	 */
	public Status GetFirstField(Field field)
	{
		JNIField jfld = new JNIField();
		long cPtr = gmsecJNI.Message_GetFirstField(swigCPtr, this, JNIField.getCPtr(jfld), jfld);
		Status result = new JNIStatus(cPtr, true);
		field.Set(jfld);
		return result;
	}

	/**
	 * @brief This method will get the next field object from the Message
	 *
	 * @param field - the Field object that is returned
	 * @return Status - the resulting status of the operation
	 */
	public Status GetNextField(Field field)
	{
		JNIField jfld = new JNIField();
		long cPtr = gmsecJNI.Message_GetNextField(swigCPtr, this, JNIField.getCPtr(jfld), jfld);
		Status result = new JNIStatus(cPtr, true);
		field.Set(jfld);
		return result;
	}

	/**  
	 * @brief This function set the Configuration of this message.  This can be used to 
	 * pass middleware specific items to the Message object.  All messages take the 
	 * 'subject' and 'kind' and will set those values when SetConfig is called. 
	 */
	public Status SetConfig(Config config)
	{
		JNIConfig cfg = new JNIConfig(config);
		long cPtr = gmsecJNI.Message_SetConfig(swigCPtr, this, JNIConfig.getCPtr(cfg), cfg);
		Status result = new JNIStatus(cPtr, true);
		return result;
	}

	public Status ToXML(GMSEC_String outst)
	{
		long cPtr = gmsecJNI.Message_ToXML(swigCPtr, this, outst);
		return new JNIStatus(cPtr, true);
	}

	public Status FromXML(String inst)
	{
		long cPtr = gmsecJNI.Message_FromXML(swigCPtr, this, inst);
		return new JNIStatus(cPtr, true);
	}

	public String ToJSON()
	{
		return gmsecJNI.Message_ToJSON(swigCPtr, this);
	}

	public Status FromJSON(String inst)
	{
		long cPtr = gmsecJNI.Message_FromJSON(swigCPtr, this, inst);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief This method will return the message size
	 * @param outst - a GMSEC_ULong that indicates the message size
	 * @return Status - the resulting status of the operation
	 */
	public Status GetMSGSize(GMSEC_ULong size)
	{
		long cPtr = gmsecJNI.Message_GetMSGSize(swigCPtr, this, size);
		return new JNIStatus(cPtr, true);
	}

	public String GetLibraryRootName()
	{
		// TODO: NOT IMPLEMENTED
		return "gmsec_jni";
	}

	public Status ClearFields()
	{
		long cPtr = gmsecJNI.Message_ClearFields(swigCPtr, this);
		return new JNIStatus(cPtr, true);
	}

	public Status ClearField(String name)
	{
		long cPtr = gmsecJNI.Message_ClearField(swigCPtr, this, name);
		return new JNIStatus(cPtr, true);
	}


	/**
	 * @note The following Set/GetTracking methods are not fully supported at this time,
	 * and will be deprecated in the near future.
	 */
	public void SetTracking(short flag)
	{
		//gmsecJNI.Message_SetTracking(swigCPtr, this, flag);
	}

	public void SetTrackingNode(short flag)
	{
		//gmsecJNI.Message_SetTrackingNode(swigCPtr, this, flag);
	}

	public void SetTrackingProcessId(short flag)
	{
		//gmsecJNI.Message_SetTrackingProcessId(swigCPtr, this, flag);
	}

	public void SetTrackingConnectionId(short flag)
	{
		//gmsecJNI.Message_SetTrackingConnectionId(swigCPtr, this, flag);
	}

	public void SetTrackingUserName(short flag)
	{
		//gmsecJNI.Message_SetTrackingUserName(swigCPtr, this, flag);
	}

	public void SetTrackingPublishTime(short flag)
	{
		//gmsecJNI.Message_SetTrackingPublishTime(swigCPtr, this, flag);
	}

	public void SetTrackingUniqueId(short flag)
	{
		//gmsecJNI.Message_SetTrackingUniqueId(swigCPtr, this, flag);
	}

	public void SetTrackingMwInfo(short flag)
	{
		//gmsecJNI.Message_SetTrackingMwInfo(swigCPtr, this, flag);
	}

	public short GetTracking()
	{
		//return gmsecJNI.Message_GetTracking(swigCPtr, this);
		return (short) gmsecAPI.MESSAGE_TRACKINGFIELDS_UNSET;
	}

	public short GetTrackingNode()
	{
		//return gmsecJNI.Message_GetTrackingNode(swigCPtr, this);
		return (short) gmsecAPI.MESSAGE_TRACKINGFIELDS_UNSET;
	}

	public short GetTrackingProcessId()
	{
		//return gmsecJNI.Message_GetTrackingProcessId(swigCPtr, this);
		return (short) gmsecAPI.MESSAGE_TRACKINGFIELDS_UNSET;
	}

	public short GetTrackingConnectionId()
	{
		//return gmsecJNI.Message_GetTrackingConnectionId(swigCPtr, this);
		return (short) gmsecAPI.MESSAGE_TRACKINGFIELDS_UNSET;
	}

	public short GetTrackingUserName()
	{
		//return gmsecJNI.Message_GetTrackingUserName(swigCPtr, this);
		return (short) gmsecAPI.MESSAGE_TRACKINGFIELDS_UNSET;
	}

	public short GetTrackingPublishTime()
	{
		//return gmsecJNI.Message_GetTrackingPublishTime(swigCPtr, this);
		return (short) gmsecAPI.MESSAGE_TRACKINGFIELDS_UNSET;
	}

	public short GetTrackingUniqueId()
	{
		//return gmsecJNI.Message_GetTrackingUniqueId(swigCPtr, this);
		return (short) gmsecAPI.MESSAGE_TRACKINGFIELDS_UNSET;
	}

	public short GetTrackingMwInfo()
	{
		//return gmsecJNI.Message_GetTrackingMwInfo(swigCPtr, this);
		return (short) gmsecAPI.MESSAGE_TRACKINGFIELDS_UNSET;
	}
}
