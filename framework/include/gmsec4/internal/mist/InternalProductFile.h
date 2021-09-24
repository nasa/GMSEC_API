/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file InernalProductFile.h
 *
 *  @brief This file contains declarations for ProductFile containers
**/

#ifndef GMSEC_API_INTERNALPRODUCTFILE_H
#define GMSEC_API_INTERNALPRODUCTFILE_H

#include <gmsec4/util/wdllexp.h>

#include <gmsec4_defs.h>

#include <string>
#include <vector>

namespace gmsec
{
namespace api
{
namespace mist
{
namespace internal
{

class GMSEC_API InternalProductFile
{
public:
	
	InternalProductFile(const char * pName, const char * pDescription, const char * pVersion, const char* pFormat, const char * pUri);

	InternalProductFile(const char * pName, const char * pDescription, const char * pVersion, const char* pFormat, GMSEC_BIN pData, size_t pFileSize);

	InternalProductFile(const InternalProductFile& other);

	InternalProductFile& operator=(const InternalProductFile& other);

	~InternalProductFile();

	void CALL_TYPE init(const char * pName, const char * pDescription, const char * pVersion, const char* pFormat, const char * pUri, GMSEC_BIN pData, size_t pFileSize);

	const char* CALL_TYPE getName() const;

	const char* CALL_TYPE getDescription() const;

	const char* CALL_TYPE getVersion() const;

	const char* CALL_TYPE getFormat() const;

	bool CALL_TYPE uriAvailable() const;

	const char* CALL_TYPE getURI() const;

	bool CALL_TYPE contentsAvailable() const;

	size_t CALL_TYPE getContents(GMSEC_BIN& contents) const;

	bool CALL_TYPE operator==(const InternalProductFile& productFile) const;

	bool CALL_TYPE operator!=(const InternalProductFile& productFile) const;

private:
	std::string m_name;
	std::string m_description;
	std::string m_version;
	std::string m_format;
	
	GMSEC_BIN   m_data;
	size_t      m_fileSize;

	bool        m_hasUri;
	std::string m_uri;

};

}
}
}
}

#endif
