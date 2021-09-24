
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






package bolt;

public class IntegerProperty
		extends Property
{
	private int value;
	
	
	public IntegerProperty (String name, int value) {
		super(name);
		this.value = value;
		setType(PropertyType.I32);
	}
	
	int getValue () {
		return value;
	}
	

	public double asDouble () {
		return getValue();
	}


	public int asInteger () {
		return getValue();
	}


	public String asString () {
		return "" + getValue();
	}
	
	public String toString () {
		return getName() + ": " + value + " [integer]";
	}
	
	int dataSize () {
		return 4;
	}
}
