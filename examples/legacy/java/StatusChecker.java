/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/** 
 * @file StatusChecker.java
 *
 * A utility for performing GMSEC status checks.
 */


import gov.nasa.gsfc.gmsecapi.Status;
import gov.nasa.gsfc.gmsecapi.util.Log;


class StatusChecker
{
	private String tag;


	StatusChecker() {
		tag = "unknown";
	}


	void setTag(String tag) {
		this.tag = tag;
	}

	boolean check(Status status) throws ExampleException {

		if (!checkNoThrow(status))
			throw new ExampleException(tag + ": " + status.Get());

		return true;
	}

	boolean checkNoThrow(Status status) {
		if (!status.isError())
			return true;

		Log.Warning(tag + ": " + status.Get());

		return false;
	}

	StatusChecker calling(String tag) {
		setTag(tag);
		Log.Verbose("calling " + tag);
		return this;
	}

}

