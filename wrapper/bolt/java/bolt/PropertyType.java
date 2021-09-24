
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






package bolt;

public enum PropertyType
{

	STRING(1, true),
	BLOB(2, true),
	I32(3, true),
	F64(4, true),
	FLAG(5, true),

	ID(21),
	TOPIC(22),
	CORR_ID(23),
	REPLY_TO(24),
	SELECTOR(25),

	COMPRESS(27);


	private int identifier;
	private boolean named;


	PropertyType(int id) {
		this(id, false);
	}

	PropertyType(int id, boolean named) {
		this.identifier = id;
		this.named = named;
	}

	int identifier () {
		return identifier;
	}

	boolean isNamed () {
		return named;
	}


	static PropertyType find (int id) {
		for (PropertyType type : PropertyType.values())
			if (type.identifier == id)
				return type;
		return null;
	}
}
