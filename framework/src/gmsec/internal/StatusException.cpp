
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/**
 *  @file StatusException.cpp
 *
 *  This file contains the StatusException class.
*/

#include <gmsec/Status.h>
#include <gmsec/internal/StatusException.h>

using gmsec::Status;
using gmsec::internal::StatusException;

StatusException::StatusException() : Status()
{
}

StatusException::StatusException(GMSEC_STATUS_CLASS eclass, unsigned int code, const char *text) : Status(eclass,code,text)
{
}

StatusException::StatusException(const StatusException &st) : Status(st)
{
}

