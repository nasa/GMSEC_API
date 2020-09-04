/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file InternalProductFileMessage.cpp
 *
 * @brief This file contains the internal implementation of ProductFile messages.
 */

// Disable the Visual Studio warning C4355
//   ('this' : used in base member initializer list)
//   Found in the Connection(Server s, const Options &o) constructor
#ifdef WIN32
#pragma warning(disable: 4355)
#endif

#include <gmsec4/mist/message/ProductFileMessage.h>

#include <gmsec4/internal/mist/message/InternalProductFileMessage.h>

#include <gmsec4/internal/mist/InternalProductFileIterator.h>
#include <gmsec4/internal/mist/MessageSubclassHelper.h>
#include <gmsec4/internal/mist/MessageTemplate.h>

#include <gmsec4/mist/ProductFile.h>
#include <gmsec4/mist/mist_defs.h>

#include <gmsec4/util/Log.h>

#include <gmsec4/Exception.h>

#include <gmsec4/internal/StringUtil.h>

#include <memory>
#include <sstream>


using namespace gmsec::api::mist::internal;
using namespace gmsec::api::mist::message;
using namespace gmsec::api::util;


static const char* HEADER_VERSION_STRING    = "HEADER-VERSION";
//static const char* MESSAGE_TYPE_STRING      = "MESSAGE-TYPE";
static const char* MESSAGE_SUBTYPE_STRING   = "MESSAGE-SUBTYPE";
//static const char* CONTENT_VERSION_STRING   = "CONTENT-VERSION";
//static const char* MSG_STRING               = "MSG";
static const char* PROD_STRING              = "PROD";
static const char* RESPONSE_STATUS_STRING   = "RESPONSE-STATUS";
static const char* PROD_TYPE_STRING         = "PROD-TYPE";
static const char* PROD_SUBTYPE_STRING      = "PROD-SUBTYPE";
static const char* NUM_OF_FILES_STRING      = "NUM-OF-FILES";
static const char* NUM_OF_INPUT_FILES_STRING= "NUM-OF-INPUT-FILES";


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

InternalProductFileMessage::InternalProductFileMessage(const char* subject, 
													   ResponseStatus::Response responseStatus, 
													   const char* schemaID,
													   const Specification& spec)
	: InternalMistMessage(subject, InternalMistMessage::findKind(schemaID), schemaID, spec),
	  m_list(),
	  m_productFileIterator(*this),
	  m_responseStatus(responseStatus)
{
	init(responseStatus, schemaID);
}


InternalProductFileMessage::InternalProductFileMessage(const char* subject, 
													   ResponseStatus::Response responseStatus, 
													   const char* schemaID,
													   const gmsec::api::Config& config, 
													   const Specification& spec)
	: InternalMistMessage(subject, InternalMistMessage::findKind(schemaID), schemaID, config, spec),
	  m_list(),
	  m_productFileIterator(*this),
	  m_responseStatus(responseStatus)
{
	init(responseStatus, schemaID);
}


InternalProductFileMessage::InternalProductFileMessage(const InternalProductFileMessage& other)
	: InternalMistMessage(other),
	  m_list(other.m_list),
	  m_productFileIterator(*this),
	  m_responseStatus(other.getResponseStatus()),
	  m_productType(other.getProductType()),
	  m_productSubtype(other.getProductSubtype())
{
	init(m_responseStatus, other.getSchemaID());
}



InternalProductFileMessage::InternalProductFileMessage(const char* data)
	: InternalMistMessage(data),
	  m_list(),
	  m_productFileIterator(*this)
{
	MessageSubclassHelper::checkFloatField(HEADER_VERSION_STRING, "InternalProductFileMessage()",
		2010.0, *this);

	MessageSubclassHelper::checkStringField(MESSAGE_SUBTYPE_STRING, "InternalProductFileMessage()",
		PROD_STRING, *this);


	std::string prodType = MessageSubclassHelper::extractStringField(PROD_TYPE_STRING, "InternalProductFileMessage::InternalProductFileMessage()", *this);
	std::string schemaID = buildSchemaID(getKind(), PROD_STRING, prodType.c_str(), getSpecVersion());
	registerTemplate(schemaID.c_str());


	GMSEC_U16 num_of_files;
	try
	{
		if(getSpecVersion() == GMSEC_ISD_2016_00)
		{
			num_of_files = MessageSubclassHelper::extractU16Field(NUM_OF_FILES_STRING, 
								"InternalProductFileMessage::InternalProductFileMessage()", *this);
		}
		else
		{
			num_of_files = MessageSubclassHelper::extractI16Field(NUM_OF_FILES_STRING, 
								"InternalProductFileMessage::InternalProductFileMessage()", *this);
		}
	}
	catch(...)
	{//this is an optional field, not a required field
	 //if we can't find the field, that's ok.  It means we have 0 product files.
		num_of_files = 0;
	}

	if(num_of_files==0)
	{//request messages uses a differently named field for its productfile count
		try
		{
			if(getSpecVersion() == GMSEC_ISD_2016_00){
				num_of_files = MessageSubclassHelper::extractU16Field(NUM_OF_INPUT_FILES_STRING, 
									"InternalProductFileMessage::InternalProductFileMessage()", *this);
			}
			else
			{
				num_of_files = MessageSubclassHelper::extractI16Field(NUM_OF_INPUT_FILES_STRING, 
									"InternalProductFileMessage::InternalProductFileMessage()", *this);
			}
		}
		catch(...)
		{//this is an optional field, not a required field
		 //if we can't find the field, that's ok.  It means we have 0 product files.
			num_of_files = 0;
		}
	}

	for (GMSEC_I16 count = 0; count < num_of_files; count++)
	{
		m_list.push_back(extractMessageProductFile(count+1));
	}

	m_responseStatus = static_cast<ResponseStatus::Response>(MessageSubclassHelper::extractI16Field(RESPONSE_STATUS_STRING, 
						"InternalProductFileMessage::InternalProductFileMessage()", *this));
		
	m_productType = MessageSubclassHelper::extractStringField(PROD_TYPE_STRING, 
												  "InternalProductFileMessage::InternalProductFileMessage()",
												  *this);
	m_productSubtype = MessageSubclassHelper::extractStringField(PROD_SUBTYPE_STRING, 
												  "InternalProductFileMessage::InternalProductFileMessage()",
												  *this);
}


InternalProductFileMessage::~InternalProductFileMessage()
{
}


void InternalProductFileMessage::addProductFile(const ProductFile& productFile)
{
	m_list.push_back(productFile);

	if(getSpecVersion() == GMSEC_ISD_2014_00){
		addField(NUM_OF_FILES_STRING, (GMSEC_I16) m_list.size());
	}else if(getSpecVersion() == GMSEC_ISD_2016_00){
		addField(NUM_OF_FILES_STRING, (GMSEC_U16) m_list.size());
	}else{
		GMSEC_WARNING << "Specification version unknown: " << getSpecVersion() << ", unable to autopopulate"
			<< " Product Message " << NUM_OF_FILES_STRING;
	}
	

	std::ostringstream ss;
	ss << "FILE." << m_list.size();
	std::string fileCountStr = ss.str();

	std::string nameCount = fileCountStr;
	nameCount.append(".NAME");
	addField(nameCount.c_str(), productFile.getName());

	std::string descCount = fileCountStr;
	descCount.append(".DESCRIPTION");
	addField(descCount.c_str(), productFile.getDescription());

	std::string formatCount = fileCountStr;
	formatCount.append(".FORMAT");
	addField(formatCount.c_str(), productFile.getFormat());

	std::string versionCount = fileCountStr;
	versionCount.append(".VERSION");
	addField(versionCount.c_str(), productFile.getVersion());

	if (productFile.uriAvailable())
	{
		std::string uriCount = fileCountStr;
		uriCount.append(".URI");
		addField(uriCount.c_str(), productFile.getURI());
	}

	if (productFile.contentsAvailable())
	{
		GMSEC_BIN fileContents;
		size_t size = productFile.getContents(fileContents);
		std::string sizeCount = fileCountStr;
		sizeCount.append(".SIZE");
		if(getSpecVersion() == GMSEC_ISD_2014_00){
			addField(sizeCount.c_str(), (GMSEC_I32) size);
		}else if(getSpecVersion() == GMSEC_ISD_2016_00){
			addField(sizeCount.c_str(), (GMSEC_U32) size);
		}else{
			GMSEC_WARNING << "Specification version unknown: " << getSpecVersion() << ", unable to autopopulate" << " Product Message " << sizeCount.c_str();
		}

		std::string dataCount = fileCountStr;
		dataCount.append(".DATA");
		addField(dataCount.c_str(), fileContents, size);
	}
}


const ProductFile& InternalProductFileMessage::getProductFile(size_t index) const
{
	if (index < m_list.size())
	{
		return m_list[index];
	}

	std::ostringstream oss;

	oss << "InternalProductFileMessage::getProductFile():  index "
	    << index << "specified is out-of-range";

	throw Exception(MIST_ERROR, INDEX_OUT_OF_RANGE, oss.str().c_str());
}


size_t InternalProductFileMessage::getNumProductFiles() const
{
	return m_list.size();
}


ResponseStatus::Response InternalProductFileMessage::getResponseStatus() const
{
	return m_responseStatus;
}


const char* InternalProductFileMessage::getProductType() const
{
	return m_productType.c_str();
}


const char* InternalProductFileMessage::getProductSubtype() const
{
	return m_productSubtype.c_str();
}


ProductFileIterator& InternalProductFileMessage::getProductFileIterator()
{
	m_productFileIterator.m_iter->reset();
	return m_productFileIterator;
}


void InternalProductFileMessage::resetProductFileIterator()
{
	m_listIter = m_list.begin();
}


bool InternalProductFileMessage::hasNextProductFile() const
{
	return m_listIter != m_list.end();
}


const ProductFile& InternalProductFileMessage::nextProductFile()
{
	if (!hasNextProductFile())
	{
		throw Exception(MIST_ERROR, ITER_INVALID_NEXT, "No more ProductFiles available");
	}

	const ProductFile& productFile = *m_listIter;

	++m_listIter;

	return productFile;
}


ProductFile InternalProductFileMessage::extractMessageProductFile(size_t index)
{
	char           tmp_name[256];
	
	GMSEC_I32   product_size;
	GMSEC_BIN   product_data        = NULL;

	StringUtil::stringFormat(tmp_name, sizeof(tmp_name), "FILE.%u.NAME", index);
	std::string product_name = MessageSubclassHelper::extractStringField(tmp_name, 
													  "InternalProductFileMessage::extractMessageProductFile()",
													  *this);

	StringUtil::stringFormat(tmp_name, sizeof(tmp_name), "FILE.%u.DESCRIPTION", index);
	std::string product_description = MessageSubclassHelper::extractStringField(tmp_name, 
													  "InternalProductFileMessage::extractMessageProductFile()",
													  *this);
	
	StringUtil::stringFormat(tmp_name, sizeof(tmp_name), "FILE.%u.FORMAT", index);
	std::string product_format = MessageSubclassHelper::extractStringField(tmp_name, 
													  "InternalProductFileMessage::extractMessageProductFile()",
													  *this);
	

	StringUtil::stringFormat(tmp_name, sizeof(tmp_name), "FILE.%u.VERSION", index);
	std::string product_version = MessageSubclassHelper::extractStringField(tmp_name, 
													  "InternalProductFileMessage::extractMessageProductFile()",
													  *this);


	StringUtil::stringFormat(tmp_name, sizeof(tmp_name), "FILE.%u.URI", index);

	std::string product_uri;
	bool product_uri_exists = MessageSubclassHelper::getOptionalString(tmp_name, *this, product_uri);
	
	StringUtil::stringFormat(tmp_name, sizeof(tmp_name), "FILE.%u.SIZE", index);

	bool product_size_exists;
	if (getSpecVersion() == GMSEC_ISD_2016_00)
	{
		GMSEC_U32 uProductSize;
		product_size_exists = MessageSubclassHelper::getOptionalU32(tmp_name, *this, uProductSize);
		product_size = uProductSize;
	}
	else
	{
		product_size_exists = MessageSubclassHelper::getOptionalI32(tmp_name, *this, product_size);
	}
		

	StringUtil::stringFormat(tmp_name, sizeof(tmp_name), "FILE.%u.DATA", index);

	try
	{
		const BinaryField& tmp_field = getBinaryField(tmp_name);
		product_data = tmp_field.getValue();
	}
	catch (...)
	{
		// No problem, optional data field may be missing
	}

	if(!product_uri_exists && (!product_size_exists || product_data == NULL))
	{
		throw Exception(MIST_ERROR,
			MISSING_REQUIRED_FIELD,
			"InternalProductFileMessage::extractMessageProductFile:  Either a URI or File Contents must be given, neither have been specified.");
	}

	if (product_uri_exists)
	{
		ProductFile product_file(product_name.c_str(),
	                                 product_description.c_str(),
	                                 product_version.c_str(),
	                                 product_format.c_str(),
	                                 product_uri.c_str());

		return product_file;
	}
	else
	{
		ProductFile product_file(product_name.c_str(),
		                         product_description.c_str(),
		                         product_version.c_str(),
		                         product_format.c_str(),
		                         product_data,
		                         product_size);

		return product_file;
	}
}


void InternalProductFileMessage::init(ResponseStatus::Response responseStatus, const char* schemaID)
{	
	const DataList<FieldTemplate>& fieldTemplates = getTemplate().listFieldTemplates();
	for(DataList<FieldTemplate>::const_iterator it = fieldTemplates.begin(); it != fieldTemplates.end(); ++it)
	{
		FieldTemplate temp = *it;
		if(!StringUtil::stringEquals(temp.getValue(), ""))
		{//the field template has a predefined value, so we'll add the field to the message
			try
			{
				std::auto_ptr<Field> field(temp.toField());

				addField(*(field.get()));
			}
			catch(...)
			{
				//ignore control fields, do nothing
			}
		}
	}
	setValue(RESPONSE_STATUS_STRING, (GMSEC_I64)responseStatus);	
}


}  // end namespace internal
}  // end namespace message
}  // end namespace mist
}  // end namespace api
}  // end namespace gmsec
