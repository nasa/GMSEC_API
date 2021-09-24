
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






package bolt;

public abstract class Property
{
	private String name;
	private PropertyType type;


	Property (String name) {
		this.name = name;
	}
	
	String getName () {
		return name;
	}

	PropertyType getType () {
		return type;
	}
	
	void setType (PropertyType type) {
		this.type = type;
	}
	
	
	public abstract int asInteger ();
	
	public abstract String asString ();
	
	public abstract double asDouble ();

	abstract int dataSize();


	public static StringProperty createID (String id) {
		StringProperty p = new StringProperty("ID", id);
		p.setType(PropertyType.ID);
		return p;
	}

	public static StringProperty createTopic (String topic) {
		StringProperty p = new StringProperty("TOPIC", topic);
		p.setType(PropertyType.TOPIC);
		return p;
	}

	public static StringProperty createCorrID (String id) {
		StringProperty p = new StringProperty("CORR_ID", id);
		p.setType(PropertyType.CORR_ID);
		return p;
	}

	public static StringProperty createReplyTo (String topic) {
		StringProperty p = new StringProperty("REPLY_TO", topic);
		p.setType(PropertyType.REPLY_TO);
		return p;
	}

	public static StringProperty createSelector (String selector) {
		StringProperty p = new StringProperty("SELECTOR", selector);
		p.setType(PropertyType.SELECTOR);
		return p;
	}

	public static IntegerProperty createCompress (boolean z) {
		IntegerProperty p = new IntegerProperty("COMPRESS", z ? 1 : 0);
		p.setType(PropertyType.COMPRESS);
		return p;
	}

	public static StringProperty createError (String s) {
		StringProperty p = new StringProperty("ERROR", s);
		// p.setType(PropertyType.ERROR);
		return p;
	}



	void getEncoding(Encoding encoding) {
		encoding.prefix = 1;	// (Mode<<6)|id
		encoding.count = dataSize();

		encoding.count += 1;	// name length
		if (type.isNamed())
			encoding.count += name.length();

		if (encoding.count < 256) {
			encoding.mode = Mode.SMALL;
			encoding.prefix += 1;
		}
		else if (encoding.count < 256*256) {
			encoding.mode = Mode.MEDIUM;
			encoding.prefix += 2;
		}
		else if (encoding.count < 256*256*256) {
			encoding.mode = Mode.LARGE;
			encoding.prefix += 3;
		}
		else
			throw new IllegalStateException("excessive length: " + encoding.count);
	}


	enum Mode
	{
		SMALL(0),
		MEDIUM(1),
		LARGE(2);

		private int mode;

		Mode(int mode) {
			this.mode = mode;
		}
		int value() {
			return mode;
		}
	}

	public static class Encoding
	{
		Mode mode;
		int prefix;
		int count;
	}
}
