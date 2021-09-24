
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





package bolt;

import java.util.logging.Logger;

public class StringProperty
		extends Property
{
	String value;
	
	private static Logger source = Log.createSource("StringProperty");

	public StringProperty(String name, String value) {
		super(name);
		this.value = value;
		setType(PropertyType.STRING);
		// 
		int slen = value.length();
		int blen = value.getBytes().length;
		if (slen != blen)
			Log.detail(source, this + " is funky: String.length=" + slen + " array.length=" + blen);
	}

	String getValue() {
		return value;
	}

	public double asDouble() {
		return Double.parseDouble(getValue());
	}

	public int asInteger() {
		return Integer.parseInt(getValue());
	}

	public String asString() {
		return getValue();
	}

	public String toString() {
		return getName() + ": " + value + " [string]";
	}

	int dataSize() {
		return value.getBytes().length;
	}
}
