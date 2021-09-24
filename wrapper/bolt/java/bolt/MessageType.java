
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






package bolt;

public enum MessageType {

	VOID(0), WELCOME(1), GOODBYE(2), ECHO(3),
	ERROR(4), ACK(5),
	
	NEGOTIATE(10),
	STATISTICS(11),

	SUBSCRIBE(20),
	UNSUBSCRIBE(21),
	
	PUBLISH(22) {
		boolean isDistributable () {
			return true;
		}
	},
	REQUEST(23) {
		boolean isDistributable () {
			return true;
		}
	},
	REPLY(24) {
		boolean isDistributable () {
			return true;
		}
	};


	private int identifier;

	MessageType (int id) {
		identifier = id;
	}

	int identifier () {
		return this.identifier;
	}

	boolean isDistributable () {
		return false;
	}

	static MessageType find (int x) {
		for (MessageType type : MessageType.values())
			if (type.identifier == x)
				return type;
		return VOID;
	}
}
