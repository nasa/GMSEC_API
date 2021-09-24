/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file ProductFile.cpp
 *
 *  @brief This file contains definitions for ProductFile containers
**/

#include <gmsec4/mist/ProductFile.h>
#include <gmsec4/internal/mist/InternalProductFile.h>
#include <string>
#include <vector>


using namespace gmsec::api;
using namespace gmsec::api::mist;


ProductFile::ProductFile(const char* pName,
                         const char* pDescription,
                         const char* pVersion,
                         const char* pFormat,
                         const char* pUri)
           : m_iProductFile(new internal::InternalProductFile(pName, pDescription, pVersion, pFormat, pUri))
{
}


ProductFile::ProductFile(const char* pName,
                         const char* pDescription,
                         const char* pVersion,
                         const char* pFormat,
                         GMSEC_BIN   pData,
                         size_t      pFileSize)
           : m_iProductFile(new internal::InternalProductFile(pName, pDescription, pVersion, pFormat, pData, pFileSize))
{
}


ProductFile::ProductFile(const ProductFile& other)
           : m_iProductFile(new internal::InternalProductFile(*(other.m_iProductFile)))
{
}

ProductFile& ProductFile::operator=(const ProductFile& other)
{
	if (this != &other)
    {
        delete m_iProductFile;
 
        m_iProductFile = new internal::InternalProductFile(*other.m_iProductFile);
    }
 
    return *this;
}


ProductFile::~ProductFile()
{
	delete m_iProductFile;
}


const char* ProductFile::getName() const
{
	return m_iProductFile->getName();
}


const char* ProductFile::getDescription() const
{
	return m_iProductFile->getDescription();
}


const char* ProductFile::getVersion() const
{
	return m_iProductFile->getVersion();
}


const char* ProductFile::getFormat() const
{
	return m_iProductFile->getFormat();
}


bool ProductFile::uriAvailable() const
{
	return m_iProductFile->uriAvailable();
}


const char* ProductFile::getURI() const
{
	return m_iProductFile->getURI();
}


bool ProductFile::contentsAvailable() const
{
	return m_iProductFile->contentsAvailable();
}


size_t ProductFile::getContents(GMSEC_BIN& ptr) const
{
	return m_iProductFile->getContents(ptr);
}


bool ProductFile::operator==(const ProductFile& productFile) const
{
	return m_iProductFile->operator==(*(productFile.m_iProductFile));
}


bool ProductFile::operator!=(const ProductFile& productFile) const
{
	return m_iProductFile->operator!=(*(productFile.m_iProductFile));
}
