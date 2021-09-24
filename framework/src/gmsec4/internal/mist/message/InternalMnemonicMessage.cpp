/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file InternalMnemonicMessage.cpp
 *
 * @brief This file contains the internal implementation of Mnemonic messages.
 */

// Disable the Visual Studio warning C4355
//   ('this' : used in base member initializer list)
//   Found in the Connection(Server s, const Options &o) constructor
#ifdef WIN32
#pragma warning(disable: 4355)
#endif


#include <gmsec4/internal/mist/message/InternalMnemonicMessage.h>

#include <gmsec4/internal/mist/InternalMnemonicIterator.h>
#include <gmsec4/internal/mist/MessageSubclassHelper.h>

#include <gmsec4/mist/ConnectionManager.h>
#include <gmsec4/mist/Mnemonic.h>

#include <gmsec4/internal/StringUtil.h>

#include <gmsec4/util/Log.h>
#include <gmsec4/util/DataList.h>

#include <gmsec4/Exception.h>

#include <sstream>


static const char* const MESSAGE_TYPE_STRING      = "MESSAGE-TYPE";
static const char* const MESSAGE_SUBTYPE_STRING   = "MESSAGE-SUBTYPE";
static const char* const MSG_ID_STRING            = "MSG-ID";
static const char* const MSG_STRING               = "MSG";
static const char* const MVAL_STRING              = "MVAL";
static const char* const NUM_OF_MNEMONICS_STRING  = "NUM-OF-MNEMONICS";


using namespace gmsec::api::mist::internal;
using namespace gmsec::api::util;

namespace gmsec
{
namespace api
{
namespace mist
{
namespace message
{
namespace internal
{


InternalMnemonicMessage::InternalMnemonicMessage(const char* subject, 
												 const char* schemaID,
												 const Specification& spec)
	: InternalMistMessage(subject, InternalMistMessage::findKind(schemaID, spec.getVersion()), schemaID, spec),
	  m_list(),
	  m_mnemonicIterator(*this)
{
	init();
}


InternalMnemonicMessage::InternalMnemonicMessage(const char* subject, 
												 const char* schemaID,
												 const gmsec::api::Config& config, 
												 const Specification& spec)
	: InternalMistMessage(subject, InternalMistMessage::findKind(schemaID, spec.getVersion()), schemaID, config, spec),
	  m_list(),
	  m_mnemonicIterator(*this)
{
	init();
}


InternalMnemonicMessage::InternalMnemonicMessage(const InternalMnemonicMessage& other)
	: InternalMistMessage(other),
	  m_list(other.m_list),
	  m_mnemonicIterator(*this)
{
	init();
}


InternalMnemonicMessage::InternalMnemonicMessage(const char* data)
	: InternalMistMessage(data),
	  m_list(),
	  m_mnemonicIterator(*this)
{
	MessageSubclassHelper::checkStringField(MESSAGE_TYPE_STRING, "InternalMnemonicMessage", MSG_STRING, *this);
	MessageSubclassHelper::checkStringField(MESSAGE_SUBTYPE_STRING, "InternalMnemonicMessage", MVAL_STRING, *this);

	GMSEC_U32 numOfMnemonics = (GMSEC_U32) this->getUnsignedIntegerValue(NUM_OF_MNEMONICS_STRING);

	for (GMSEC_U32 count = 0; count < numOfMnemonics; ++count)
	{
		m_list.push_back(extractMessageMnemonic(count+1));
	}
}


InternalMnemonicMessage::InternalMnemonicMessage(const Specification& spec, const char* data)
	: InternalMistMessage(spec, data),
	  m_list(),
	  m_mnemonicIterator(*this)
{
	MessageSubclassHelper::checkStringField(MESSAGE_TYPE_STRING, "InternalMnemonicMessage", MSG_STRING, *this);
	MessageSubclassHelper::checkStringField(MESSAGE_SUBTYPE_STRING, "InternalMnemonicMessage", MVAL_STRING, *this);

	GMSEC_U32 numOfMnemonics = (GMSEC_U32) this->getUnsignedIntegerValue(NUM_OF_MNEMONICS_STRING);

	for (GMSEC_U32 count = 0; count < numOfMnemonics; ++count)
	{
		m_list.push_back(extractMessageMnemonic(count+1));
	}
}


InternalMnemonicMessage::~InternalMnemonicMessage()
{
}


void InternalMnemonicMessage::addMnemonic(const Mnemonic& mnemonic)
{
	m_list.push_back(mnemonic);

	setValue(NUM_OF_MNEMONICS_STRING, (GMSEC_I64) m_list.size());

	std::ostringstream s;
	s << "MNEMONIC." << m_list.size();
	std::string strCount = s.str();

	std::string nameStr = strCount;
	nameStr.append(".NAME");
	setValue(nameStr.c_str(), mnemonic.getName());

	if (mnemonic.statusAvailable())
	{
		std::string statusStrName = strCount;
		statusStrName.append(".STATUS");

		GMSEC_I16 i16_value = 0;

		try
		{
			i16_value = dynamic_cast<const I16Field&>(mnemonic.getStatus()).getValue();
		}
		catch (...)
		{
			throw Exception(MIST_ERROR,
			                INCORRECT_FIELD_TYPE,
			                "InternalMnemonicMessage::addMnemonic():  Expected I16Field and got something different in a dynamic_cast, for .STATUS.");
		}

		setValue(statusStrName.c_str(), (GMSEC_I64) i16_value);
	}

	if (mnemonic.unitsAvailable())
	{
		std::string unitsName = strCount;
		unitsName.append(".UNITS");
		setValue(unitsName.c_str(), mnemonic.getUnits());
	}

	std::string numSamplesName = strCount;
	numSamplesName.append(".NUM-OF-SAMPLES");

	setValue(numSamplesName.c_str(), (GMSEC_I64) mnemonic.getSampleCount());

	for (size_t s_idx = 0; s_idx < mnemonic.getSampleCount(); s_idx++)
	{
		std::ostringstream ss;
		ss << strCount << ".SAMPLE." << (s_idx + 1);
		std::string sampleCountStr = ss.str();

		MnemonicSample *current = const_cast<MnemonicSample*>(&(mnemonic.getSample(s_idx)));
		
		std::string tsCount = sampleCountStr;
		tsCount.append(".TIME-STAMP");
		setValue(tsCount.c_str(), current->getTimestamp());

		std::string rawValCount = sampleCountStr;
		rawValCount.append(".RAW-VALUE");

		GMSEC_I32 i32_value = 0;

		try
		{
			i32_value = dynamic_cast<const I32Field&>(current->getRawValue()).getValue();
		}
		catch (...)
		{
			throw Exception(MIST_ERROR,
			                INCORRECT_FIELD_TYPE,
			                "InternalMnemonicMessage::addMnemonic():  Expected I32Field and got something different in a dynamic_cast, for .RAW-VALUE.");
		}
		setValue(rawValCount.c_str(), (GMSEC_I64) i32_value);

		if (current->hasEUValue())
		{
			std::string euValCount = sampleCountStr;
			euValCount.append(".EU-VALUE");

			try
			{
				setValue(euValCount.c_str(), current->getEUValue().getDoubleValue());
			}
			catch (...)
			{
				throw Exception(MIST_ERROR,
				                INCORRECT_FIELD_TYPE,
				                "InternalMnemonicMessage::addMnemonic():  Unable to convert EU-VALUE to double value.");
			}
		}

		if (current->hasTextValue())
		{
			std::string textValCount = sampleCountStr;
			textValCount.append(".TEXT-VALUE");
			setValue(textValCount.c_str(), current->getTextValue());
		}

		if (current->hasFlags())
		{
			std::string flagsCount = sampleCountStr;
			flagsCount.append(".FLAGS");
			addField(flagsCount.c_str(), current->getFlags());
		}

		if (current->hasLimitEnableDisable())
		{
			std::string limitEnableDisableCount = sampleCountStr;
			limitEnableDisableCount.append(".LIMIT-ENABLE-DISABLE");
			addField(limitEnableDisableCount.c_str(), current->getLimitEnableDisable());
		}

		std::string limitCount = sampleCountStr;
		if (current->getLimit() == MnemonicSample::RED_HIGH)
		{
			limitCount.append(".RED-HIGH");
			addField(limitCount.c_str(), true);
		}
		else if(current->getLimit() == MnemonicSample::YELLOW_HIGH)
		{
			limitCount.append(".YELLOW-HIGH");
			addField(limitCount.c_str(), true);
		}
		else if(current->getLimit() == MnemonicSample::YELLOW_LOW)
		{
			limitCount.append(".YELLOW-LOW");
			addField(limitCount.c_str(), true);
		}
		else if(current->getLimit() == MnemonicSample::RED_LOW)
		{
			limitCount.append(".RED-LOW");
			addField(limitCount.c_str(), true);
		}

		if (current->hasStalenessStatus())
		{
			std::string statusCount = sampleCountStr;
			statusCount.append(".STATIC");
			addField(statusCount.c_str(), current->getStalenessStatus());
		}

		if (current->hasQuality())
		{
			std::string qualityCount = sampleCountStr;
			qualityCount.append(".QUALITY");
			addField(qualityCount.c_str(), current->getQuality());
		}
	}
}


const Mnemonic& InternalMnemonicMessage::getMnemonic(size_t index) const
{
	if (index < m_list.size())
	{
		return m_list[index];
	}

	std::ostringstream oss;

	oss << "InternalMnemonicMessage::getMnemonic():  index "
	    << index << "specified is out-of-range";

	throw Exception(MIST_ERROR, INDEX_OUT_OF_RANGE, oss.str().c_str());
}


size_t InternalMnemonicMessage::getNumMnemonics() const
{
	return m_list.size();
}


MnemonicIterator& InternalMnemonicMessage::getMnemonicIterator()
{
	m_mnemonicIterator.m_iter->reset();
	return m_mnemonicIterator;
}


void InternalMnemonicMessage::resetMnemonicIterator()
{
	m_listIter = m_list.begin();
}


bool InternalMnemonicMessage::hasNextMnemonic() const
{
	return m_listIter != m_list.end();
}


const Mnemonic& InternalMnemonicMessage::nextMnemonic()
{
	if (!hasNextMnemonic())
	{
		throw Exception(MIST_ERROR, ITER_INVALID_NEXT, "No more Mnemonics available");
	}

	const Mnemonic& mnemonic= *m_listIter;

	++m_listIter;

	return mnemonic;
}


Mnemonic InternalMnemonicMessage::extractMessageMnemonic(size_t index) const
{
	char tmp_name[256];

	GMSEC_I16   mnemonic_status;
	std::string mnemonic_units;

	StringUtil::stringFormat(tmp_name, sizeof(tmp_name), "MNEMONIC.%u.NAME", index);
	std::string mnemonic_name = MessageSubclassHelper::extractStringField(tmp_name, "InternalMnemonicMessage::extractMessageMnemonic()", *this);

	StringUtil::stringFormat(tmp_name, sizeof(tmp_name), "MNEMONIC.%u.STATUS", index);

	bool mnemonic_status_set = MessageSubclassHelper::getOptionalI16(tmp_name, *this, mnemonic_status);
	

	StringUtil::stringFormat(tmp_name, sizeof(tmp_name), "MNEMONIC.%u.UNITS", index);
	bool mnemonic_units_set = MessageSubclassHelper::getOptionalString(tmp_name, *this, mnemonic_units);

	StringUtil::stringFormat(tmp_name, sizeof(tmp_name), "MNEMONIC.%u.NUM-OF-SAMPLES", index);
	size_t mnemonic_num_of_samples = (size_t) this->getUnsignedIntegerValue(tmp_name);

	DataList<MnemonicSample*> data_list;

	Mnemonic mnemonic(mnemonic_name.c_str(), data_list);

	size_t sample_idx = 1;
	
	while (sample_idx <= mnemonic_num_of_samples)
	{
		const Field*              sample_raw_value_ptr = NULL;
		const Field*              sample_eu_value_ptr  = NULL;
		bool                      eu_value_set     = false;
		GMSEC_I32                 sample_flags     = 0;
		bool                      sample_limit_enable_disable = false;
		MnemonicSample::LimitFlag sample_limit     = MnemonicSample::UNSPECIFIED;
		bool                      sample_static    = false;
		bool                      sample_quality   = false;

		std::string               sample_timestamp;
		std::string               sample_text_value;

		StringUtil::stringFormat(tmp_name, sizeof(tmp_name), "MNEMONIC.%u.SAMPLE.%u.TIME-STAMP", index, sample_idx);

		sample_timestamp = MessageSubclassHelper::extractStringField(tmp_name, "InternalMnemonicMessage::extractMessageMnemonic()", *this);

		StringUtil::stringFormat(tmp_name, sizeof(tmp_name), "MNEMONIC.%u.SAMPLE.%u.RAW-VALUE", index, sample_idx);

		sample_raw_value_ptr = MessageSubclassHelper::extractField(tmp_name, "InternalMnemonicMessage::extractMessageMnemonic()", *this);
		
		StringUtil::stringFormat(tmp_name, sizeof(tmp_name), "MNEMONIC.%u.SAMPLE.%u.EU-VALUE", index, sample_idx);

		sample_eu_value_ptr = getField(tmp_name);
		if (sample_eu_value_ptr)
		{
			eu_value_set = true;
		}
		// Else no problem, engineering units field may be missing

		StringUtil::stringFormat(tmp_name, sizeof(tmp_name), "MNEMONIC.%u.SAMPLE.%u.TEXT-VALUE", index, sample_idx);
		bool text_value_set = MessageSubclassHelper::getOptionalString(tmp_name, *this, sample_text_value);
		
		StringUtil::stringFormat(tmp_name, sizeof(tmp_name), "MNEMONIC.%u.SAMPLE.%u.FLAGS", index, sample_idx);

		bool flags_set = MessageSubclassHelper::getOptionalI32(tmp_name, *this, sample_flags);

		StringUtil::stringFormat(tmp_name, sizeof(tmp_name), "MNEMONIC.%u.SAMPLE.%u.LIMIT-ENABLE-DISABLE", index, sample_idx);

		bool limit_enable_disable_set = MessageSubclassHelper::getOptionalBool(tmp_name, *this, sample_limit_enable_disable);

		if (limit_enable_disable_set && sample_limit_enable_disable)
		{
			bool red_low_flag = false;
			bool yellow_low_flag = false;
			bool yellow_high_flag = false;
			bool red_high_flag = false;

			try
			{
				StringUtil::stringFormat(tmp_name, sizeof(tmp_name), "MNEMONIC.%u.SAMPLE.%u.RED-HIGH", index, sample_idx);

				const BooleanField red_high_field = getBooleanField(tmp_name);
				red_high_flag = red_high_field.getValue();
			}
			catch (...)
			{
				// No problem, limit field may be missing
			}

			try
			{
				StringUtil::stringFormat(tmp_name, sizeof(tmp_name), "MNEMONIC.%u.SAMPLE.%u.YELLOW-HIGH", index, sample_idx);

				const BooleanField yellow_high_field = getBooleanField(tmp_name);
				yellow_high_flag = yellow_high_field.getValue();
			}
			catch (...)
			{
				// No problem, limit field may be missing
			}
		
			try
			{
				StringUtil::stringFormat(tmp_name, sizeof(tmp_name), "MNEMONIC.%u.SAMPLE.%u.YELLOW-LOW", index, sample_idx);

				const BooleanField yellow_low_field = getBooleanField(tmp_name);
				yellow_low_flag = yellow_low_field.getValue();
			}
			catch (...)
			{
				// No problem, limit field may be missing
			}

			try
			{
				StringUtil::stringFormat(tmp_name, sizeof(tmp_name), "MNEMONIC.%u.SAMPLE.%u.RED-LOW", index, sample_idx);

				const BooleanField red_low_field = getBooleanField(tmp_name);
				red_low_flag = red_low_field.getValue();
			}
			catch (...)
			{
				// No problem, limit field may be missing
			}
	
			//MEH - ISD 2014 does not specific exclusivity for limits, therefore two or more limit flags set will
			//not trigger an error exception. However, as this condition is not logical, this code will prioritize and
			//select a flag for the resulting object.

			if (red_low_flag)
			{
				sample_limit = MnemonicSample::RED_LOW;
			}
			else if (yellow_low_flag)
			{
				sample_limit = MnemonicSample::YELLOW_LOW;
			}
			else if (yellow_high_flag)
			{
				sample_limit = MnemonicSample::YELLOW_HIGH;
			}
			else if (red_high_flag)
			{
				sample_limit = MnemonicSample::RED_HIGH;
			}
			else
			{
				sample_limit = MnemonicSample::NO_LIMIT_VIOLATION;
			}
		}
		else
		{
			sample_limit = MnemonicSample::UNSPECIFIED;
		}


		StringUtil::stringFormat(tmp_name, sizeof(tmp_name), "MNEMONIC.%u.SAMPLE.%u.STATIC", index, sample_idx);

		bool static_set = MessageSubclassHelper::getOptionalBool(tmp_name, *this, sample_static);
		

		StringUtil::stringFormat(tmp_name, sizeof(tmp_name), "MNEMONIC.%u.SAMPLE.%u.QUALITY", index, sample_idx);

		bool quality_set = MessageSubclassHelper::getOptionalBool(tmp_name, *this, sample_quality);

		MnemonicSample mnemonic_sample(sample_timestamp.c_str(), *sample_raw_value_ptr);

		if (eu_value_set)
		{
			mnemonic_sample.setEUValue(*sample_eu_value_ptr);
		}

		if (text_value_set)
		{
			mnemonic_sample.setTextValue(sample_text_value.c_str());
		}

		if (flags_set)
		{
			mnemonic_sample.setFlags(sample_flags);
		}

		if (limit_enable_disable_set)
		{
			mnemonic_sample.setLimitEnableDisable(sample_limit_enable_disable);
		}

		if (limit_enable_disable_set && sample_limit_enable_disable)
		{
			mnemonic_sample.setLimit(sample_limit);
		}

		if (static_set)
		{
			mnemonic_sample.setStalenessStatus(sample_static);
		}

		if (quality_set)
		{
			mnemonic_sample.setQuality(sample_quality);
		}

		mnemonic.addSample(mnemonic_sample);

		sample_idx++;
	}

	if (mnemonic_status_set)
	{
		//MEH - create a name labeling this field, value is purely for human
		//reading, as the the mnemonic object contains an implicit labeling 
		I16Field mnemonic_status_field("Mnemonic Status", mnemonic_status);

		mnemonic.setStatus(mnemonic_status_field);
	}

	if (mnemonic_units_set)
	{
		mnemonic.setUnits(mnemonic_units.c_str());
	}

	return mnemonic;
}


void InternalMnemonicMessage::init()
{
	setValue(NUM_OF_MNEMONICS_STRING, (GMSEC_I64) m_list.size());

	if ((getSpecVersion() <= GMSEC_ISD_2014_00) && (getField(MSG_ID_STRING) == NULL))
	{
		addField(MSG_ID_STRING, getSubject()); //MSG-ID only needed pre-2016
	}
}

}  // end namespace internal
}  // end namespace message
}  // end namespace mist
}  // end namespace api
}  // end namespace gmsec

