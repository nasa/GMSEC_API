/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file product_file_message.cpp
 *
 *  @brief This file contains functions for the management of a MIST ProductFileMessage object.
 *  The DeviceMessage is a specialization, or extension, of the standard GMSEC Message.
 */


#include <gmsec4/c/mist/product_file_message.h>

#include <gmsec4/mist/ProductFileMessage.h>

#include <gmsec4/Exception.h>
#include <gmsec4/Status.h>

using namespace gmsec::api;
using namespace gmsec::api::mist;



GMSEC_Message CALL_TYPE productFileMessageCreate(const char* subject, GMSEC_ResponseStatus response, const char* type, const char* subtype, unsigned int version, GMSEC_Status status)
{
	GMSEC_Message msg = NULL;
	Status        result;

	try
	{
		int tmp = response;
		ResponseStatus::Response resp = static_cast<ResponseStatus::Response>(tmp);

		msg = reinterpret_cast<GMSEC_Message>(new ProductFileMessage(subject, resp, type, subtype, version));
	}
	catch (Exception& e)
	{
		result = Status(e);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return msg;
}


GMSEC_Message CALL_TYPE productFileMessageCreateWithConfig(const char* subject, const GMSEC_Config config, GMSEC_ResponseStatus response, const char* type, const char* subtype, unsigned int version, GMSEC_Status status)
{
	GMSEC_Message msg = NULL;
	Status        result;

	const Config* cfg = reinterpret_cast<const Config*>(config);

	if (cfg)
	{
		try
		{
			int tmp = response;
			ResponseStatus::Response resp = static_cast<ResponseStatus::Response>(tmp);

			msg = reinterpret_cast<GMSEC_Message>(new ProductFileMessage(subject, *cfg, resp, type, subtype, version));
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Config handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return msg;
}


GMSEC_Message CALL_TYPE productFileMessageCreateUsingData(const char* data, GMSEC_Status status)
{
	GMSEC_Message msg = NULL;
	Status        result;

	try
	{
		msg = reinterpret_cast<GMSEC_Message>(new ProductFileMessage(data));
	}
	catch (Exception& e)
	{
		result = Status(e);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return msg;
}


GMSEC_Message CALL_TYPE productFileMessageCreateCopy(const GMSEC_Message other, GMSEC_Status status)
{
	GMSEC_Message msg = NULL;
	Status        result;

	const Message*            tmpMsg      = reinterpret_cast<const Message*>(other);
	const ProductFileMessage* prodFileMsg = dynamic_cast<const ProductFileMessage*>(tmpMsg);

	if (prodFileMsg)
	{
		msg = reinterpret_cast<GMSEC_Message>(new ProductFileMessage(*prodFileMsg));
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ProductFileMessage handle is NULL or is not a ProductFileMessage");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return msg;
}


GMSEC_ResponseStatus CALL_TYPE productFileMessageGetResponseStatus(const GMSEC_Message msg, GMSEC_Status status)
{
	GMSEC_ResponseStatus response = GMSEC_ACKNOWLEDGEMENT;
	Status               result;

	const Message*            tmpMsg      = reinterpret_cast<const Message*>(msg);
	const ProductFileMessage* prodFileMsg = dynamic_cast<const ProductFileMessage*>(tmpMsg);

	if (prodFileMsg)
	{
		int tmp = prodFileMsg->getResponseStatus();

		response = static_cast<GMSEC_ResponseStatus>(tmp);
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ProductFileMessage handle is NULL or is not a ProductFileMessage");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return response;
}


const char* CALL_TYPE productFileMessageGetProductType(const GMSEC_Message msg, GMSEC_Status status)
{
	const char* type = NULL;
	Status      result;

	const Message*            tmpMsg      = reinterpret_cast<const Message*>(msg);
	const ProductFileMessage* prodFileMsg = dynamic_cast<const ProductFileMessage*>(tmpMsg);

	if (prodFileMsg)
	{
		type = prodFileMsg->getProductType();
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ProductFileMessage handle is NULL or is not a ProductFileMessage");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return type;
}


const char* CALL_TYPE productFileMessageGetProductSubtype(const GMSEC_Message msg, GMSEC_Status status)
{
	const char* subtype = NULL;
	Status      result;

	const Message*            tmpMsg      = reinterpret_cast<const Message*>(msg);
	const ProductFileMessage* prodFileMsg = dynamic_cast<const ProductFileMessage*>(tmpMsg);

	if (prodFileMsg)
	{
		subtype = prodFileMsg->getProductSubtype();
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ProductFileMessage handle is NULL or is not a ProductFileMessage");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return subtype;
}


void CALL_TYPE productFileMessageAddProductFile(GMSEC_Message msg, const GMSEC_ProductFile productFile, GMSEC_Status status)
{
	Status result;

	Message*            tmpMsg      = reinterpret_cast<Message*>(msg);
	ProductFileMessage* prodFileMsg = dynamic_cast<ProductFileMessage*>(tmpMsg);
	const ProductFile*  prodFile    = reinterpret_cast<const ProductFile*>(productFile);

	if (!prodFileMsg)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ProductFileMessage handle is NULL or is not a ProductFileMessage");
	}
	else if (!prodFile)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ProductFile handle is NULL");
	}
	else
	{
		prodFileMsg->addProductFile(*prodFile);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


size_t CALL_TYPE productFileMessageGetNumProductFiles(const GMSEC_Message msg, GMSEC_Status status)
{
	size_t numFiles = 0;
	Status result;

	const Message*            tmpMsg      = reinterpret_cast<const Message*>(msg);
	const ProductFileMessage* prodFileMsg = dynamic_cast<const ProductFileMessage*>(tmpMsg);

	if (prodFileMsg)
	{
		numFiles = prodFileMsg->getNumProductFiles();
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ProductFileMessage handle is NULL or is not a ProductFileMessage");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return numFiles;
}


const GMSEC_ProductFile CALL_TYPE productFileMessageGetProductFile(const GMSEC_Message msg, size_t index, GMSEC_Status status)
{
	GMSEC_ProductFile pfile = NULL;
	Status            result;

	const Message*            tmpMsg      = reinterpret_cast<const Message*>(msg);
	const ProductFileMessage* prodFileMsg = dynamic_cast<const ProductFileMessage*>(tmpMsg);

	if (prodFileMsg)
	{
		try
		{
			ProductFile& tmp = const_cast<ProductFile&>(prodFileMsg->getProductFile(index));

			pfile = reinterpret_cast<GMSEC_ProductFile>(&tmp);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ProductFileMessage handle is NULL or is not a ProductFileMessage");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return pfile;
}


GMSEC_Message CALL_TYPE productFileMessageConvert(const GMSEC_Message msg, GMSEC_Status status)
{
	GMSEC_Message productFileMsg = NULL;
	Status        result;

	const Message* tmpMsg = reinterpret_cast<const Message*>(msg);

	if (tmpMsg)
	{
		ProductFileMessage tmpProductFileMsg = ProductFileMessage::convertMessage(*tmpMsg);

		productFileMsg = reinterpret_cast<GMSEC_Message>(new ProductFileMessage(tmpProductFileMsg));
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return productFileMsg;
}
