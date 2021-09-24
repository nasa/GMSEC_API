/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



// Managed
#include <field\Field_Net.h>

#include <GMSEC_Exception_Net.h>


// Native
#include <gmsec4/Exception.h>


using namespace GMSEC::API;
using namespace System;


Field::~Field()
{
	m_child = nullptr;
}


String^ Field::GetName()
{
	return gcnew String(m_child->getName());
}


Field::FieldType Field::GetType()
{
	return GetFieldType();
}


Field::FieldType Field::GetFieldType()
{
	return static_cast<FieldType>(m_child->getType());
}


bool Field::IsHeader()
{
	return m_child->isHeader();
}


String^ Field::ToXML()
{
	return gcnew String(m_child->toXML());
}


String^ Field::ToJSON()
{
	return gcnew String(m_child->toJSON());
}


Int64 Field::GetIntegerValue()
{
	try
	{
		return m_child->getIntegerValue();
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


UInt64 Field::GetUnsignedIntegerValue()
{
	try
	{
		return m_child->getUnsignedIntegerValue();
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


double Field::GetDoubleValue()
{
	try
	{
		return m_child->getDoubleValue();
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


String^ Field::GetStringValue()
{
	try
	{
		return gcnew String(m_child->getStringValue());
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


void Field::RegisterChild(gmsec::api::Field* child)
{
	m_child = child;
}


gmsec::api::Field* Field::GetChild()
{
	return m_child;
}
