
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






package bolt;

public class DoubleProperty
		extends Property
{
	private double value;
	
	public DoubleProperty (String name, double value) {
		super(name);
		this.value = value;
		setType(PropertyType.F64);
	}
	
	
	double getValue () {
		return value;
	}
	

	public double asDouble () {
		return getValue();
	}


	public int asInteger () {
		return (int) getValue();
	}


	public String asString () {
		return "" + getValue();
	}
	
	public String toString () {
		return getName() + ": " + value + " [real]";
	}
	
	int dataSize () {
		return 8;
	}
}
