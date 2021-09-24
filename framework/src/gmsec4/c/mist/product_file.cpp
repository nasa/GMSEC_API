/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file product_file.cpp
 *
 *  @brief This file contains functions for the management of a MIST ProductFile object.
 */


#include <gmsec4/c/mist/product_file.h>

#include <gmsec4/mist/ProductFile.h>

#include <gmsec4/Exception.h>
#include <gmsec4/Status.h>

using namespace gmsec::api;
using namespace gmsec::api::mist;


GMSEC_ProductFile CALL_TYPE productFileCreate(const char* name, const char* description, const char* version, const char* format, const char* uri, GMSEC_Status status)
{
	GMSEC_ProductFile pfile = NULL;
	Status            result;

	try
	{
		pfile = reinterpret_cast<GMSEC_ProductFile>(new ProductFile(name, description, version, format, uri));
	}
	catch (Exception& e)
	{
		result = Status(e);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return pfile;
}


GMSEC_ProductFile CALL_TYPE productFileCreateUsingData(const char* name, const char* description, const char* version, const char* format, const GMSEC_BIN data, size_t dataSize, GMSEC_Status status)
{
	GMSEC_ProductFile pfile = NULL;
	Status            result;

	try
	{
		pfile = reinterpret_cast<GMSEC_ProductFile>(new ProductFile(name, description, version, format, data, dataSize));
	}
	catch (Exception& e)
	{
		result = Status(e);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return pfile;
}


GMSEC_ProductFile CALL_TYPE productFileCreateCopy(const GMSEC_ProductFile other, GMSEC_Status status)
{
	GMSEC_ProductFile pfile = NULL;
	Status            result;

	const ProductFile* otherFile = reinterpret_cast<const ProductFile*>(other);

	if (otherFile)
	{
		pfile = reinterpret_cast<GMSEC_ProductFile>(new ProductFile(*otherFile));
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ProductFile handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return pfile;
}


void CALL_TYPE productFileDestroy(GMSEC_ProductFile* productFile)
{
	if (productFile && *productFile)
	{
		ProductFile* tmp = reinterpret_cast<ProductFile*>(*productFile);

		delete tmp;

		*productFile = NULL;
	}
}


const char* CALL_TYPE productFileGetName(const GMSEC_ProductFile productFile, GMSEC_Status status)
{
	const char* name = NULL;
	Status      result;

	const ProductFile* pfile = reinterpret_cast<const ProductFile*>(productFile);

	if (pfile)
	{
		name = pfile->getName();
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ProductFile handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return name;
}


const char* CALL_TYPE productFileGetDescription(const GMSEC_ProductFile productFile, GMSEC_Status status)
{
	const char* description = NULL;
	Status      result;

	const ProductFile* pfile = reinterpret_cast<const ProductFile*>(productFile);

	if (pfile)
	{
		description = pfile->getDescription();
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ProductFile handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return description;
}


const char* CALL_TYPE productFileGetVersion(const GMSEC_ProductFile productFile, GMSEC_Status status)
{
	const char* version = NULL;
	Status      result;

	const ProductFile* pfile = reinterpret_cast<const ProductFile*>(productFile);

	if (pfile)
	{
		version = pfile->getVersion();
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ProductFile handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return version;
}


const char* CALL_TYPE productFileGetFormat(const GMSEC_ProductFile productFile, GMSEC_Status status)
{
	const char* format = NULL;
	Status      result;

	const ProductFile* pfile = reinterpret_cast<const ProductFile*>(productFile);

	if (pfile)
	{
		format = pfile->getFormat();
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ProductFile handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return format;
}


GMSEC_BOOL CALL_TYPE productFileURIAvailable(const GMSEC_ProductFile productFile, GMSEC_Status status)
{
	GMSEC_BOOL available = GMSEC_FALSE;
	Status     result;

	const ProductFile* pfile = reinterpret_cast<const ProductFile*>(productFile);

	if (pfile)
	{
		available = (pfile->uriAvailable() ? GMSEC_TRUE : GMSEC_FALSE);
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ProductFile handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return available;
}


const char* CALL_TYPE productFileGetURI(const GMSEC_ProductFile productFile, GMSEC_Status status)
{
	const char* uri = NULL;
	Status      result;

	const ProductFile* pfile = reinterpret_cast<const ProductFile*>(productFile);

	if (pfile)
	{
		try
		{
			uri = pfile->getURI();
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ProductFile handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return uri;
}


GMSEC_BOOL CALL_TYPE productFileContentsAvailable(const GMSEC_ProductFile productFile, GMSEC_Status status)
{
	GMSEC_BOOL available = GMSEC_FALSE;
	Status     result;

	const ProductFile* pfile = reinterpret_cast<const ProductFile*>(productFile);

	if (pfile)
	{
		available = (pfile->contentsAvailable() ? GMSEC_TRUE : GMSEC_FALSE);
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ProductFile handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return available;
}


size_t CALL_TYPE productFileGetContents(const GMSEC_ProductFile productFile, GMSEC_BIN* data, GMSEC_Status status)
{
	size_t dataSize = 0;
	Status result;

	const ProductFile* pfile = reinterpret_cast<const ProductFile*>(productFile);

	if (!pfile)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ProductFile handle is NULL");
	}
	else if (!data)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "GMSEC_BIN handle is NULL");
	}
	else
	{
		try
		{
			dataSize = pfile->getContents(*data);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return dataSize;
}
