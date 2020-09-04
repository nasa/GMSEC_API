/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;


interface Example
{
	abstract boolean run() throws GMSEC_Exception;

	abstract boolean cleanup() throws GMSEC_Exception;
}
