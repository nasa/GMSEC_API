
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






package bolt;

public class FlagProperty
		extends Property
{
	private boolean value;


	public FlagProperty (String name, boolean value) {
		super(name);
		this.value = value;
		setType(PropertyType.FLAG);
	}

	boolean getValue () {
		return value;
	}

	public double asDouble () {
		 return value ? 1 : 0;
	}


	public int asInteger () {
		 return value ? 1 : 0;
	}


	public String asString () {
		return "" + getValue();
	}
	
	public String toString () {
		return getName() + ": " + value + " [flag]";
	}
	
	int dataSize () {
		return 1;
	}
}
