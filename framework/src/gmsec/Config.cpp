
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/**
 *  @file Config.cpp
 *
 *  Opaque Config class
 *
 *
*/

#include <gmsec/Config.h>
#include <gmsec/internal/BaseConfig.h>


namespace gmsec
{

using namespace gmsec::internal;


Config::Config()
		:
		ptr(new BaseConfig())
{
}


Config::Config(int argc, char* argv[])
		:
		ptr(new BaseConfig(argc, argv))
{
}


Config::Config(const char *xml)
		:
		ptr(new BaseConfig(xml))
{
}


Config::Config(const Config &other)
		:
		ptr(new BaseConfig(*other.ptr))
{
}


Config::~Config()
{
	delete ptr;
	ptr = 0;
}


Status Config::Clear()
{
	return ptr->Clear();
}


Status Config::AddValue(const char *name, const char *value)
{
	return ptr->AddValue(name, value);
}


Status Config::ClearValue(const char *name)
{
	return ptr->ClearValue(name);
}


Status Config::GetValue(const char *name, const char *&value) const
{
	return ptr->GetValue(name, value);
}


bool Config::CheckBoolValue(const char *name, bool defaultValue) const
{
	return ptr->CheckBoolValue(name, defaultValue);
}


Status Config::GetFirst(const char *&name, const char *&value) const
{
	return ptr->GetFirst(name, value);
}


Status Config::GetNext(const char *&name, const char *&value) const
{
	return ptr->GetNext(name, value);
}


Status Config::ToXML(const char *&xml)
{
	return ptr->ToXML(xml);
}


Status Config::FromXML(const char *xml)
{
	return ptr->FromXML(xml);
}

} // namespace gmsec
