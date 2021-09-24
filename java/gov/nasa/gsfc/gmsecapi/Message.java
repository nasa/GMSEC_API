/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







package gov.nasa.gsfc.gmsecapi;

import gov.nasa.gsfc.gmsecapi.internal.MessageImpl;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.TimeZone;

import org.w3c.dom.*;


/** @class Message
 *
 * @deprecated API 3.x has been deprecated; use API 4.x instead.
 *
 * @brief The Message object is a container for GMSEC Messages.  The methods of this
 * class allow the construction and manipulation of the data in a message.
 *
 */
@Deprecated
public class Message implements MessageImpl
{
	private MessageImpl fMsg = null;

	public Message()
	{
	}

	public Message(MessageImpl msg)
	{
		fMsg = msg;
	}

	public void SetImpl(MessageImpl msg)
	{
		fMsg = msg;
	}

	public MessageImpl GetImpl()
	{
		return fMsg;
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
		return fMsg != null && fMsg.isValid();
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
		if (fMsg == null)
			return uninitialized();

		return fMsg.SetKind(kind);
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
		if (fMsg == null)
			return uninitialized();

		if (kind == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_MESSAGE_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Kind is NULL");
		}

		return fMsg.GetKind(kind);
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
		if (fMsg == null)
			return uninitialized();

		return fMsg.SetSubject(subject);
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
		if (fMsg == null)
			return uninitialized();

		if (subject == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_MESSAGE_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Subject is NULL");
		}

		return fMsg.GetSubject(subject);
	}

	/**
	 * @brief This method will add the specified Field to the Message
	 *
	 * @param field - the Field object that is to be added to the message
	 * @return Status - the resulting status of the operation
	 */
	public Status AddField(Field field)
	{
		if (fMsg == null)
			return uninitialized();

		if (field == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_MESSAGE_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Field is NULL");
		}

		return fMsg.AddField(field);
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
		if (fMsg == null)
			return uninitialized();

		if (field == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_MESSAGE_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Field is NULL");
		}

		return fMsg.GetField(name, field);
	}

	/**
	 * @brief This method will get the number of fields attached to this Message
	 *
	 * @param count - the number of fields in the message
	 * @return Status - the resulting status of the operation
	 */
	public Status GetFieldCount(GMSEC_Long count)
	{
		if (fMsg == null)
			return uninitialized();

		if (count == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_MESSAGE_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Count is NULL");
		}

		return fMsg.GetFieldCount(count);
	}

	/**
	 * @brief This method will get the first field object from the Message
	 *
	 * @param field - the Field object that is returned
	 * @return Status - the resulting status of the operation
	 */
	public Status GetFirstField(Field field)
	{
		if (fMsg == null)
			return uninitialized();

		if (field == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_MESSAGE_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Field is NULL");
		}

		return fMsg.GetFirstField(field);
	}

	/**
	 * @brief This method will get the next field object from the Message
	 *
	 * @param field - the Field object that is returned
	 * @return Status - the resulting status of the operation
	 */
	public Status GetNextField(Field field)
	{
		if (fMsg == null)
			return uninitialized();

		if (field == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_MESSAGE_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Field is NULL");
		}

		return fMsg.GetNextField(field);
	}

	/**  
	 * @brief This function set the Configuration of this message.  This can be used to 
	 * pass middleware specific items to the Message object.  All messages take the 
	 * 'subject' and 'kind' and will set those values when SetConfig is called. 
	 */
	public Status SetConfig(Config config)
	{
		if (fMsg == null)
			return uninitialized();

		if (config == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_MESSAGE_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Config is NULL");
		}

		return fMsg.SetConfig(config);
	}

	/**
	 * @brief This method will return an XML formatted String with the values from the message.
	 * The XML format is the same as parsed by the FromXML().
	 *
	 * XML Format example:
	 * @code
	 * <MESSAGE SUBJECT="GMSEC.MISSION.CONST.SAT.EVT.MSG" KIND="PUBLISH">
	 * 	<FIELD TYPE="CHAR" NAME="CHAR-FIELD">c</FIELD>
	 * 	<FIELD TYPE="BOOL" NAME="BOOL-FIELD">TRUE</FIELD>
	 * 	<FIELD TYPE="I16" NAME="SHORT-FIELD">-123</FIELD>
	 * 	<FIELD TYPE="U16" NAME="USHORT-FIELD">123</FIELD>
	 * 	<FIELD TYPE="I32" NAME="INT-FIELD">-12345678</FIELD>
	 * 	<FIELD TYPE="U32" NAME="UINT-FIELD">123456789</FIELD>
	 * 	<FIELD TYPE="STRING" NAME="STRING-FIELD">This is a test of the Message::ToXML function</FIELD>
	 * 	<FIELD TYPE="F32" NAME="LONG-FIELD" BITS="42F6E666">123.45</FIELD>
	 * 	<FIELD TYPE="F64" NAME="DOUBLE-FIELD" BITS="4B94216CA930D75A">1.234e56</FIELD>
	 * 	<FIELD TYPE="BIN" NAME="BIN-FIELD">4a4c4d4e4f5051</FIELD>
	 * </MESSAGE>
	 * @endcode
	 *
	 * @param xml - string where to store XML representation of the Message
	 * @return Status - the resulting status of the operation
	 *
	 * @sa FromXML()
	 */
	public Status ToXML(GMSEC_String xml)
	{
		if (fMsg == null)
			return uninitialized();

		if (xml == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_MESSAGE_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "XML output string is NULL");
		}

		return fMsg.ToXML(xml);
	}

	/**
	 * @brief This method will parse a Message from XML.
	 * The format is the same as that which is generated by ToXML().
	 *
	 * @param xml - XML string representation of a Message
	 * @return Status - the resulting status of the operation.
	 *
	 * @sa ToXML()
	 */
	public Status FromXML(String xml)
	{
		if (fMsg == null)
			return uninitialized();

		return fMsg.FromXML(xml);
	}

	/**
	 * @brief This method will return an JSON formatted String with the values from the message.
	 * The JSON format is the same as parsed by the FromJSON().
	 *
	 * JSON Format example:
	 * @code
	 * {
	 *    "MESSAGE":{
	 *        "KIND":"PUBLISH",
	 *        "SUBJECT":"GMSEC.MISSION.CONST.SAT.EVT.MSG",
	 *        "FIELD":[
	 *            {
	 *                "NAME":"CHAR-FIELD",
	 *                "TYPE":"CHAR",
	 *                "VALUE":"c"
	 *            },
	 *            {
	 *                "NAME":"BOOL-FIELD",
	 *                "TYPE":"BOOL",
	 *                "VALUE":"TRUE"
	 *            },
	 *            {
	 *                "NAME":"SHORT-FIELD",
	 *                "TYPE":"I16",
	 *                "VALUE":"-123"
	 *            },
	 *            {
	 *                "NAME":"USHORT-SHORT",
	 *                "TYPE":"U16",
	 *                "VALUE":"123"
	 *            },
	 *            {
	 *                "NAME":"INT-FIELD",
	 *                "TYPE":"I32",
	 *                "VALUE":"-12345678"
	 *            },
	 *            {
	 *                "NAME":"UINT-FIELD",
	 *                "TYPE":"U32",
	 *                "VALUE":"123456789"
	 *            },
	 *            {
	 *                "NAME":"STRING-FIELD",
	 *                "TYPE":"STRING",
	 *                "VALUE":"This is a test of the Message::toJSON function"
	 *            },
	 *            {
	 *                "NAME":"LONG-FIELD",
	 *                "TYPE":"F32",
	 *                "VALUE":"123.45"
	 *                "BITS":"42F6E666"
	 *            },
	 *            {
	 *                "NAME":"DOUBLE-FIELD",
	 *                "TYPE":"F64",
	 *                "VALUE":"1.234e56"
	 *                "BITS":"4B94216CA930D75A"
	 *            },
	 *            {
	 *                "NAME":"BIN-FIELD",
	 *                "TYPE":"BIN",
	 *                "VALUE":"4a4c4d4e4f5051"
	 *            }
	 *	    ]
	 *	}
	 * }
	 * @endcode
	 *
	 * @param json - string where to store JSON representation of the Message
	 * @return Status - the resulting status of the operation
	 *
	 * @sa FromJSON()
	 */
	public String ToJSON()
	{
		if (fMsg == null)
			return null;

		return fMsg.ToJSON();
	}

	/**
	 * @brief This method will parse a Message from JSON.
	 * The format is the same as that which is generated by ToJSON().
	 *
	 * @param json - JSON string representation of a Message
	 * @return Status - the resulting status of the operation.
	 *
	 * @sa ToJSON()
	 */
	public Status FromJSON(String json)
	{
		if (fMsg == null)
			return uninitialized();

		return fMsg.FromJSON(json);
	}

	/**
	 * @brief Populates this message from XML given the first Node in the XML document
	 * @note This method has been deprecated.
	 * @param n The first node
	 * @return A Status object denoting success or failure 
	 */
	public Status FromXML(Node n)
	{
		Status result = new Status();

		String nodeName = n.getNodeName();

		if (nodeName.equals("MESSAGE"))
		{
			// get attributes
			NamedNodeMap attr = n.getAttributes();

			if (attr != null)
			{
				Node kindNode    = attr.getNamedItem("KIND");
				Node subjectNode = attr.getNamedItem("SUBJECT");

				if (kindNode != null)
				{
					String kind = kindNode.getNodeValue();

					if (kind.equalsIgnoreCase("REQUEST"))
					{
						this.SetKind(gmsecAPI.GMSEC_MSG_REQUEST);
					}
					else if (kind.equalsIgnoreCase("REPLY"))
					{
						this.SetKind(gmsecAPI.GMSEC_MSG_REPLY);
					}
					else
					{
						this.SetKind(gmsecAPI.GMSEC_MSG_PUBLISH);
					}
				}

				if (subjectNode != null)
				{
					String subject = subjectNode.getNodeValue();

					this.SetSubject(subject);
				}
			}


			NodeList nl = n.getChildNodes();

			if (nl != null)
			{
				for (int i = 0; i < nl.getLength(); i++)
				{
					Node node = nl.item(i);

					// search for field nodes
					if (node != null && node.getNodeName().equals("FIELD"))
					{
						//get attributes
						NamedNodeMap nm = node.getAttributes();

						if (nm != null)
						{
							Node   nameNode = nm.getNamedItem("NAME");
							Node   typeNode = nm.getNamedItem("TYPE");
							String value    = node.getNodeValue();

							if (nameNode != null && typeNode != null && value != null)
							{
								String name = nameNode.getNodeValue();
								String type = typeNode.getNodeValue();

								// add to message
								Field field = new Field();

								field.SetName(name);
								field.SetValue(Field.LookupType(type), value);

								this.AddField(field);
							}
						}
					}
				}
			}
		}
		else
		{
			return new Status(gmsecAPI.GMSEC_STATUS_MESSAGE_ERROR, gmsecAPI.GMSEC_OTHER_ERROR,
				"Parsing XML failed, invalid node: " + nodeName);
		}

		return result;
	}

	/**
	 * @brief This method will return the message size
	 * @param size - a GMSEC_ULong that indicates the message size
	 * @return Status - the resulting status of the operation
	 */
	public Status GetMSGSize(GMSEC_ULong size)
	{
		if (fMsg == null)
			return uninitialized();

		if (size == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_MESSAGE_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Size is NULL");
		}

		return fMsg.GetMSGSize(size);
	}

	public String GetLibraryRootName()
	{
		if (fMsg == null)
			return null;

		return fMsg.GetLibraryRootName();
	}

	public static String GetTime()
	{
		Date now = new Date();
		SimpleDateFormat df = new SimpleDateFormat("yyyy-DDD-kk:mm:ss.SSS");
		df.setTimeZone(TimeZone.getTimeZone("UTC"));
		return df.format(now);
	}

	/** 
	 * @brief This function will remove all fields form this message.
	 * @note <b>This may not be available in all middleware implementations.</b>
	 *
	 * @return status - result of the operation
	 */
	public Status ClearFields()
	{
		if (fMsg == null)
			return uninitialized();

		return fMsg.ClearFields();
	}

	/** 
	 * @brief This function will remove a field from this message by name.
	 * @note <b>This may not be available in all middleware implementations.</b>
	 *
	 * @param name - name of field to remove
	 * @return status - result of the operation
	 */
	public Status ClearField(String name)
	{
		if (fMsg == null)
			return uninitialized();

		return fMsg.ClearField(name);
	}

	public String toString()
	{
		if (fMsg == null)
			return new String("[null Message]");

		GMSEC_String xml = new GMSEC_String();
		ToXML(xml);
		return xml.Get();
	}

	public Status CopyFields(Message message)
	{
		if (fMsg == null)
			return uninitialized();

		Status status = new Status();
		Field field = new Field();

		status = message.GetFirstField(field);

		while (status.IsError() == false)
		{
			status = this.AddField(field);
			if (status.IsError())
			{
				status.Set(gmsecAPI.GMSEC_STATUS_MESSAGE_ERROR, gmsecAPI.GMSEC_OTHER_ERROR, "Error adding field while cloning message");
				return status;
			}

			status = message.GetNextField(field);
		}

		Status ok = new Status();
		return ok;
	}

	/**
	 * @fn SetTracking(short flag)
	 *
	 * @note This method has been deprecated; do NOT use it.
	 */
	public void SetTracking(short flag)
	{
		if (fMsg != null)
		{
			fMsg.SetTracking(flag);
		}
	}

	/**
	 * @fn SetTrackingNode(short flag)
	 *
	 * @note This method has been deprecated; do NOT use it.
	 */
	public void SetTrackingNode(short flag)
	{
		if (fMsg != null)
		{
			fMsg.SetTrackingNode(flag);
		}
	}

	/**
	 * @fn SetTrackingProcessId(short flag)
	 *
	 * @note This method has been deprecated; do NOT use it.
	 */
	public void SetTrackingProcessId(short flag)
	{
		if (fMsg != null)
		{
			fMsg.SetTrackingProcessId(flag);
		}
	}

	/**
	 * @fn SetTrackingConnectionId(short flag)
	 *
	 * @note This method has been deprecated; do NOT use it.
	 */
	public void SetTrackingConnectionId(short flag)
	{
		if (fMsg != null)
		{
			fMsg.SetTrackingConnectionId(flag);
		}
	}

	/**
	 * @fn SetTrackingUserName(short flag)
	 *
	 * @note This method has been deprecated; do NOT use it.
	 */
	public void SetTrackingUserName(short flag)
	{
		if (fMsg != null)
		{
			fMsg.SetTrackingUserName(flag);
		}
	}

	/**
	 * @fn SetTrackingPublishTime(short flag)
	 *
	 * @note This method has been deprecated; do NOT use it.
	 */
	public void SetTrackingPublishTime(short flag)
	{
		if (fMsg != null)
		{
			fMsg.SetTrackingPublishTime(flag);
		}
	}

	/**
	 * @fn SetTrackingUniqueId(short flag)
	 *
	 * @note This method has been deprecated; do NOT use it.
	 */
	public void SetTrackingUniqueId(short flag)
	{
		if (fMsg != null)
		{
			fMsg.SetTrackingUniqueId(flag);
		}
	}

	/**
	 * @fn SetTrackingMwInfo(short flag)
	 *
	 * @note This method has been deprecated; do NOT use it.
	 */
	public void SetTrackingMwInfo(short flag)
	{
		if (fMsg != null)
		{
			fMsg.SetTrackingMwInfo(flag);
		}
	}


	/**
	 * @fn GetTracking()
	 *
	 * @note This method has been deprecated; do NOT use it.
	 *
	 * @return short MESSAGE_TRACKINGFIELDS_UNSET, MESSAGE_TRACKINGFIELDS_ON, or MESSAGE_TRACKINGFIELDS_OFF
	 */
	public short GetTracking()
	{
		short result = gmsecAPI.MESSAGE_TRACKINGFIELDS_UNSET;
		if (fMsg != null)
		{
			result = fMsg.GetTracking();
		}
		return result;
	}

	/**
	 * @fn GetTrackingNode()
	 *
	 * @note This method has been deprecated; do NOT use it.
	 *
	 * @return short MESSAGE_TRACKINGFIELDS_UNSET, MESSAGE_TRACKINGFIELDS_ON, or MESSAGE_TRACKINGFIELDS_OFF
	 */
	public short GetTrackingNode()
	{
		short result = gmsecAPI.MESSAGE_TRACKINGFIELDS_UNSET;
		if (fMsg != null)
		{
			result = fMsg.GetTrackingNode();
		}
		return result;
	}

	/**
	 * @fn GetTrackingProcessId()
	 *
	 * @note This method has been deprecated; do NOT use it.
	 *
	 * @return short MESSAGE_TRACKINGFIELDS_UNSET, MESSAGE_TRACKINGFIELDS_ON, or MESSAGE_TRACKINGFIELDS_OFF
	 */
	public short GetTrackingProcessId()
	{
		short result = gmsecAPI.MESSAGE_TRACKINGFIELDS_UNSET;
		if (fMsg != null)
		{
			result = fMsg.GetTrackingProcessId();
		}
		return result;
	}

	/**
	 * @fn GetTrackingConnectionId()
	 *
	 * @note This method has been deprecated; do NOT use it.
	 *
	 * @return short MESSAGE_TRACKINGFIELDS_UNSET, MESSAGE_TRACKINGFIELDS_ON, or MESSAGE_TRACKINGFIELDS_OFF
	 */
	public short GetTrackingConnectionId()
	{
		short result = gmsecAPI.MESSAGE_TRACKINGFIELDS_UNSET;
		if (fMsg != null)
		{
			result = fMsg.GetTrackingConnectionId();
		}
		return result;
	}

	/**
	 * @fn GetTrackingUserName()
	 *
	 * @note This method has been deprecated; do NOT use it.
	 *
	 * @return short MESSAGE_TRACKINGFIELDS_UNSET, MESSAGE_TRACKINGFIELDS_ON, or MESSAGE_TRACKINGFIELDS_OFF
	 */
	public short GetTrackingUserName()
	{
		short result = gmsecAPI.MESSAGE_TRACKINGFIELDS_UNSET;
		if (fMsg != null)
		{
			result = fMsg.GetTrackingUserName();
		}
		return result;
	}

	/**
	 * @fn GetTrackingPublishTime()
	 *
	 * @note This method has been deprecated; do NOT use it.
	 *
	 * @return short MESSAGE_TRACKINGFIELDS_UNSET, MESSAGE_TRACKINGFIELDS_ON, or MESSAGE_TRACKINGFIELDS_OFF
	 */
	public short GetTrackingPublishTime()
	{
		short result = gmsecAPI.MESSAGE_TRACKINGFIELDS_UNSET;
		if (fMsg != null)
		{
			result = fMsg.GetTrackingPublishTime();
		}
		return result;
	}

	/**
	 * @fn GetTrackingUniqueId()
	 *
	 * @note This method has been deprecated; do NOT use it.
	 *
	 * @return short MESSAGE_TRACKINGFIELDS_UNSET, MESSAGE_TRACKINGFIELDS_ON, or MESSAGE_TRACKINGFIELDS_OFF
	 */
	public short GetTrackingUniqueId()
	{
		short result = gmsecAPI.MESSAGE_TRACKINGFIELDS_UNSET;
		if (fMsg != null)
		{
			result = fMsg.GetTrackingUniqueId();
		}
		return result;
	}

	/**
	 * @fn GetTrackingMwInfo()
	 *
	 * @note This method has been deprecated; do NOT use it.
	 *
	 * @return short MESSAGE_TRACKINGFIELDS_UNSET, MESSAGE_TRACKINGFIELDS_ON, or MESSAGE_TRACKINGFIELDS_OFF
	 */
	public short GetTrackingMwInfo()
	{
		short result = gmsecAPI.MESSAGE_TRACKINGFIELDS_UNSET;
		if (fMsg != null)
		{
			result = fMsg.GetTrackingMwInfo();
		}
		return result;
	}

	private Status uninitialized() {
		return new Status(
			gmsecAPI.GMSEC_STATUS_MESSAGE_ERROR,
			gmsecAPI.GMSEC_UNINITIALIZED_OBJECT,
			"Uninitialized Message");
	}
}
