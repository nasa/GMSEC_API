/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file Mnemonic.h
 *
 *  @brief This file contains declarations for Mnemonic containers
**/


#ifndef GMSEC_API_MNEMONIC_H
#define GMSEC_API_MNEMONIC_H

#include <gmsec4/mist/MnemonicSample.h>
#include <gmsec4/util/DataList.h>
#include <gmsec4_defs.h>

namespace gmsec
{
namespace api
{
namespace mist
{

namespace internal
{
class InternalMnemonic;
}

/**
 * @class Mnemonic
 *
 * @brief This class is a lightweight container for holding information
 * on a Mnemonic, and used to generate GMSEC Mnemonic messages by the 
 * ConnectionManager class
 *
 * @sa ConnectionManager @n
 *		
*/
class GMSEC_API Mnemonic
{
  public:

	/**
	 * @fn Mnemonic(const char* name, const gmsec::api::util::DataList<MnemonicSamples*>& samples)
	 *
	 * @brief Constructor - Initializes the Mnemonic object with a set of
	 * MnemonicSample information
	 *
	 * @param name - The name of the Mnemonic
	 * @param samples - The MnemonicSamples to add to the Mnemonic object.
	 *
	 * @throw An Exception is thrown if a NULL or empty name parameter is provided.
	 */
	Mnemonic(const char* name, const gmsec::api::util::DataList<MnemonicSample*>& samples);

        /**
         * @fn Mnemonic(const char* name)
         *
         * @brief Constructor - Initializes the Mnemonic object with only the name.
         *
         * @param name - the name of the Mnemonic
         *
         * @throw An Exception is thrown if a NULL or empty name parameter is provided.
         */
        Mnemonic(const char* name);

	/**
	 * @fn Mnemonic(const Mnemonic& mnemonic)
	 *
	 * @brief Constructor - Initializes the Mnemonic object with the information
	 * from another device
	 *
	 * @param mnemonic - The other Mnemonic
	 */
	Mnemonic(const Mnemonic& mnemonic);

	/**
	 * @fn Mnemonic& operator=(const Mnemonic& other)
	 *
	 * @brief Assignment operator - Initializes the Mnemonic object with the information
	 * from another mnemonic object
	 *
	 * @param other - The other mnemonic
	 */
	Mnemonic& operator=(const Mnemonic& other);


	/**
	 * @fn ~Mnemonic()
	 *
	 * @brief Destructor
	 *
	 */
	~Mnemonic();


	/**
	 * @fn const char* getName() const
	 *
	 * @brief Retrieves the name of the Mnemonic
	 *
	 * @return The name of the Mnemonic
	 */
	const char* CALL_TYPE getName() const;


	/**
	 * @fn bool statusAvailable() const
	 *
	 * @brief Returns the availability of a Mnemonic status
	 *
	 * @return True if a Mnemonic status has been set, false otherwise
	 */
	bool CALL_TYPE statusAvailable() const;


	/**
	 * @fn const Field& getStatus() const
	 *
	 * @brief Accessor for the Mnemonic status
	 *
	 * @return A reference to the Mnemonic status field
	 *
	 * @throw An exception is thrown if the status field has not been set
	 */
	const Field& CALL_TYPE getStatus() const;


	/**
	 * @fn void setStatus(const Field& status)
	 *
	 * @brief Supplies a Status for the Mnemonic and makes a copy of the field
	 *
	 */
	void CALL_TYPE setStatus(const Field& status);


	/**
	 * @fn bool unitsAvailable() const
	 *
	 * @brief Returns the availability of Mnemonic units
	 *
	 * @return True if Mnemonic units have been set, false otherwise
	 */
	bool CALL_TYPE unitsAvailable() const;


	/**
	 * @fn const char* getUnits() const
	 *
	 * @brief Accessor for the Mnemonic units
	 *
	 * @return A pointer to the Mnemonic units string
	 *
	 * @throw An exception is thrown if the units string has not been set
	 */
	const char* CALL_TYPE getUnits() const;


	/**
	 * @fn void setUnits(const char* units)
	 *
	 * @brief Supplies a units string for the Mnemonic and makes a copy of the string
	 *
	 */
	void CALL_TYPE setUnits(const char* units);


	/**
	 * @fn size_t getSampleCount() const
	 *
	 * @brief Returns the number of samples for this Mnemonic
	 *
	 * @return a size_t representation of the number of samples
	 */
	size_t CALL_TYPE getSampleCount() const;


	/**
	 * @fn const MnemonicSample& getSample(size_t idx) const
	 *
	 * @brief Returns a reference to a MnemonicSample held in this object
	 *
	 * @param idx - the zero-based index of the MnemonicSample object to access
	 *
	 * @return A reference to the requested MnemonicSample object
	 *
	 * @throw An exception if the index supplied is out of bounds
	 */
	const MnemonicSample& CALL_TYPE getSample(size_t idx) const;


	/**
	 * @fn void addSample(const MnemonicSample& sample)
	 *
	 * @brief Adds a single MnemonicSample to the current list
	 *
	 * @param param - the new MnemonicSample object
	 *
	 */
	void CALL_TYPE addSample(const MnemonicSample& sample);


private:
	gmsec::api::mist::internal::InternalMnemonic* m_iMnemonic;
};

}
}
}

#endif
