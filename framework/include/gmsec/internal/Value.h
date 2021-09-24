/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file gmsec/internal/Value.h
*/

#ifndef gmsec_internal_Value_h
#define gmsec_internal_Value_h


#include <gmsec/Status.h>
#include <gmsec/util/wdllexp.h>

#include <map>
#include <string>


namespace gmsec
{
	// forward declaration(s)
	class Message;

namespace internal
{

class GMSEC_API Value
{
public:

#if 0
	// Activate this block if Values are to be used outside of ValueMap
	(for example, by Field)
	static Value * createString (const char * value);
	static Value * createBoolean (bool value);
	static Value * createI32 (int value);
	static Value * createF64 (double value);
#endif

	virtual ~Value ();

	virtual bool isInteger() const { return false; }

	virtual bool isString() const { return false; }

	virtual bool isBoolean() const { return false; }

	virtual bool isReal() const { return false; }

	virtual bool isOpaque() const { return false; }

	virtual Status getBoolean (bool &value) const = 0;
	virtual Status getI32 (GMSEC_I32 &value) const = 0;
	virtual Status getF64 (GMSEC_F64 &value) const = 0;
	virtual Status getString (std::string &value) const = 0;

	virtual Value* clone() const = 0;
};


class GMSEC_API ValueMap
{
public:

	ValueMap();
	~ValueMap();

	bool contains(const char* key) const;
	bool contains(const std::string& key) const;

	size_t size() const;

	Status getBoolean(const char* key, bool& value, bool* pdefault = 0) const;
	Status getI32(const char* key, GMSEC_I32& value, GMSEC_I32* pdefault = 0) const;
	Status getF64(const char* key, GMSEC_F64& value, GMSEC_F64* pdefault = 0) const;
	Status getString(const char* key, std::string& value, std::string* pdefault = 0) const;
	Value* getOpaqueValue(const char* key);

	void setBoolean(const char* key, bool value);
	void setI32(const char* key, GMSEC_I32 value);
	void setF64(const char* key, GMSEC_F64 value);
	void setString(const char* key, const std::string& value);
	void setOpaqueValue(const char * key, Value* value);

	typedef std::map<std::string, Value *> MapType;


	class GMSEC_API Iterator
	{
	public:
		Iterator();
		Iterator(MapType &vm);
		~Iterator();

		const Value *next();
		std::string getID() const {
			return id;
		}

	private:

		friend class ValueMap;

		const MapType *pmap;
		MapType::const_iterator current;
		std::string id;
	};

	void reset(Iterator &i) const;

	void cloneValues(const ValueMap& from);

private:
	// Declared, but not implemented.
	ValueMap(const ValueMap &);
	ValueMap &operator=(const ValueMap &);

	MapType values;
};


namespace tag
{

const char ID[]         = "ID";
const char TOPIC[]      = "TOPIC";
const char REPLY_TO[]   = "REPLY_TO";
const char CORR_ID[]    = "CORR_ID";

} // namespace tag


GMSEC_API Status CALL_TYPE transferHeaderFields(gmsec::Message &from, ValueMap &to);


} // namespace internal
} // namespace gmsec

#endif /* gmsec_internal_Value_h */

