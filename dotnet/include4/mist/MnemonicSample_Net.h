/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_MNEMONIC_SAMPLE_NET_H
#define GMSEC_API_MNEMONIC_SAMPLE_NET_H


// C++ API native
#include <gmsec4/mist/MnemonicSample.h>


namespace GMSEC
{
namespace API
{

// Forward declarations
ref class Field;

namespace MIST
{


/// <summary>
/// This class is a lightweight container for holding information on a Mnemonic Sample,
/// and used to generate GMSEC Mnemonic messages by the ConnectionManager and Mnemonic classes.
/// </summary>


public ref class MnemonicSample
{
public:
	enum class LimitFlag
	{
		UNSPECIFIED = 0,
		NO_LIMIT_VIOLATION = 1,
		RED_LOW = 2,
		YELLOW_LOW = 3,
		YELLOW_HIGH = 4,
		RED_HIGH = 5
	};


	/// <summary>Creates a Mnemonic Sample instance with a timestamp and a raw value.</summary>
	///
	/// <param name="timestamp">The time at which the sampled value was taken</param>
	/// <param name="rawValue">The Field containing the sampled raw value</param>
	MnemonicSample(System::String^ timestamp, Field^ rawValue);


	/// <summary>Copy-constructor that can be used to create a Mnemonic Sample instance from another instance.</summary>
	MnemonicSample(MnemonicSample^ other);


	/// <summary>Destructor.</summary>
	~MnemonicSample();


	/// <summary>Returns the timestamp associated with the sample.</summary>
	System::String^ GetTimestamp();


	/// <summary>Returns the field containing the raw value which is associated with the sample.</summary>
	Field^ GetRawValue();


	/// <summary>Allows for the setting of the EU value within the sample.</summary>
	///
	/// <param name="euValue">The field containing the EU value</param>
	void SetEUValue(Field^ euValue);


	/// <summary>Returns whether an EU value has been set within the sample.</summary>
	bool HasEUValue();


	/// <summary>Returns the field containing the EU value associated with the sample.</summary>
	///
	/// <exception cref="GMSEC_Exception">An exception is thrown if an EU value has not been associated with the sample</exception>
	///
	/// <seealso cref="MnemonicSample::setEUValue"/>
	/// <seealso cref="MnemonicSample::hasEUValue"/>
	Field^ GetEUValue();


	/// <summary>Allows for the setting of the text value within the sample.</summary>
	///
	/// <param name="textValue">The string containing the text value</param>
	void SetTextValue(System::String^ textValue);


	/// <summary>Returns whether a text value has been set within the sample.</summary>
	bool HasTextValue();


	/// <summary>Returns the text value associated with the sample.</summary>
	///
	/// <exception cref="GMSEC_Exception">An exception is thrown if a text value has not been associated with the sample</exception>
	///
	/// <seealso cref="MnemonicSample::setTextValue"/>
	/// <seealso cref="MnemonicSample::hasTextValue"/>
	System::String^ GetTextValue();


	/// <summary>Allows for the setting of the flags value within the sample.</summary>
	///
	/// <param name="flags">The 32-bit flags value</param>
	void SetFlags(System::Int32 flags);


	/// <summary>Returns whether a flags value has been set within the sample.</summary>
	bool HasFlags();


	/// <summary>Returns the flags value associated with the sample.</summary>
	///
	/// <exception cref="GMSEC_Exception">An exception is thrown if a flags value has not been associated with the sample</exception>
	///
	/// <seealso cref="MnemonicSample::setFlags"/>
	/// <seealso cref="MnemonicSample::hasFlags"/>
	System::Int32 GetFlags();


	/// <summary>Allows for the setting of the Limit enable/disable value within the sample.</summary>
	///
	/// <param name="value">Boolean value to indicate whether the limit is enabled (true) or disabled (false)</param>
	void SetLimitEnableDisable(bool value);


	/// <summary>Returns whether a limit enable/disable value has been set within the sample.</summary>
	bool HasLimitEnableDisable();


	/// <summary>Returns the limit enable/disable boolean value associated with the sample.</summary>
	///
	/// <exception cref="GMSEC_Exception">An exception is thrown if a limit enable/disablue value has not been associated with the sample</exception>
	///
	/// <seealso cref="MnemonicSample::setLimitEnableDisable"/>
	/// <seealso cref="MnemonicSample::hasLimitEnableDisable"/>
	bool GetLimitEnableDisable();


	/// <summary>Allows for the setting of the Limit flag within the sample.</summary>
	///
	/// <param name="limit">Enumerated LimitFlag value</param>
	void SetLimit(LimitFlag limit);


	/// <summary>Returns whether a LimitFlag value has been set within the sample.</summary>
	bool HasLimit();


	/// <summary>Returns the LimitFlag associated with the sample.</summary>
	///
	/// <exception cref="GMSEC_Exception">An exception is thrown if a LimitFlag has not been associated with the sample</exception>
	///
	/// <seealso cref="MnemonicSample::setLimit"/>
	/// <seealso cref="MnemonicSample::hasLimit"/>
	LimitFlag GetLimit();


	/// <summary>Allows for the setting of the Staleness Status boolean value within the sample.</summary>
	///
	/// <param name="staleness">Staleness boolean value</param>
	void SetStalenessStatus(bool staleness);


	/// <summary>Returns whether a Staleness Status value has been set within the sample.</summary>
	bool HasStalenessStatus();


	/// <summary>Returns the Staleness Status boolean value associated with the sample.</summary>
	///
	/// <exception cref="GMSEC_Exception">An exception is thrown if a Staleness Status value has not been associated with the sample</exception>
	///
	/// <seealso cref="MnemonicSample::setStalenessStatus"/>
	/// <seealso cref="MnemonicSample::hasStalenessStatus"/>
	bool GetStalenessStatus();


	/// <summary>Allows for the setting of the Quality boolean value within the sample.</summary>
	///
	/// <param name="quality">Quality boolean value</param>
	void SetQuality(bool quality);


	/// <summary>Returns whether a Quality value has been set within the sample.</summary>
	bool HasQuality();


	/// <summary>Returns the Quality boolean value associated with the sample.</summary>
	///
	/// <exception cref="GMSEC_Exception">An exception is thrown if a Quality value has not been associated with the sample</exception>
	///
	/// <seealso cref="MnemonicSample::setQuality"/>
	/// <seealso cref="MnemonicSample::hasQuality"/>
	bool GetQuality();


protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!MnemonicSample();


internal:
	/// <summary>This constructor creates the association of this object with an existing unmanaged object</summary>
	///
	/// <param name="sample">unmanaged implementation to initialize with</param>
	MnemonicSample(gmsec::api::mist::MnemonicSample* sample, bool owned);


	/// <summary>Get unmanaged implementation version</summary>
	///
	/// <returns>Unmanaged implementation</returns>
	gmsec::api::mist::MnemonicSample* GetUnmanagedImplementation();


	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanagedImplementation();


	/// <summary>Delete old impl if present, and set unmanaged implementation to new value</summary>
	void SetUnmanagedImplementation(gmsec::api::mist::MnemonicSample* sample, bool owned);


private:
	gmsec::api::mist::MnemonicSample* m_impl;
	bool                              m_owned;
};

} // end namespace MIST
} // end namespace API
} // end namespace GMSEC

#endif
