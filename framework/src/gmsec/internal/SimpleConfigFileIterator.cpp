/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





/** @file SimpleConfigFileIterator.cpp
 *
 *  This file contains the implementation of the ConfigFileIterator class
 *
 */

#include <gmsec/internal/SimpleConfigFileIterator.h>
#include <gmsec/internal/BaseConfig.h>
#include <gmsec/internal/BaseConfigFile.h>
#include <gmsec/internal/InternalMessage.h>
#include <gmsec/internal/MessageBuddy.h>
#include <gmsec/internal/strutil.h>
#include <gmsec/internal/Log.h>
#include <tinyxml2.h>


namespace gmsec {
namespace internal {


using namespace tinyxml2;
	

SimpleConfigFileIterator::SimpleConfigFileIterator()
	: initialized(false),
	  cfgIter(0),
	  msgIter(0),
	  subIter(0),
	  customXmlIter(0)
{
}


SimpleConfigFileIterator::~SimpleConfigFileIterator()
{
	uninitialize();
}


void SimpleConfigFileIterator::uninitialize()
{
	if (initialized)
	{
		delete cfgIter;
		delete msgIter;
		delete subIter;
		delete customXmlIter;

		cfgIter       = 0;
		msgIter       = 0;
		subIter       = 0;
		customXmlIter = 0;

		initialized = false;
	}
}


Status SimpleConfigFileIterator::Initialize(gmsec::util::List<NodeItr> &nodeIters)
{
	Status result;

	uninitialize();

	//Set Iterator Structs(end, cursorMain, cursorName)
	NodeItr it;
	//Configs
	cfgIter = new iterInfo;
	nodeIters.get(0,it);	//Cfg_begin
	cfgIter->cursorMain = it;
	cfgIter->cursorName = it;
	nodeIters.get(1,it);	//Cfg_end
	cfgIter->end = it;

	//Messages
	msgIter = new iterInfo;
	nodeIters.get(2,it);	//Msg_begin
	msgIter->cursorMain = it;
	msgIter->cursorName = it;
	nodeIters.get(3,it);	//Msg_end
	msgIter->end = it;
	
	//Subscriptions
	subIter = new iterInfo;
	nodeIters.get(4,it);	//Sub_begin
	subIter->cursorMain = it;
	subIter->cursorName = it;
	nodeIters.get(5,it);	//Sub_end
	subIter->end = it;		

	//Custom XML Elements
	customXmlIter = new iterInfo;
	nodeIters.get(6,it);	//CustomXML_begin
	customXmlIter->cursorMain = it;
	customXmlIter->cursorName = it;
	nodeIters.get(7,it);	//CustmoXML_end
	customXmlIter->end = it;

	initialized = true;
	return result;
}


bool SimpleConfigFileIterator::HasNextSubscriptionName()
{	
	if(!initialized)
	{
		return false;
	}
	return HasNextAux(subIter, true);
}


bool SimpleConfigFileIterator::HasNextConfigName()
{
	if(!initialized)
	{
		return false;
	}
	return HasNextAux(cfgIter, true);
}


bool SimpleConfigFileIterator::HasNextMessageName()
{
	if(!initialized)
	{
		return false;
	}
	return HasNextAux(msgIter, true);
}


bool SimpleConfigFileIterator::HasNextSubscription()
{
	if(!initialized)
	{
		return false;
	}
	return HasNextAux(subIter, false);
}


bool SimpleConfigFileIterator::HasNextConfig()
{
	if(!initialized)
	{
		return false;
	}
	return HasNextAux(cfgIter, false);
}


bool SimpleConfigFileIterator::HasNextMessage()
{
	if(!initialized)
	{
		return false;
	}
	return HasNextAux(msgIter, false);
}


bool SimpleConfigFileIterator::HasNextCustomElement()
{
	if(!initialized)
	{
		return false;
	}
	return HasNextAux(customXmlIter, false);
}


bool SimpleConfigFileIterator::HasNextAux(iterInfo *iter, bool name)
{
	if(name)
	{
		if(iter->cursorName == iter->end)
		{
			return false;
		}
		return true;
	}
	else
	{
		if(iter->cursorMain == iter->end)
		{
			return false;
		}
		return true;
	}
}


Status SimpleConfigFileIterator::NextSubscriptionName(gmsec::util::String &gmstr)
{
	Status result;
	if(!initialized)
	{
		return Error_UNINITIALIZED();
	}
	if(HasNextAux(subIter,true))
	{
		gmstr = gmsec::util::String(subIter->cursorName->first.c_str());
		subIter->cursorName++;
	}
	else
	{
		return Error_END();
	}
	return result;
}


Status SimpleConfigFileIterator::NextConfigName(gmsec::util::String &gmstr)
{
	Status result;
	if(!initialized)
	{
		return Error_UNINITIALIZED();
	}
	if(HasNextAux(cfgIter,true))
	{
		gmstr = gmsec::util::String(cfgIter->cursorName->first.c_str());
		cfgIter->cursorName++;
	}
	else
	{
		return Error_END();
	}
	return result;
}


Status SimpleConfigFileIterator::NextMessageName(gmsec::util::String &gmstr)
{
	Status result;
	if(!initialized)
	{
		return Error_UNINITIALIZED();
	}
	if(HasNextAux(msgIter,true))
	{
		gmstr = gmsec::util::String(msgIter->cursorName->first.c_str());
		msgIter->cursorName++;
	}
	else
	{
		return Error_END();
	}
	return result;
}


Status SimpleConfigFileIterator::NextSubscription(gmsec::util::String &gmstr)
{
	Status result;
	if(!initialized)
	{
		return Error_UNINITIALIZED();
	}
	if(HasNextAux(subIter,false))
	{
		XMLElement *element = subIter->cursorMain->second;
		gmstr = gmsec::util::String(getStrAttr(element, "PATTERN"));
		subIter->cursorMain++;
	}
	else
	{
		return Error_END();
	}
	return result;
}


Status SimpleConfigFileIterator::NextConfig(Config *config)
{
	Status result;
	if(!initialized)
	{
		return Error_UNINITIALIZED();
	}
	if(HasNextAux(cfgIter,false))
	{
		XMLElement *element = cfgIter->cursorMain->second;
		BaseConfig::getInternal(config)->FromXML(element);
		cfgIter->cursorMain++;
	}
	else
	{
		return Error_END();
	}
	return result;
}


Status SimpleConfigFileIterator::NextMessage(Message *message)
{
	Status result;
	if(!initialized)
	{
		return Error_UNINITIALIZED();
	}
	if(HasNextAux(msgIter,false))
	{
		XMLElement *element = msgIter->cursorMain->second;
		MessageBuddy::getInternal(message)->FromXML(BaseConfigFile::ElementToString(element).c_str());
		msgIter->cursorMain++;
	}
	else
	{
		return Error_END();
	}
	return result;
}


Status SimpleConfigFileIterator::NextCustomElement(gmsec::util::String &gmstr)
{
	Status result;
	if(!initialized)
	{
		return Error_UNINITIALIZED();
	}
	if(HasNextAux(customXmlIter,false))
	{
		XMLPrinter printer;
		XMLElement *element = customXmlIter->cursorMain->second;
		element->Accept(&printer);

		// If the last character in the printer string is a newline character, additional
		// subtraction is necessary.  The buffer may or may not have a newline character,
		// depending on the current tinyxml2 document settings (ie. compact == true).
		std::string elementStr = std::string(printer.CStr(), printer.CStrSize() - 1);
		size_t actualSize = elementStr.size() - (elementStr.c_str()[elementStr.size() - 1] == '\n' ? 1 : 0);

		gmstr = gmsec::util::String(elementStr.substr(0, actualSize).c_str());
		customXmlIter->cursorMain++;
	}
	else
	{
		return Error_END();
	}
	return result;
}


const char* SimpleConfigFileIterator::getStrAttr(XMLElement *element, const char * field)
{
	BaseConfigFile cfile;

	const XMLAttribute *attr = NULL;
	for (attr = element->FirstAttribute(); attr; attr = attr->Next())
	{
		const char *caname = attr->Name();
		if (caname != NULL && cfile.nameOrAliasMatches(caname, field))
		{
			return attr->Value();
		}
	}
	return NULL;
}


Status SimpleConfigFileIterator::Error_UNINITIALIZED()
{
	//Report CFI not initialized
	Status result;
	result.Set(GMSEC_STATUS_ITERATOR_ERROR,
			   GMSEC_INVALID_NEXT,
			   "Iterator never initialized!");
	LOG_WARNING << result.Get();
	return result;
}


Status SimpleConfigFileIterator::Error_END()
{
	//Report an invalid next (cursor on end)
	Status result;
	result.Set(GMSEC_STATUS_ITERATOR_ERROR,
		       GMSEC_INVALID_NEXT,
			   "Next called on iterator at end!");
	LOG_WARNING << result.Get();
	return result;
}


}  //namespace internal
}  //namespace gmsec

