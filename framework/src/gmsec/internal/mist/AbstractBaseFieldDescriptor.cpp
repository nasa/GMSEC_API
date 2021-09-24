
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file AbstractBaseFieldDescriptor.cpp
 *
 *  @brief This file contains the AbstractBaseFieldDescriptor class.
 *
**/

#include <gmsec/internal/mist/AbstractBaseFieldDescriptor.h>

#include <gmsec/mist/MIST_errors.h>

#include <gmsec/internal/mist/Context.h>

#include <gmsec/internal/strutil.h>


using namespace gmsec;
using namespace gmsec::internal;
using namespace gmsec::mist::internal;



/*******************************************************/

AbstractBaseFieldDescriptor::AbstractBaseFieldDescriptor()
	: fname(std::string("unset")),
	fmode(F_OPTIONAL),
	fclass(F_NONE),
	fdescription(std::string("unset"))
{
}

AbstractBaseFieldDescriptor::~AbstractBaseFieldDescriptor()
{
}

Status AbstractBaseFieldDescriptor::GetName(const char * &name) const
{
	Status status;

	if(fname.compare(std::string("unset")) != 0)
	{
		name = util::stringNew(fname.c_str());
	}
	else
	{
		status.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_UNSET_FIELD, "Name has not been set yet");
	}

	return status;
}

FieldMode AbstractBaseFieldDescriptor::GetMode()const
{
	return fmode;
}

FieldClass AbstractBaseFieldDescriptor::GetClass() const
{
	return fclass;
}

Status AbstractBaseFieldDescriptor::GetDescription(const char * &description) const
{
	Status status;

	if(fdescription.compare(std::string("unset")) != 0)
	{
		description = util::stringNew(fdescription.c_str());
	}
	else
	{
		status.Set(GMSEC_GENERAL_MIST_ERROR, GMSEC_UNSET_FIELD, "Description has not been set yet");
	}

	return status;
}

Status AbstractBaseFieldDescriptor::SetFieldVariables(const char * name, FieldMode mode, FieldClass clss, const char * description)
{
	Status status;

	if (name)
	{
		if (!std::string(name).empty())
		{
			fname = name;

			fmode = mode;
			fclass = clss;

			if (description)
			{
				fdescription = description;
			}
			else
			{
				fdescription = "";
			}
		}
		else
		{
			status.Set(GMSEC_GENERAL_MIST_ERROR,
			           GMSEC_SCHEMA_FAILED_TO_PARSE,
			           "Field name was empty");
		}
	}
	else
	{
		status.Set(GMSEC_GENERAL_MIST_ERROR,
		           GMSEC_SCHEMA_FAILED_TO_PARSE,
		           "Field name was missing");
	}

	return status;
}

Status AbstractBaseFieldDescriptor::SetName(const char * name)
{
	Status status;
	fname = std::string(name);
	return status;
}

Status AbstractBaseFieldDescriptor::SetMode(FieldMode mode)
{
	Status status;
	fmode = mode;
	return status;
}

Status AbstractBaseFieldDescriptor::SetClass(FieldClass clss)
{
	Status status;
	fclass = clss;
	return status;
}

Status AbstractBaseFieldDescriptor::SetDescription(const char * description)
{
	Status status;
	fdescription = std::string(description);
	return status;
}

