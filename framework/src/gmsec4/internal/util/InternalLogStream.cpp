/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec4/internal/util/InternalLogStream.h>

using namespace gmsec::api::internal;


InternalLogStream::InternalLogStream()
	: m_oss(),
	  m_data()
{
}


InternalLogStream::~InternalLogStream()
{
}


const char* InternalLogStream::getText() const
{
	m_data = m_oss.str();
	return m_data.c_str();
}
