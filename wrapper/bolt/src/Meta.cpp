
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







#include <bolt/Meta.h>
#include <bolt/Log.h>
#include <bolt/Exception.h>
#include <bolt/util.h>


namespace bolt {


using namespace std;



Meta::Meta()
	:
	encodedSize(0)
{
}


Meta::Meta(const Meta &meta)
	:
	encodedSize(0)
{
	for (ConstIterator i = meta.properties.begin(); i != meta.properties.end(); ++i)
		add(i->second->clone());
}


Meta::~Meta()
{
	for (Iterator i = properties.begin(); i != properties.end(); ++i)
		delete i->second;

	properties.clear();
}


Meta& Meta::operator=(const Meta& other)
{
	if (this != &other)
	{
		encodedSize = other.encodedSize;
		for (ConstIterator i = other.properties.begin(); i != other.properties.end(); ++i)
			add(i->second->clone());
	}
	return *this;
}


bool Meta::parse(InputBuffer &buffer)
{
	bool ok = false;

	try
	{
		ok = parseAux(buffer);
	}
	catch (Exception &e)
	{
		GMSEC_WARNING << "Meta.parse: " << e.getText().c_str();
	}

	return ok;
}


bool Meta::parseAux(InputBuffer &buffer)
{
	if (buffer.available() < 6)
		throw IOException("invalid buffer available=" + util::toString(buffer.available()));

	i32 size = buffer.get4();
	if (size < 0)
		throw IOException("invalid buffer size=" + util::toString(size));

	i32 count = buffer.get2();
	if (count < 0 || count > MAX_ITEMS)
		throw IOException("invalid meta count=" + util::toString(count));

	for (i32 i = 0; i < count; ++i)
	{
		Property *property = Property::get(buffer);
		if (property)
			add(property);
	}

	return true;
}


bool Meta::add(Property *property)
{
	bool changed = false;

	Iterator i = properties.find(property->getName());

	if (i != properties.end())
	{
		changed = true;
		delete i->second;
	}

	properties[property->getName()] = property;

	encodedSize = 0;

	return changed;
}


Property *Meta::get(const string &name) const
{
	ConstIterator i = properties.find(name);

	Property *property = 0;

	if (i != properties.end())
		property = i->second;

	return property;
}


i32 Meta::determineSize() const
{
	if (encodedSize > 0)
		return encodedSize;

	i32 size = 4; // length
	size += 2; // count

	for (ConstIterator i = properties.begin(); i != properties.end(); ++i)
	{
		int prefix = 0;
		i32 tmp = i->second->getEncoding(prefix);
		size += prefix + tmp;
	}

	encodedSize = size;

	return size;
}


Result Meta::put(OutputBuffer &buffer) const
{
	Result result;

	try
	{
		i32 size = determineSize();

		if (buffer.available() < size)
			throw IOException("Meta.put: invalid buffer available=" + util::toString(buffer.available()));

		const u8 *start = buffer.at();

		buffer.put4(size);

		buffer.put2(properties.size());

		for (ConstIterator i = properties.begin(); i != properties.end(); ++i)
			i->second->put(buffer);

		if (buffer.at() != start + size)
			throw IOException("Meta.put: did not end where expected");
	}
	catch (Exception &e)
	{
		result.set(ERR_BUG, e.getText());
		GMSEC_WARNING << "Meta.put: " << e.getText().c_str();
	}

	return result;
}



ostream &Meta::put(ostream &os, const char *prefix) const
{
	os << prefix << "Meta[" << this << "] count=" << properties.size() << '\n';
	for (ConstIterator i = properties.begin(); i != properties.end(); ++i)
	{
		os << prefix;
		i->second->put(os);
	}
	// os << "--- end ---\n";
	return os;
}



void Meta::setID(const string &id)
{
	add(Property::createID(id));
}


string Meta::getID() const
{
	return getString("ID");
}


void Meta::setTopic(const string &topic)
{
	add(Property::createTopic(topic));
}


string Meta::getTopic() const
{
	return getString("TOPIC");
}


void Meta::setCorrID(const string &id)
{
	add(Property::createCorrID(id));
}


string Meta::getCorrID() const
{
	return getString("CORR_ID");
}


void Meta::setReplyTo(const string &topic)
{
	add(Property::createReplyTo(topic));
}


string Meta::getReplyTo() const
{
	return getString("REPLY_TO");
}


string Meta::getString(const string &key) const
{
	ConstIterator i = properties.find(key);
	if (i != properties.end())
		return i->second->asString();
	static string empty;
	return empty;
}


} // namespace bolt

