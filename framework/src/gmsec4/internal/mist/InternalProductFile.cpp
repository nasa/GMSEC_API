/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file InternalProductFile.h
 *
 *  @brief This file contains declarations for ProductFile containers
**/


#include <gmsec4/internal/mist/InternalProductFile.h>
#include <gmsec4/Exception.h>
#include <gmsec4/Errors.h>
#include <cstring>


using namespace gmsec::api;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;


InternalProductFile::InternalProductFile(const char* pName,
                                         const char* pDescription,
                                         const char *pVersion,
                                         const char* pFormat,
                                         const char* pUri)
{
	init(pName, pDescription, pVersion, pFormat, pUri, NULL, 0);
}


InternalProductFile::InternalProductFile(const char* pName,
                                         const char* pDescription,
                                         const char* pVersion,
                                         const char* pFormat,
                                         GMSEC_BIN   pData,
                                         size_t pFileSize)
{
	init(pName, pDescription, pVersion, pFormat, NULL, pData, pFileSize);
}


InternalProductFile::InternalProductFile(const InternalProductFile& other)
{
	if (other.m_hasUri)
	{
		init(other.m_name.c_str(), other.m_description.c_str(), other.m_version.c_str(), other.m_format.c_str(), other.m_uri.c_str(), NULL, 0);
	}
	else
	{
		init(other.m_name.c_str(), other.m_description.c_str(), other.m_version.c_str(), other.m_format.c_str(), NULL, other.m_data, other.m_fileSize);
	}
}

InternalProductFile& InternalProductFile::operator=(const InternalProductFile& other)
{
    if (this != &other)
    {
        if (other.m_hasUri)
		{
			init(other.m_name.c_str(), other.m_description.c_str(), other.m_version.c_str(), other.m_format.c_str(), other.m_uri.c_str(), NULL, 0);
		}
		else
		{
			init(other.m_name.c_str(), other.m_description.c_str(), other.m_version.c_str(), other.m_format.c_str(), NULL, other.m_data, other.m_fileSize);
		}
    }
 
    return *this;
}


InternalProductFile::~InternalProductFile()
{
	if (m_data)
	{
		delete[] m_data;
	}
}


void InternalProductFile::init(const char* pName,
                               const char* pDescription,
                               const char* pVersion,
                               const char* pFormat,
                               const char* pUri,
                               GMSEC_BIN   pData,
                               size_t      pFileSize)
{
	if (pName && !std::string(pName).empty())
	{
		m_name = pName;
	}
	else
	{
		throw Exception(MIST_ERROR,
		                OTHER_ERROR_CODE,
		                "InternalProductFile constructor:  NULL or empty name provided.");
	}

	if (pDescription && !std::string(pDescription).empty())
	{
		m_description = pDescription;
	}
	else
	{
		throw Exception(MIST_ERROR,
		                OTHER_ERROR_CODE,
		                "InternalProductFile constructor:  NULL or empty description provided.");
	}

	if (pVersion && !std::string(pVersion).empty())
	{
		m_version = pVersion;
	}
	else
	{
		throw Exception(MIST_ERROR,
		                OTHER_ERROR_CODE,
		                "InternalProductFile constructor:  NULL or empty version provided.");
	}

	if (pFormat && !std::string(pFormat).empty())
	{
		m_format = pFormat;
	}
	else
	{
		throw Exception(MIST_ERROR,
		                OTHER_ERROR_CODE,
		                "InternalProductFile constructor:  NULL or empty format provided.");
	}

	if (pUri && !std::string(pUri).empty())
	{
		m_uri = pUri;
		m_hasUri = true;
	}
	else
	{
		m_hasUri = false;
	}

	if (pData && pFileSize > 0)
	{
		try
		{
			m_data     = new unsigned char[pFileSize];
			m_fileSize = pFileSize;

			for (size_t idx = 0; idx < m_fileSize; idx++)
			{
				m_data[idx] = pData[idx];
			}
		}
		catch (...)
		{
			throw Exception(MIST_ERROR, OUT_OF_MEMORY, "Unable to allocate memory for Product File contents");
		}
	}
	else
	{
		m_data = NULL;
		m_fileSize = 0;
	}
}


const char* InternalProductFile::getName() const
{
	return m_name.c_str();
}


const char* InternalProductFile::getDescription() const
{
	return m_description.c_str();
}


const char* InternalProductFile::getVersion() const
{
	return m_version.c_str();
}


const char* InternalProductFile::getFormat() const
{
	return m_format.c_str();
}


bool InternalProductFile::uriAvailable() const
{
	return m_hasUri;
}


const char* InternalProductFile::getURI() const
{
	if (m_hasUri)
	{
		return m_uri.c_str();
	}
	else
	{
		return NULL;
	}
}


bool InternalProductFile::contentsAvailable() const
{
	if (m_data)
	{
		return true;
	}
	else
	{
		return false;
	}
}


size_t InternalProductFile::getContents(GMSEC_BIN& contents) const
{
	if (m_data)
	{
		contents = m_data;
	}
	else
	{
		throw Exception(MIST_ERROR,
		                OTHER_ERROR_CODE,
		                "InternalProductFile::getFileContents() :  NULL  file contents.");
	}

	return m_fileSize;
}


bool InternalProductFile::operator==(const InternalProductFile& productFile) const
{
	bool ret_val = false;

	if (m_name        == productFile.m_name &&
	    m_description == productFile.m_description &&
	    m_version     == productFile.m_version &&
	    m_format      == productFile.m_format &&
	    std::memcmp(m_data, productFile.m_data, m_fileSize) == 0 &&
	    m_fileSize    == productFile.m_fileSize &&
	    m_hasUri      == productFile.m_hasUri &&
	    m_uri         == productFile.m_uri)
	{
		ret_val = true;
	}

	return ret_val;

}


bool InternalProductFile::operator!=(const InternalProductFile& productFile) const
{
	return !(operator==(productFile));

}
