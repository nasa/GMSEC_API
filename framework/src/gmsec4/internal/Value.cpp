/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file Value.cpp
 */


#include <gmsec4/internal/Value.h>

#include <gmsec4/internal/StringUtil.h>

#include <gmsec4/Message.h>

#include <gmsec4/util/Log.h>

#include <algorithm>
#include <iterator>		// inserter


using namespace gmsec::api::util;


namespace gmsec
{
namespace api
{
namespace internal
{


class I32Value : public Value
{
public:
	I32Value(GMSEC_I32 x) : value(x) { }

	bool isInteger() const {
		return true;
	}

	Status getBoolean(bool& out) const
	{
		Status status;
		out = value != 0;
		return status;
	}

	Status getI32(GMSEC_I32& out) const
	{
		Status status;
		out = value;
		return status;
	}

	Status getF64(GMSEC_F64& out) const
	{
		Status status;
		out = value;
		return status;
	}

	Status getString(std::string& out) const
	{
		Status status;
		char buffer[32];
		StringUtil::stringFormat(buffer, sizeof(buffer), "%d", value);
		out = buffer;
		return status;
	}

	virtual Value* clone() const
	{
		return new I32Value(*this);
	}

private:
	GMSEC_I32 value;
};


class BooleanValue : public Value
{
public:
	BooleanValue(bool x) : value(x) { }

	bool isBoolean() const {
		return true;
	}

	Status getBoolean(bool& out) const
	{
		Status status;
		out = value;
		return status;
	}

	Status getI32(GMSEC_I32& out) const
	{
		Status status;
		if (value)
			out = 1;
		else
			out = 0;
		return status;
	}

	Status getF64(GMSEC_F64& out) const
	{
		Status status;
		if (value)
			out = 1;
		else
			out = 0;
		return status;
	}

	Status getString(std::string& out) const
	{
		Status status;
		if (value)
			out = "TRUE";
		else
			out = "FALSE";
		return status;
	}

	virtual Value* clone() const
	{
		return new BooleanValue(*this);
	}

private:
	bool value;
};


class StringValue : public Value
{
public:
	StringValue(const std::string& x) : value(x) { }

	bool isString() const {
		return true;
	}

	Status getBoolean(bool& out) const
	{
		Status status;
		out = StringUtil::stringIsTrue(value.c_str());
		return status;
	}

	Status getI32(GMSEC_I32& out) const
	{
		Status status;
		if (!StringUtil::stringParseI32(value.c_str(), out))
		{
			status.set(FIELD_ERROR, FIELD_TYPE_MISMATCH, "Invalid I32 string");
		}
		return status;
	}

	Status getF64(GMSEC_F64& out) const
	{
		Status status;
		if (!StringUtil::stringParseF64(value.c_str(), out))
		{
			status.set(FIELD_ERROR, FIELD_TYPE_MISMATCH, "Invalid F64 string");
		}
		return status;
	}

	Status getString(std::string& out) const
	{
		Status status;
		out = value;
		return status;
	}

	virtual Value* clone() const
	{
		return new StringValue(*this);
	}

private:
	std::string value;
};


class F64Value : public Value
{
public:
	F64Value(GMSEC_F64 x) : value(x) { }

	bool isReal() const {
		return true;
	}

	Status getBoolean(bool& out) const
	{
		Status status;
		out = value != 0;
		return status;
	}

	Status getI32(GMSEC_I32& out) const
	{
		Status status;
		out = (GMSEC_I32) value;
		if (out != value)
		{
			status.set(FIELD_ERROR, FIELD_TYPE_MISMATCH, "getI32: truncated F64Value");
		}
		return status;
	}

	Status getF64(GMSEC_F64& out) const
	{
		Status status;
		out = value;
		return status;
	}

	Status getString(std::string& out) const
	{
		Status status;
		char buffer[32];
		StringUtil::stringFormat(buffer, sizeof(buffer), "%d", value);
		out = buffer;
		return status;
	}

	virtual Value* clone() const
	{
		return new F64Value(*this);
	}

private:
	GMSEC_F64 value;
};



Value::~Value()
{
}


ValueMap::ValueMap()
{
}


ValueMap::~ValueMap()
{
	for (MapType::iterator i = values.begin(); i != values.end(); ++i)
	{
		delete i->second;
	}
}


bool ValueMap::contains(const char* key) const
{
	return values.find(key) != values.end();
}

size_t GMSEC_API ValueMap::size() const
{
	return values.size();
}


Status ValueMap::getBoolean(const char* key, bool& value, bool* pdefault) const
{
	Status status;

	MapType::const_iterator i = values.find(key);
	if (i == values.end())
	{
		if (pdefault)
		{
			value = *pdefault;
		}
		else
		{
			status.set(CONFIG_ERROR, INVALID_CONFIG_NAME, "getBoolean: missing");
		}
	}
	else
	{
		status = i->second->getBoolean(value);
	}

	return status;
}


Status ValueMap::getI32(const char* key, GMSEC_I32& value, GMSEC_I32* pdefault) const
{
	Status status;

	MapType::const_iterator i = values.find(key);
	if (i == values.end())
	{
		if (pdefault)
		{
			value = *pdefault;
		}
		else
		{
			status.set(CONFIG_ERROR, INVALID_CONFIG_NAME, "getI32: missing");
		}
	}
	else
	{
		status = i->second->getI32(value);
	}

	return status;
}


Status ValueMap::getF64(const char* key, GMSEC_F64& value, GMSEC_F64* pdefault) const
{
	Status status;

	MapType::const_iterator i = values.find(key);
	if (i == values.end())
	{
		if (pdefault)
		{
			value = *pdefault;
		}
		else
		{
			status.set(CONFIG_ERROR, INVALID_CONFIG_NAME, "getF64: missing");
		}
	}
	else
	{
		status = i->second->getF64(value);
	}

	return status;
}


Status ValueMap::getString(const char* key, std::string& value, std::string* pdefault) const
{
	Status status;

	MapType::const_iterator i = values.find(key);
	if (i == values.end())
	{
		if (pdefault)
		{
			value = *pdefault;
		}
		else
		{
			status.set(CONFIG_ERROR, INVALID_CONFIG_NAME, "getString: missing");
		}
	}
	else
	{
		status = i->second->getString(value);
	}

	return status;
}


Value* ValueMap::getOpaqueValue(const char* key)
{
	Value* value = 0;

	MapType::iterator it = values.find(key);

	if (it != values.end())
	{
		value = it->second;
	}

	return value;
}


void ValueMap::setBoolean(const char* key, bool value)
{
	delete values[key];
	values[key] = new BooleanValue(value);
}


void ValueMap::setI32(const char* key, GMSEC_I32 value)
{
	delete values[key];
	values[key] = new I32Value(value);
}


void ValueMap::setF64(const char* key, GMSEC_F64 value)
{
	delete values[key];
	values[key] = new F64Value(value);
}


void ValueMap::setString(const char* key, const std::string& value)
{
	delete values[key];
	values[key] = new StringValue(value);
}


void ValueMap::setOpaqueValue(const char* key, Value* value)
{
	delete values[key];
	values[key] = value;
}


void ValueMap::clearValue(const char* key)
{
	MapType::iterator it = values.find(key);

	if (it != values.end())
	{
		delete it->second;
		values.erase(it);
	}
}


void ValueMap::reset(ValueMap::Iterator& i) const
{
	i.pmap = &values;
	i.current = values.begin();
	i.id.clear();
}


ValueMap::Iterator::Iterator()
	: pmap(0)
{
}


ValueMap::Iterator::Iterator(ValueMap::MapType& vm)
	: pmap(&vm),
	  current(vm.begin())
{
}


ValueMap::Iterator::~Iterator()
{
}


const Value* ValueMap::Iterator::next()
{
	if (!pmap)
	{
		return 0;
	}
	if (current == pmap->end())
	{
		id = "";
		return 0;
	}

	id = current->first;
	const Value *value = current->second;
	++current;
	return value;
}


template<typename T_PAIR>
struct Clone : public std::unary_function<T_PAIR, typename T_PAIR::first_type>
{
	const T_PAIR operator()(const T_PAIR& item) const
	{
		return T_PAIR(item.first, item.second->clone());
	}
};


void ValueMap::cloneValues(const ValueMap& from)
{
	// Check if there are already values in our current map; delete it necessary.
	for (MapType::iterator i = values.begin(); i != values.end(); ++i)
	{
		delete i->second;
	}

	// Clone 'from' values into our current map.
	std::transform(from.values.begin(), from.values.end(), std::inserter(values, values.begin()), Clone<MapType::value_type>());
}


void transferHeaderFields(const Message& from, ValueMap& to)
{
	MessageFieldIterator& iter = from.getFieldIterator(MessageFieldIterator::HEADER_FIELDS);

	while (iter.hasNext())
	{
		const Field& field = iter.next();

		const char*      name = field.getName();
		Field::FieldType type = field.getType();


		switch (type)
		{
			case Field::BOOL_TYPE: {
				bool value = dynamic_cast<const BooleanField&>(field).getValue();
				to.setBoolean(name, value);
				break;
			}
			case Field::CHAR_TYPE: {
				GMSEC_CHAR value = dynamic_cast<const CharField&>(field).getValue();
				to.setI32(name, value);
				break;
			}
			case Field::I8_TYPE: {
				GMSEC_I8 value = dynamic_cast<const I8Field&>(field).getValue();
				to.setI32(name, value);
				break;
			}
			case Field::U8_TYPE: {
				GMSEC_U8 value = dynamic_cast<const U8Field&>(field).getValue();
				to.setI32(name, value);
				break;
			}
			case Field::I16_TYPE: {
				GMSEC_I16 value = dynamic_cast<const I16Field&>(field).getValue();
				to.setI32(name, value);
				break;
			}
			case Field::U16_TYPE: {
				GMSEC_U16 value = dynamic_cast<const U16Field&>(field).getValue();
				to.setI32(name, value);
				break;
			}
			case Field::I32_TYPE: {
				GMSEC_I32 value = dynamic_cast<const I32Field&>(field).getValue();
				to.setI32(name, value);
				break;
			}
			case Field::U32_TYPE: {
				GMSEC_U32 value = dynamic_cast<const U32Field&>(field).getValue();
				if (value > 2147483647) {
					GMSEC_WARNING << "Underflow while attempting to store GMSEC_U32 as a GMSEC_I32";
				}
				to.setI32(name, (GMSEC_I32) value);
				break;
			}
			case Field::STRING_TYPE: {
				const char* value = dynamic_cast<const StringField&>(field).getValue();
				to.setString(name, value);
				break;
			}
			case Field::F32_TYPE: {
				GMSEC_F32 value = dynamic_cast<const F32Field&>(field).getValue();
				to.setF64(name, value);
				break;
			}
			case Field::F64_TYPE: {
				GMSEC_F64 value = dynamic_cast<const F64Field&>(field).getValue();
				to.setF64(name, value);
				break;
			}
			default:
				GMSEC_WARNING << "Unhandled header field type: " << type;
				break;
		}
	}
}

}  // end namespace internal
}  // end namespace api
}  // end namespace gmsec
