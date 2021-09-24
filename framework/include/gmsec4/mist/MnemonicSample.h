/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file MnemonicSample.h
 *
 *  @brief This file contains declarations for Mnemonic parameters
**/

#ifndef GMSEC_API_MNEMONICSAMPLE_H
#define GMSEC_API_MNEMONICSAMPLE_H


#include <gmsec4/field/Field.h>
#include <gmsec4/util/wdllexp.h>

#include <gmsec4_defs.h>


namespace gmsec
{
namespace api
{
namespace mist
{

namespace internal
{
	class InternalMnemonicSample;
}

/**
 * @class MnemonicSample
 *
 * @brief This class is a lightweight container for holding information
 * on a Mnemonic Sample, and used to generate GMSEC Mnemonic messages by the 
 * ConnectionManager and Device classes
 *
 * @sa Mnemonic @n
 *		ConnectionManager
*/
class GMSEC_API MnemonicSample
{
  public:

	/**
     * @enum LimitFlag
     * @brief Possible values for the a MnemonicSample's limit.
     */
	enum LimitFlag
	{
		UNSPECIFIED = 0,		///< No Limit specified         
		NO_LIMIT_VIOLATION = 1,	///< No violation           
		RED_LOW = 2,			///< Lower red-line violation
		YELLOW_LOW = 3,			///< Lower yellow-line violation 
		YELLOW_HIGH = 4,		///< Upper yellow-line violation 
		RED_HIGH = 5			///< Upper red-line violation
	};

	/**
	 * @fn MnemonicSample(const char* timestamp, const Field& rawValue)
	 *
	 * @brief Constructor - Initializes the MnemonicSample object with 
	 * a timestamp and value
	 *
	 * @param timestamp - The time at which the sampled value was taken
	 * @param rawValue - The raw value of the parameter
	 */
	MnemonicSample(const char* timestamp, const Field& rawValue);

	/**
	 * @fn MnemonicSample(const MnemonicSample& other)
	 *
	 * @brief Constructor - Initializes the MnemonicSample object from another
	 * MnemonicSample
	 *
	 * @param other - The other MnemonicSample to inherit values from
	 */
	MnemonicSample(const MnemonicSample& other);

	/**
	 * @fn ~MnemonicSample()
	 *
	 * @brief Destructor - Clean up internal member
	 */
	~MnemonicSample();

	/**
	 * @fn const char* getTimestamp() const
	 *
	 * @brief Retrieves the timestamp of the MnemonicSample
	 *
	 * @return The timestamp of the MnemonicSample
	 */
	const char* CALL_TYPE getTimestamp() const;

	/**
	 * @fn const Field& getRawValue() const
	 *
	 * @brief Retrieves the value of the MnemonicSample
	 *
	 * @return The value of the MnemonicSample
	 */
	const Field& CALL_TYPE getRawValue() const;

	/**
	 * @fn bool hasEUValue() const
	 *
	 * @brief Returns the availability of a MnemonicSample value in
	 * engineering units
	 *
	 * @return True if MnemonicSample EU value has been set, false otherwise
	 */
	bool CALL_TYPE hasEUValue() const;

	/**
	 * @fn const Field& getEUValue() const
	 *
	 * @brief Accessor for the EU Value
	 *
	 * @return A reference to the MnemonicSample EU value field
	 *
	 * @throw An exception is thrown if the EU value field has not been set
	 */
	const Field& CALL_TYPE getEUValue() const;

	/**
	 * @fn void setEUValue(const Field& value)
	 *
	 * @brief Supplies a field for the MnemonicSample EU value and makes a copy of the field
	 *
	 */
	void CALL_TYPE setEUValue(const Field& value);

	/**
	 * @fn bool hasTextValue() const
	 *
	 * @brief Returns the availability of a MnemonicSample text value
	 *
	 * @return True if MnemonicSample text value has been set, false otherwise
	 */
	bool CALL_TYPE hasTextValue() const;

	/**
	 * @fn const char* getTextValue() const
	 *
	 * @brief Accessor for the text value
	 *
	 * @return A pointer to the MnemonicSample text value field
	 *
	 * @throw An exception is thrown if the text value field has not been set
	 */
	const char* CALL_TYPE getTextValue() const;

	/**
	 * @fn void setTextValue(const char* value)
	 *
	 * @brief Supplies a string for the MnemonicSample text value and makes a copy of the string
	 *
	 */
	void CALL_TYPE setTextValue(const char* value);

	/**
	 * @fn bool hasFlags() const
	 *
	 * @brief Returns the availability of a MnemonicSample flags field
	 *
	 * @return True if MnemonicSample flags value has been set, false otherwise
	 */
	bool CALL_TYPE hasFlags() const;

	/**
	 * @fn GMSEC_I32 getFlags() const
	 *
	 * @brief Accessor for the flags
	 *
	 * @return A reference to the MnemonicSample flags field
	 *
	 * @throw An exception is thrown if the flags field has not been set
	 */
	GMSEC_I32 CALL_TYPE getFlags() const;

	/**
	 * @fn void setFlags(GMSEC_I32 flags)
	 *
	 * @brief Supplies a GMSEC_I32 for the MnemonicSample flags value
	 *
	 */
	void CALL_TYPE setFlags(GMSEC_I32 flags);

	/**
	 * @fn bool hasLimitEnableDisable() const
	 *
	 * @brief Returns the availability of a MnemonicSample limit enable/disable flag
	 *
	 * @return True if MnemonicSample limit enable/disable flag has been set,
	 * false otherwise
	 */
	bool CALL_TYPE hasLimitEnableDisable() const;

	/**
	 * @fn bool getLimitEnableDisable() const
	 *
	 * @brief Accessor for the limit enable/disable flag 
	 *
	 * @return The MnemonicSample limit enable/disable flag
	 *
	 * @throw An exception is thrown if the limit enable/disable flag has not been set
	 */
	bool CALL_TYPE getLimitEnableDisable() const;

	/**
	 * @fn void setLimitEnableDisable(bool value)
	 *
	 * @brief Supplies a boolean flag for the MnemonicSample setLimitEnableDisable(bool value)
	 *
	 */
	void CALL_TYPE setLimitEnableDisable(bool value);

	/**
	 * @fn bool hasLimit() const
	 *
	 * @brief Returns the availability of a MnemonicSample limit
	 *
	 * @return True if MnemonicSample limit has been set, false otherwise
	 */
	bool CALL_TYPE hasLimit() const;

	/**
	 * @fn LimitFlag getLimit() const
	 *
	 * @brief Accessor for the limit
	 *
	 * @return The MnemonicSample limit
	 *
	 * @throw An exception is thrown if the limit has not been set
	 */
	LimitFlag CALL_TYPE getLimit() const;

	/**
	 * @fn void setLimit(LimitFlag)
	 *
	 * @brief Supplies a limit flag for the MnemonicSample limit
	 *
	 */
	void CALL_TYPE setLimit(LimitFlag);

	/**
	 * @fn bool hasStalenessStatus() const
	 *
	 * @brief Returns the availability of a MnemonicSample staleness status
	 *
	 * @return True if MnemonicSample staleness has been set, false otherwise
	 */
	bool CALL_TYPE hasStalenessStatus() const;

	/**
	 * @fn bool getStalenessStatus() const
	 *
	 * @brief Accessor for the staleness
	 *
	 * @return The MnemonicSample staleness
	 *
	 * @throw An exception is thrown if the staleness flag has not been set
	 */
	bool CALL_TYPE getStalenessStatus() const;

	/**
	 * @fn void setStalenessStatus(bool staleness)
	 *
	 * @brief Supplies a staleness flag for the MnemonicSample
	 *
	 */
	void CALL_TYPE setStalenessStatus(bool staleness);

	/**
	 * @fn bool hasQuality() const
	 *
	 * @brief Returns the availability of a MnemonicSample quality flag
	 *
	 * @return True if MnemonicSample quality has been set, false otherwise
	 */
	bool CALL_TYPE hasQuality() const;

	/**
	 * @fn bool getQuality() const
	 *
	 * @brief Accessor for the quality
	 *
	 * @return The MnemonicSample quality
	 *
	 * @throw An exception is thrown if the quality flag has not been set
	 */
	bool CALL_TYPE getQuality() const; 

	/**
	 * @fn void setQuality(bool quality)
	 *
	 * @brief Supplies a quality flag for the MnemonicSample
	 *
	 */
	void CALL_TYPE setQuality(bool quality);

	/**
	 * @fn bool operator==(const MnemonicSample& mnemonicSample) const
	 *
	 * @brief Equality operator for MnemonicSample
	 *
	 */
	bool CALL_TYPE operator==(const MnemonicSample& mnemonicSample) const;

	/**
	 * @fn bool operator!=(const MnemonicSample& mnemonicSample) const
	 *
	 * @brief Non-equality operator for MnemonicSample
	 *
	 */
	bool CALL_TYPE operator!=(const MnemonicSample& mnemonicSample) const;

  private:

	MnemonicSample(); // not implemented

	gmsec::api::mist::internal::InternalMnemonicSample* m_iMnemonicSample;

};

}
}
}

#endif
