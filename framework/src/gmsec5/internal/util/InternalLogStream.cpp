/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec5/internal/util/InternalLogStream.h>

using namespace gmsec::api5::internal;


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
