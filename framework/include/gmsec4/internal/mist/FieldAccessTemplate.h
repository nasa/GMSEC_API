#ifndef GMSEC_MIST_FIELD_ACCESS_TEMPLATE_H
#define GMSEC_MIST_FIELD_ACCESS_TEMPLATE_H

#include <gmsec4/Exception.h>
#include <gmsec4/Fields.h>

#include <gmsec4/internal/field/InternalField.h>

#include <gmsec4/internal/StringUtil.h>

#include <sstream>

#include <cstring>


namespace gmsec
{
namespace api
{
namespace mist
{
namespace internal
{

template <typename T> class FieldAccessTemplate
{
public:
	FieldAccessTemplate(const std::string& fieldName, T value, size_t size = 0) : m_field(0)
	{
		throw Exception(MIST_ERROR, UNINITIALIZED_OBJECT,
			"Generic FieldAccessTemplate constructor should never be called");
	}

	FieldAccessTemplate(const Field& field) : m_field(0)
	{
		throw Exception(MIST_ERROR, UNINITIALIZED_OBJECT,
			"Generic FieldAccessTemplate copy-constructor should never be called");
	}

	~FieldAccessTemplate()
	{
		delete m_field;
	}

	void getValue(T& value, size_t& size) const
	{
		throw Exception(MIST_ERROR, UNINITIALIZED_OBJECT,
			"Generic FieldAccessTemplate getValue method should never be called");
	}

	Field& getField() const
	{
		throw Exception(MIST_ERROR, UNINITIALIZED_OBJECT,
			"Generic FieldAccessTemplate getField method should never be called");
	}

private:
	Field* m_field;
};


template <> class FieldAccessTemplate<GMSEC_CHAR>
{
public:
	FieldAccessTemplate(const Field& field) : m_field(gmsec::api::internal::InternalField::makeFieldCopy(field)) {}

	FieldAccessTemplate(const std::string& fieldName, GMSEC_CHAR value, size_t size = 0)
		: m_field(new CharField(fieldName.c_str(), value))
	{
	}

	~FieldAccessTemplate() { delete m_field; }

	void getValue(GMSEC_CHAR& value, size_t& size) const
	{
		const CharField* field = dynamic_cast<const CharField*>(m_field);
		if (!field)
		{
			std::ostringstream oss;

			oss << "FieldAccessTemplate encountered issues converting Field " << m_field->getName() << " to CharField";

			throw Exception(MIST_ERROR, FIELD_TYPE_MISMATCH, oss.str().c_str());
		}
		value = field->getValue();
		size  = sizeof(GMSEC_CHAR);
	}

	Field& getField() const { return *m_field; }

private:
	Field* m_field;
};


template <> class FieldAccessTemplate<GMSEC_BOOL>
{
public:
	FieldAccessTemplate(const Field& field) : m_field(gmsec::api::internal::InternalField::makeFieldCopy(field)) {}

	FieldAccessTemplate(const std::string& fieldName, GMSEC_BOOL value, size_t size = 0)
		: m_field(new BooleanField(fieldName.c_str(), (value ? true : false)))
	{
	}

	~FieldAccessTemplate() { delete m_field; }

	void getValue(GMSEC_BOOL& value, size_t& size) const
	{
		const BooleanField* field = dynamic_cast<const BooleanField*>(m_field);
		if (!field)
		{
			std::ostringstream oss;

			oss << "FieldAccessTemplate encountered issues converting Field " << m_field->getName() << " to BooleanField";

			throw Exception(MIST_ERROR, FIELD_TYPE_MISMATCH, oss.str().c_str());
		}
		value = (field->getValue() ? GMSEC_TRUE : GMSEC_FALSE);
		size  = sizeof(GMSEC_BOOL);
	}

	Field& getField() const { return *m_field; }

private:
	Field* m_field;
};


template <> class FieldAccessTemplate<GMSEC_I8>
{
public:
	FieldAccessTemplate(const Field& field) : m_field(gmsec::api::internal::InternalField::makeFieldCopy(field)) {}

	FieldAccessTemplate(const std::string& fieldName, GMSEC_I8 value, size_t size = 0)
		: m_field(new I8Field(fieldName.c_str(), value))
	{
	}

	~FieldAccessTemplate() { delete m_field; }

	void getValue(GMSEC_I8& value, size_t& size) const
	{
		const I8Field* field = dynamic_cast<const I8Field*>(m_field);
		if (!field)
		{
			std::ostringstream oss;

			oss << "FieldAccessTemplate encountered issues converting Field " << m_field->getName() << " to I8Field";

			throw Exception(MIST_ERROR, FIELD_TYPE_MISMATCH, oss.str().c_str());
		}
		value = field->getValue();
		size  = sizeof(GMSEC_I8);
	}

	Field& getField() const { return *m_field; }

private:
	Field* m_field;
};


template <> class FieldAccessTemplate<GMSEC_I16>
{
public:
	FieldAccessTemplate(const Field& field) : m_field(gmsec::api::internal::InternalField::makeFieldCopy(field)) {}

	FieldAccessTemplate(const std::string& fieldName, GMSEC_I16 value, size_t size = 0)
		: m_field(new I16Field(fieldName.c_str(), value))
	{
	}

	~FieldAccessTemplate() { delete m_field; }

	void getValue(GMSEC_I16& value, size_t& size) const
	{
		const I16Field* field = dynamic_cast<const I16Field*>(m_field);
		if (!field)
		{
			std::ostringstream oss;

			oss << "FieldAccessTemplate encountered issues converting Field " << m_field->getName() << " to I16Field";

			throw Exception(MIST_ERROR, FIELD_TYPE_MISMATCH, oss.str().c_str());
		}
		value = field->getValue();
		size  = sizeof(GMSEC_I16);
	}

	Field& getField() const { return *m_field; }

private:
	Field* m_field;
};


template <> class FieldAccessTemplate<GMSEC_U16>
{
public:
	FieldAccessTemplate(const Field& field) : m_field(gmsec::api::internal::InternalField::makeFieldCopy(field)) {}

	FieldAccessTemplate(const std::string& fieldName, GMSEC_U16 value, size_t size = 0)
		: m_field(new U16Field(fieldName.c_str(), value))
	{
	}

	~FieldAccessTemplate() { delete m_field; }

	void getValue(GMSEC_U16& value, size_t& size) const
	{
		const U16Field* field = dynamic_cast<const U16Field*>(m_field);
		if (!field)
		{
			std::ostringstream oss;

			oss << "FieldAccessTemplate encountered issues converting Field " << m_field->getName() << " to U16Field";

			throw Exception(MIST_ERROR, FIELD_TYPE_MISMATCH, oss.str().c_str());
		}
		value = field->getValue();
		size  = sizeof(GMSEC_U16);
	}

	Field& getField() const { return *m_field; }

private:
	Field* m_field;
};


template <> class FieldAccessTemplate<GMSEC_I32>
{
public:
	FieldAccessTemplate(const Field& field) : m_field(gmsec::api::internal::InternalField::makeFieldCopy(field)) {}

	FieldAccessTemplate(const std::string& fieldName, GMSEC_I32 value, size_t size = 0)
		: m_field(new I32Field(fieldName.c_str(), value))
	{
	}

	~FieldAccessTemplate() { delete m_field; }

	void getValue(GMSEC_I32& value, size_t& size) const
	{
		const I32Field* field = dynamic_cast<const I32Field*>(m_field);
		if (!field)
		{
			std::ostringstream oss;

			oss << "FieldAccessTemplate encountered issues converting Field " << m_field->getName() << " to I32Field";

			throw Exception(MIST_ERROR, FIELD_TYPE_MISMATCH, oss.str().c_str());
		}
		value = field->getValue();
		size  = sizeof(GMSEC_I32);
	}

	Field& getField() const { return *m_field; }

private:
	Field* m_field;
};


template <> class FieldAccessTemplate<GMSEC_U32>
{
public:
	FieldAccessTemplate(const Field& field) : m_field(gmsec::api::internal::InternalField::makeFieldCopy(field)) {}

	FieldAccessTemplate(const std::string& fieldName, GMSEC_U32 value, size_t size = 0)
		: m_field(new U32Field(fieldName.c_str(), value))
	{
	}

	~FieldAccessTemplate() { delete m_field; }

	void getValue(GMSEC_U32& value, size_t& size) const
	{
		const U32Field* field = dynamic_cast<const U32Field*>(m_field);
		if (!field)
		{
			std::ostringstream oss;

			oss << "FieldAccessTemplate encountered issues converting Field " << m_field->getName() << " to U32Field";

			throw Exception(MIST_ERROR, FIELD_TYPE_MISMATCH, oss.str().c_str());
		}
		value = field->getValue();
		size  = sizeof(GMSEC_U32);
	}

	Field& getField() const { return *m_field; }

private:
	Field* m_field;
};


template <> class FieldAccessTemplate<GMSEC_F32>
{
public:
	FieldAccessTemplate(const Field& field) : m_field(gmsec::api::internal::InternalField::makeFieldCopy(field)) {}

	FieldAccessTemplate(const std::string& fieldName, GMSEC_F32 value, size_t size = 0)
		: m_field(new F32Field(fieldName.c_str(), value))
	{
	}

	~FieldAccessTemplate() { delete m_field; }

	void getValue(GMSEC_F32& value, size_t& size) const
	{
		const F32Field* field = dynamic_cast<const F32Field*>(m_field);
		if (!field)
		{
			std::ostringstream oss;

			oss << "FieldAccessTemplate encountered issues converting Field " << m_field->getName() << " to F32Field";

			throw Exception(MIST_ERROR, FIELD_TYPE_MISMATCH, oss.str().c_str());
		}
		value = field->getValue();
		size  = sizeof(GMSEC_F32);
	}

	Field& getField() const { return *m_field; }

private:
	Field* m_field;
};


template <> class FieldAccessTemplate<GMSEC_F64>
{
public:
	FieldAccessTemplate(const Field& field) : m_field(gmsec::api::internal::InternalField::makeFieldCopy(field)) {}

	FieldAccessTemplate(const std::string& fieldName, GMSEC_F64 value, size_t size = 0)
		: m_field(new F64Field(fieldName.c_str(), value))
	{
	}

	~FieldAccessTemplate() { delete m_field; }

	void getValue(GMSEC_F64& value, size_t& size) const
	{
		const F64Field* field = dynamic_cast<const F64Field*>(m_field);
		if (!field)
		{
			std::ostringstream oss;

			oss << "FieldAccessTemplate encountered issues converting Field " << m_field->getName() << " to F64Field";

			throw Exception(MIST_ERROR, FIELD_TYPE_MISMATCH, oss.str().c_str());
		}
		value = field->getValue();
		size  = sizeof(GMSEC_F64);
	}

	Field& getField() const { return *m_field; }

private:
	Field* m_field;
};


template <> class FieldAccessTemplate<char*>
{
public:
	FieldAccessTemplate(const Field& field) : m_field(gmsec::api::internal::InternalField::makeFieldCopy(field)) {}

	FieldAccessTemplate(const std::string& fieldName, char* value, size_t size = 0)
		: m_field(new StringField(fieldName.c_str(), const_cast<const char*>(value)))
	{
	}

	~FieldAccessTemplate() { delete m_field; }

	void getValue(char*& value, size_t& size) const
	{
		const StringField* field = dynamic_cast<const StringField*>(m_field);
		if (!field)
		{
			std::ostringstream oss;

			oss << "FieldAccessTemplate encountered issues converting Field " << m_field->getName() << " to StringField";

			throw Exception(MIST_ERROR, FIELD_TYPE_MISMATCH, oss.str().c_str());
		}
		value = const_cast<char*>(field->getValue());
		size  = gmsec::api::util::StringUtil::stringLength(value);
	}

	Field& getField() const { return *m_field; }

private:
	Field* m_field;
};


template <> class FieldAccessTemplate<GMSEC_BIN>
{
public:
	FieldAccessTemplate(const Field& field) : m_field(gmsec::api::internal::InternalField::makeFieldCopy(field)) {}

	FieldAccessTemplate(const std::string& fieldName, GMSEC_BIN value, size_t size = 0)
		: m_field(new BinaryField(fieldName.c_str(), value, size))
	{
	}

	~FieldAccessTemplate() { delete m_field; }

	void getValue(GMSEC_BIN& value, size_t& size) const
	{
		const BinaryField* field = dynamic_cast<const BinaryField*>(m_field);

		if (!field)
		{
			size = 0;

			switch (m_field->getType())
			{

			  case Field::BOOL_TYPE:
			  {
				const BooleanField* bool_field =
			  	dynamic_cast<const BooleanField*>(m_field);

				if (bool_field)
				{
					bool bool_value = bool_field->getValue();
					size = sizeof(bool);
					value = new unsigned char[size];
					std::memcpy(value, &bool_value, size);
				}
			  }
			  break;
 
			  case Field::CHAR_TYPE:
			  {
				const CharField* char_field =
				  dynamic_cast<const CharField*>(m_field);

				if (char_field)
				{
					GMSEC_CHAR char_value = char_field->getValue();
					size = sizeof(GMSEC_CHAR);
					value = new unsigned char[size];
					std::memcpy(value, &char_value, size);
				}
			  }
			  break;
 
			  case Field::I8_TYPE:
			  {
				const I8Field* i8_field =
				  dynamic_cast<const I8Field*>(m_field);

				if (i8_field)
				{
					GMSEC_I8 i8_value = i8_field->getValue();
					size = sizeof(GMSEC_I8);
					value = new unsigned char[size];
					std::memcpy(value, &i8_value, size);
				}
			  }
			  break;
 
			  case Field::I16_TYPE:
			  {
				const I16Field* i16_field =
				  dynamic_cast<const I16Field*>(m_field);

				if (i16_field)
				{
					GMSEC_I16 i16_value = i16_field->getValue();
					size = sizeof(GMSEC_I16);
					value = new unsigned char[size];
					std::memcpy(value, &i16_value, size);
				}
			  }
			  break;
 
			  case Field::I32_TYPE:
			  {
				const I32Field* i32_field =
				  dynamic_cast<const I32Field*>(m_field);

				if (i32_field)
				{
					GMSEC_I32 i32_value = i32_field->getValue();
					size = sizeof(GMSEC_I32);
					value = new unsigned char[size];
					std::memcpy(value, &i32_value, size);
				}
			  }
			  break;
 
			  case Field::I64_TYPE:
			  {
				const I64Field* i64_field =
				  dynamic_cast<const I64Field*>(m_field);

				if (i64_field)
				{
					GMSEC_I64 i64_value = i64_field->getValue();
					size = sizeof(GMSEC_I64);
					value = new unsigned char[size];
					std::memcpy(value, &i64_value, size);
				}
			  }
			  break;
 
			  case Field::F32_TYPE:
			  {
				const F32Field* f32_field =
				  dynamic_cast<const F32Field*>(m_field);

				if (f32_field)
				{
					GMSEC_F32 f32_value = f32_field->getValue();
					size = sizeof(GMSEC_F32);
					value = new unsigned char[size];
					std::memcpy(value, &f32_value, size);
				}
			  }
			  break;
 
			  case Field::F64_TYPE:
			  {
				const F64Field* f64_field =
				  dynamic_cast<const F64Field*>(m_field);

				if (f64_field)
				{
					GMSEC_F64 f64_value = f64_field->getValue();
					size = sizeof(GMSEC_F64);
					value = new unsigned char[size];
					std::memcpy(value, &f64_value, size);
				}
			  }
			  break;
 
			  case Field::STRING_TYPE:
			  {
				const StringField* string_field =
				  dynamic_cast<const StringField*>(m_field);

				if (string_field)
				{
					const char* string_value = string_field->getValue();
					size = gmsec::api::util::StringUtil::stringLength(string_value);
					value = new unsigned char[size];
					std::memcpy(value, string_value, size);
				}
			  }
			  break;
 
			  case Field::U8_TYPE:
			  {
				const U8Field* u8_field =
				  dynamic_cast<const U8Field*>(m_field);

				if (u8_field)
				{
					GMSEC_U8 u8_value = u8_field->getValue();
					size = sizeof(GMSEC_U8);
					value = new unsigned char[size];
					std::memcpy(value, &u8_value, size);
				}
			  }
			  break;
 
			  case Field::U16_TYPE:
			  {
				const U16Field* u16_field =
				  dynamic_cast<const U16Field*>(m_field);

				if (u16_field)
				{
					GMSEC_U16 u16_value = u16_field->getValue();
					size = sizeof(GMSEC_U16);
					value = new unsigned char[size];
					std::memcpy(value, &u16_value, size);
				}
			  }
			  break;
 
			  case Field::U32_TYPE:
			  {
				const U32Field* u32_field =
				  dynamic_cast<const U32Field*>(m_field);

				if (u32_field)
				{
					GMSEC_U32 u32_value = u32_field->getValue();
					size = sizeof(GMSEC_U32);
					value = new unsigned char[size];
					std::memcpy(value, &u32_value, size);
				}
			  }
			  break;
 
			  case Field::U64_TYPE:
			  {
				const U64Field* u64_field =
				  dynamic_cast<const U64Field*>(m_field);

				if (u64_field)
				{
					GMSEC_U64 u64_value = u64_field->getValue();
					size = sizeof(GMSEC_U64);
					value = new unsigned char[size];
					std::memcpy(value, &u64_value, size);
				}
			  }
			  break;

			  default:
			    break;

			}

			if (size == 0)
			{
				std::ostringstream oss;

				oss << "FieldAccessTemplate encountered issues converting Field " << m_field->getName() << " to BinaryField";

				throw Exception(MIST_ERROR, FIELD_TYPE_MISMATCH, oss.str().c_str());
			}

		}
		else
		{
			value = field->getValue();
			size  = field->getLength();
		}
	}

	Field& getField() const { return *m_field; }

private:
	Field* m_field;
};


}  //namespace internal
}  //namespace mist
}  //namespace api
}  //namespace gmsec

#endif
