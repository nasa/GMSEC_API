/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file gmsec/internal/Value.cpp
*/


#include <gmsec/internal/Value.h>
#include <gmsec/internal/strutil.h>
#include <gmsec/Message.h>

#include <algorithm>
#include <iterator>		// inserter


using namespace gmsec::util;

using std::string;
using std::map;


namespace gmsec
{
namespace internal
{


class I32Value : public Value
{
public:
	I32Value (GMSEC_I32 x) : value(x) { }

	bool isInteger() const {
		return true;
	}

	Status getBoolean (bool &out) const
	{
		Status status;
		out = value != 0;
		return status;
	}

	Status getI32 (GMSEC_I32 &out) const
	{
		Status status;
		out = value;
		return status;
	}

	Status getF64 (GMSEC_F64 &out) const
	{
		Status status;
		out = value;
		return status;
	}

	Status getString (string &out) const
	{
		Status status;
		char buffer[32];
		stringFormat(buffer, sizeof(buffer), "%d", value);
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
	BooleanValue (bool x) : value(x) { }

	bool isBoolean() const {
		return true;
	}

	Status getBoolean (bool &out) const
	{
		Status status;
		out = value;
		return status;
	}

	Status getI32 (GMSEC_I32 &out) const
	{
		Status status;
		if (value)
			out = 1;
		else
			out = 0;
		return status;
	}

	Status getF64 (GMSEC_F64 &out) const
	{
		Status status;
		if (value)
			out = 1;
		else
			out = 0;
		return status;
	}

	Status getString (string &out) const
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
	StringValue (const string &x) : value(x) { }

	bool isString() const {
		return true;
	}

	Status getBoolean (bool &out) const
	{
		Status status;
		out = stringIsTrue(value.c_str());
		return status;
	}

	Status getI32 (GMSEC_I32 &out) const
	{
		Status status;
		if (!stringParseI32(value.c_str(), out))
			status.Set(GMSEC_STATUS_FIELD_ERROR,
				GMSEC_FIELD_TYPE_MISMATCH,
				"Invalid I32 string");
		return status;
	}

	Status getF64 (GMSEC_F64 &out) const
	{
		Status status;
		if (!stringParseF64(value.c_str(), out))
			status.Set(GMSEC_STATUS_FIELD_ERROR,
				GMSEC_FIELD_TYPE_MISMATCH,
				"Invalid F64 string");
		return status;
	}

	Status getString (string &out) const
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
	string value;
};


class F64Value : public Value
{
public:
	F64Value (GMSEC_F64 x) : value(x) { }

	bool isReal() const {
		return true;
	}

	Status getBoolean (bool &out) const
	{
		Status status;
		out = value != 0;
		return status;
	}

	Status getI32 (GMSEC_I32 &out) const
	{
		Status status;
		out = (GMSEC_I32) value;
		if (out != value)
		{
			status.Set(GMSEC_STATUS_FIELD_ERROR,
				GMSEC_FIELD_TYPE_MISMATCH,
				"getI32: truncated F64Value");
		}
		return status;
	}

	Status getF64 (GMSEC_F64 &out) const
	{
		Status status;
		out = value;
		return status;
	}

	Status getString (string &out) const
	{
		Status status;
		char buffer[32];
		stringFormat(buffer, sizeof(buffer), "%d", value);
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
		delete i->second;
}


bool ValueMap::contains (const char * key) const
{
	return values.find(key) != values.end();
}

size_t GMSEC_API ValueMap::size() const
{
	return values.size();
}


Status ValueMap::getBoolean (const char * key, bool &value, bool * pdefault) const
{
	Status status;

	MapType::const_iterator i = values.find(key);
	if (i == values.end())
	{
		if (pdefault)
			value = *pdefault;
		else
			status.Set(GMSEC_STATUS_CONFIG_ERROR,
				GMSEC_INVALID_CONFIG_NAME,
				"getBoolean: missing");
	}
	else
		status = i->second->getBoolean(value);

	return status;
}


Status ValueMap::getI32 (const char * key, GMSEC_I32 &value, GMSEC_I32 * pdefault) const
{
	Status status;

	MapType::const_iterator i = values.find(key);
	if (i == values.end())
	{
		if (pdefault)
			value = *pdefault;
		else
			status.Set(GMSEC_STATUS_CONFIG_ERROR,
				GMSEC_INVALID_CONFIG_NAME,
				"getI32: missing");
	}
	else
		status = i->second->getI32(value);

	return status;
}


Status ValueMap::getF64 (const char * key, GMSEC_F64 &value, GMSEC_F64 * pdefault) const
{
	Status status;

	MapType::const_iterator i = values.find(key);
	if (i == values.end())
	{
		if (pdefault)
			value = *pdefault;
		else
			status.Set(GMSEC_STATUS_CONFIG_ERROR,
				GMSEC_INVALID_CONFIG_NAME,
				"getF64: missing");
	}
	else
		status = i->second->getF64(value);

	return status;
}


Status ValueMap::getString (const char * key, string &value, string * pdefault) const
{
	Status status;

	MapType::const_iterator i = values.find(key);
	if (i == values.end())
	{
		if (pdefault)
			value = *pdefault;
		else
			status.Set(GMSEC_STATUS_CONFIG_ERROR,
				GMSEC_INVALID_CONFIG_NAME,
				"getString: missing");
	}
	else
		status = i->second->getString(value);

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


void ValueMap::setBoolean (const char * key, bool value)
{
	delete values[key];
	values[key] = new BooleanValue(value);
}


void ValueMap::setI32 (const char * key, GMSEC_I32 value)
{
	delete values[key];
	values[key] = new I32Value(value);
}


void ValueMap::setF64 (const char * key, GMSEC_F64 value)
{
	delete values[key];
	values[key] = new F64Value(value);
}


void ValueMap::setString (const char * key, const string &value)
{
	delete values[key];
	values[key] = new StringValue(value);
}


void ValueMap::setOpaqueValue(const char* key, Value* value)
{
	delete values[key];
	values[key] = value;
}


void ValueMap::reset(ValueMap::Iterator &i) const
{
	i.pmap = &values;
	i.current = values.begin();
	i.id.clear();
}


ValueMap::Iterator::Iterator()
	:
	pmap(0)
{
}


ValueMap::Iterator::Iterator(ValueMap::MapType &vm)
	:
	pmap(&vm),
	current(vm.begin())
{
}


ValueMap::Iterator::~Iterator()
{
}


const Value *ValueMap::Iterator::next()
{
	if (!pmap)
		return 0;
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
		delete i->second;

	// Clone 'from' values into our current map.
	std::transform(from.values.begin(), from.values.end(), std::inserter(values, values.begin()), Clone<MapType::value_type>());
}


Status transferHeaderFields(Message &from, ValueMap &to)
{
	FieldIterator i(&selectHeaderFields);
	Field field;
	Status status = from.Reset(i);

	while (!status.isError() && i.HasNext())
	{
		status = i.Next(field);
		if (!status.isError())
		{
			GMSEC_STR name = 0;
			field.GetName(name);
			if (!name)
			{
				status.Set(GMSEC_STATUS_FIELD_ERROR,
						GMSEC_INVALID_FIELD_NAME,
						"Null name");
				continue;
			}

			GMSEC_TYPE type = GMSEC_TYPE_UNSET;
			field.GetType(type);

			switch (type)
			{
			case GMSEC_TYPE_BOOL: {
				GMSEC_BOOL x = GMSEC_FALSE;
				field.GetValue(x);
				to.setBoolean(name, x == GMSEC_TRUE);
				break;
			}
			case GMSEC_TYPE_CHAR: {
				GMSEC_CHAR x = 0;
				field.GetValue(x);
				to.setI32(name, x);
				break;
			}
			case GMSEC_TYPE_I8: {
				GMSEC_I8 x = 0;
				field.GetValue(x);
				to.setI32(name, x);
				break;
			}
			case GMSEC_TYPE_U8: {
				GMSEC_U8 x = 0;
				field.GetValue(x);
				to.setI32(name, x);
				break;
			}
			case GMSEC_TYPE_I16: {
				GMSEC_I16 x = 0;
				field.GetValue(x);
				to.setI32(name, x);
				break;
			}
			case GMSEC_TYPE_U16: {
				GMSEC_U16 x = 0;
				field.GetValue(x);
				to.setI32(name, x);
				break;
			}
			case GMSEC_TYPE_I32: {
				GMSEC_I32 x = 0;
				field.GetValue(x);
				to.setI32(name, x);
				break;
			}
			case GMSEC_TYPE_STR: {
				GMSEC_STR x = 0;
				field.GetValue(x);
				to.setString(name, x);
				break;
			}
			case GMSEC_TYPE_F64: {
				GMSEC_F64 x = 0;
				field.GetValue(x);
				to.setF64(name, x);
				break;
			}
			case GMSEC_TYPE_F32: {
				GMSEC_F32 x = 0;
				field.GetValue(x);
				to.setF64(name, x);
				break;
			}
			default:
				status.Set(GMSEC_STATUS_FIELD_ERROR,
						GMSEC_INVALID_FIELD_VALUE,
						"Null name");
				break;
			}
		}
	}

	return status;
}

}  // end namespace internal
}  // end namespace gmsec
