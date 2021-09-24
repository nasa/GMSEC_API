/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file ProductFile.h
 *
 *  @brief This file contains declarations for ProductFile containers
**/

#ifndef GMSEC_API_PRODUCTFILE_H
#define GMSEC_API_PRODUCTFILE_H

#include <gmsec4/util/wdllexp.h>

#include <gmsec4_defs.h>

#include <cstdlib>

#if defined(__hpux) || defined(__sun)
#include <string.h>
#endif

namespace gmsec
{
namespace api
{
namespace mist
{

namespace internal
{
	class InternalProductFile;
}

/**
 * @class ProductFile
 *
 * @brief This class is a lightweight container for holding information
 * on a Product File, and used to generate GMSEC Product File messages by the 
 * ConnectionManager class
 *
 * @sa ConnectionManager @n
 *		
*/
class GMSEC_API ProductFile
{
public:

	/**
	 * @fn ProductFile(const char* pName, const char* pDescription, const char* pVersion, 
	 *	const char* pFormat, const char* pUri)
	 *
	 * @brief Constructor - Initializes the ProductFile object with 
	 * a URI to describe a commonly accessible resource
	 *
	 * @param pName - The name of the file
	 * @param pDescription - The description of the file
	 * @param pVersion - The version of the file
	 * @param pFormat - The descriptor for the file format
	 * @param pUri - The URI for the file
	 *
	 * @throw An exception is thrown if pName, pDescription, pVersion,
	 *        or pFormat are NULL or empty strings.
	 */
	ProductFile(const char* pName, const char* pDescription, const char* pVersion, 
		const char* pFormat, const char* pUri);

	/**
	 * @fn ProductFile(const char* pName, const char* pDescription, const char* pVersion, 
	 *	const char* pFormat, GMSEC_BIN pData, size_t pFileSize)
	 *
	 * @brief Constructor - Initializes the ProductFile object with 
	 * the binary contents of the file
	 *
	 * @param pName - The name of the file
	 * @param pDescription - The description of the file
	 * @param pVersion - The version of the file
	 * @param pFormat - The descriptor for the file format
	 * @param pData - The binary contents of the file
	 * @param pFileSize - The size of the binary contents of the file
	 *
	 * @throw An exception is thrown if pName, pDescription, pVersion,
	 *        or pFormat are NULL or empty strings.
	 */
	ProductFile(const char* pName, const char* pDescription, const char* pVersion, 
		const char* pFormat, GMSEC_BIN pData, size_t pFileSize);

	/**
	 * @fn ProductFile(const ProductFile& other)
	 *
	 * @brief Constructor - Initializes the ProductFile object with 
	 * the information from another ProjectFile
	 *
	 * @param other - The other ProductFile
	 */
	ProductFile(const ProductFile& other);

	/**
	 * @fn ProductFile& operator=(const ProductFile& other)
	 *
	 * @brief Assignment operator - Initializes the Device object with the information
	 * from another product file
	 *
	 * @param other - The other product file
	 */
	ProductFile& operator=(const ProductFile& other);

	/**
	 * @fn ~ProductFile()
	 *
	 * @brief Destructor
	 *
	 */
	~ProductFile();

	/**
	 * @fn const char* getName() const
	 *
	 * @brief Retrieves the name of the ProductFile
	 *
	 * @return The name of the ProductFile
	 */
	const char* CALL_TYPE getName() const;

	/**
	 * @fn const char* getDescription() const
	 *
	 * @brief Retrieves the description of the ProductFile
	 *
	 * @return The description of the ProductFile
	 */
	const char* CALL_TYPE getDescription() const;

	/**
	 * @fn const char* getVersion() const
	 *
	 * @brief Retrieves the version of the ProductFile
	 *
	 * @return The version of the ProductFile
	 */
	const char* CALL_TYPE getVersion() const;

	/**
	 * @fn const char* getFormat() const
	 *
	 * @brief Retrieves the format of the ProductFile
	 *
	 * @return The format of the ProductFile
	 */
	const char* CALL_TYPE getFormat() const;

	/**
	 * @fn bool uriAvailable() const
	 *
	 * @brief Returns the availability of a ProductFile URI
	 *
	 * @return True if a ProductFile URI has been set, false otherwise
	 */
	bool CALL_TYPE uriAvailable() const;

	/**
	 * @fn const char* getURI() const
	 *
	 * @brief Accessor for the ProductFile status
	 *
	 * @return A pointer to the ProductFile URI string, or NULL if
	 *         he URI is not available.
	 *
	 */
	const char* CALL_TYPE getURI() const;

	/**
	 * @fn bool contentsAvailable() const
	 *
	 * @brief Returns the availability of a ProductFile file content
	 *
	 * @return True if a ProductFile file content has been set, false otherwise
	 */
	bool CALL_TYPE contentsAvailable() const;

	/**
	 * @fn size_t getContents(GMSEC_BIN& contents) const
	 *
	 * @brief Accessor for the ProductFile file content
	 *
	 * @return A reference to the ProductFile file content
	 *
	 * @throw An exception is thrown if the file content has not been set
	 */
	size_t CALL_TYPE getContents(GMSEC_BIN& contents) const;

	/**
	 * @fn bool operator==(const ProductFile& productFile) const
	 *
	 * @brief Equality operator for ProductFile
	 *
	 */
	bool CALL_TYPE operator==(const ProductFile& productFile) const;

	/**
	 * @fn bool operator!=(const ProductFile& productFile) const
	 *
	 * @brief Non-equality operator for ProductFile
	 *
	 */
	bool CALL_TYPE operator!=(const ProductFile& productFile) const;

private:

	gmsec::api::mist::internal::InternalProductFile* m_iProductFile;

};

}
}
}

#endif
