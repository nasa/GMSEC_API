
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






package bolt;

public class Statistics
{	
	private long bytesRead;
	private long bytesWritten;
	private long messagesSent;
	private long messagesRead;
	
	
	
	synchronized void takeSnapshot (Statistics snapshot) {
		snapshot.bytesRead = bytesRead;
		snapshot.bytesWritten = bytesWritten;
		
		snapshot.messagesRead = messagesRead;
		snapshot.messagesSent = messagesSent;
	}
	
	void addBytesRead (int count) {
		bytesRead += count;
	}
	
	void addBytesWritten (int count) {
		bytesWritten += count;
	}
	
	void readMessage () {
		++messagesRead;
	}

	void sentMessage () {
		++messagesSent;
	}
	
	
	long getBytesRead () {
		return bytesRead;
	}
	
	long getBytesWritten () {
		return bytesWritten;
	}
	
	long getMessagesRead () {
		return messagesRead;
	}
	
	long getMessagesSent () {
		return messagesSent;
	}
}
