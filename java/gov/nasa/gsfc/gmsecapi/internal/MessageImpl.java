/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







package gov.nasa.gsfc.gmsecapi.internal;

import gov.nasa.gsfc.gmsecapi.*;

import org.w3c.dom.Node;

/** @interface MessageImpl
*
* @brief Definition of Message interface
*/
public interface MessageImpl
{
	public boolean isValid();

	public Status SetKind(int kind);

	public Status GetKind(GMSEC_UShort kind);

	public Status SetSubject(String subject);

	public Status GetSubject(GMSEC_String subject);

	public Status ClearFields();

	public Status ClearField(String name);

	public Status AddField(Field field);

	public Status GetField(String name, Field field);

	public Status GetFieldCount(GMSEC_Long count);

	public Status GetFirstField(Field field);

	public Status GetNextField(Field field);

	public Status SetConfig(Config config);

	public Status ToXML(GMSEC_String outst);

	public Status FromXML(String inst);

	public Status FromXML(Node n);

	public String ToJSON();

	public Status FromJSON(String inst);

	public Status GetMSGSize(GMSEC_ULong size);

	public String GetLibraryRootName();

	public short GetTracking();

	public short GetTrackingNode();

	public short GetTrackingProcessId();

	public short GetTrackingConnectionId();

	public short GetTrackingUserName();

	public short GetTrackingPublishTime();

	public short GetTrackingUniqueId();

	public short GetTrackingMwInfo();

	public void SetTracking(short s);

	public void SetTrackingNode(short s);

	public void SetTrackingProcessId(short s);

	public void SetTrackingConnectionId(short s);

	public void SetTrackingUserName(short s);

	public void SetTrackingPublishTime(short s);

	public void SetTrackingUniqueId(short s);

	public void SetTrackingMwInfo(short s);
}
