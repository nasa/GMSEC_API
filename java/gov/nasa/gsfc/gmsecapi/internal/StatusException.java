/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







package gov.nasa.gsfc.gmsecapi.internal;

import gov.nasa.gsfc.gmsecapi.Status;


/**
 * @class StatusException
 *
 * @brief This exception can be thrown by Connect constructors,
 * but is hidden by ConnectionFactory::Create().
 * 
 */
public class StatusException extends Exception
{
	private static final long serialVersionUID = 1L;

	private Status status = null;

	public StatusException(Status status)
	{
		this.status = status;
	}

	public Status getStatus()
	{
		return status;
	}

}
