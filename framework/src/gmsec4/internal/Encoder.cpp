/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#define GMSEC_WARN_MISSING_LLONG 1

#include <gmsec4/internal/Encoder.h>

#include <gmsec4/internal/MessageBuddy.h>
#include <gmsec4/internal/StringUtil.h>

#include <gmsec4/Errors.h>
#include <gmsec4/Exception.h>
#include <gmsec4/Fields.h>

#include <gmsec4/util/Log.h>
#include <gmsec4/util/Mutex.h>

#include <cstring>  // for memcpy


using namespace gmsec::api;
using namespace gmsec::api::internal;
using namespace gmsec::api::util;


namespace gmsec {
namespace api {
namespace internal {


static void setError(const char* text)
{
	throw Exception(MSG_ERROR, ENCODING_ERROR, text);
}


Encoder* Encoder::getEncoder()
{
	static Encoder* encoder = 0;
	if (!encoder)
	{
		AutoMutex hold(StringUtil::getSharedMutex());
		if (!encoder)
		{
			encoder = new Encoder;
		}
	}
	return encoder;
}


Decoder* Decoder::getDecoder()
{
	static Decoder* decoder = 0;
	if (!decoder)
	{
		AutoMutex hold(StringUtil::getSharedMutex());
		if (!decoder)
		{
			decoder = new Decoder;
		}
	}
	return decoder;
}


static void put1ByteI8(const GMSEC_I8 * pi, char *&po)
{
	po[0] = pi[0];
	po += 1;
}


static void put1ByteU8(const GMSEC_U8 * pi, char *&po)
{
	po[0] = pi[0];
	po += 1;
}


/* I16 encoding */
static inline void put2BytesBigEndian(const char * pi, char *&po)
{
	po[0] = pi[0];
	po[1] = pi[1];
	po += 2;
}


static inline void put2BytesLittleEndian(const char * pi, char *&po)
{
	po[1] = pi[0];
	po[0] = pi[1];
	po += 2;
}


static void put2ByteI16BigEndian(const GMSEC_I16 * pi, char *&po)
{
	const char * pc = (const char *) pi;
	put2BytesBigEndian(pc, po);
}


static void put2ByteI16LittleEndian(const GMSEC_I16 * pi, char *&po)
{
	const char * pc = (const char *) pi;
	put2BytesLittleEndian(pc, po);
}


static void put2ByteU16BigEndian(const GMSEC_U16 * pi, char *&po)
{
	const char * pc = (const char *) pi;
	put2BytesBigEndian(pc, po);
}


static void put2ByteU16LittleEndian(const GMSEC_U16 * pi, char *&po)
{
	const char * pc = (const char *) pi;
	put2BytesLittleEndian(pc, po);
}


static void put4ByteI16BigEndian(const GMSEC_I16 * pi, char *&po)
{
	const char * pc = (const char *) pi;
	/* constrain value? */
	put2BytesBigEndian(pc + 2, po);
}


static void put4ByteI16LittleEndian(const GMSEC_I16 * pi, char *&po)
{
	const char * pc = (const char *) pi;
	/* constrain value? */
	put2BytesLittleEndian(pc, po);
}


static void put4ByteU16BigEndian(const GMSEC_U16 * pi, char *&po)
{
	const char * pc = (const char *) pi;
	/* constrain value? */
	put2BytesBigEndian(pc + 2, po);
}


static void put4ByteU16LittleEndian(const GMSEC_U16 * pi, char *&po)
{
	const char * pc = (const char *) pi;
	/* constrain value? */
	put2BytesLittleEndian(pc, po);
}


/* 32 bit value encoding */
static inline void put4BytesBigEndian(const char * pi, char *&po)
{
	po[0] = pi[0];
	po[1] = pi[1];
	po[2] = pi[2];
	po[3] = pi[3];
	po += 4;
}

static inline void put4BytesLittleEndian(const char * pi, char *&po)
{
	po[3] = pi[0];
	po[2] = pi[1];
	po[1] = pi[2];
	po[0] = pi[3];
	po += 4;
}


static void put4ByteI32BigEndian(const GMSEC_I32 * pi, char *&po)
{
	const char * pc = (const char *) pi;
	put4BytesBigEndian(pc, po);
}


static void put4ByteI32LittleEndian(const GMSEC_I32 * pi, char *&po)
{
	const char * pc = (const char *) pi;
	put4BytesLittleEndian(pc, po);
}


static void put4ByteU32BigEndian(const GMSEC_U32 * pi, char *&po)
{
	const char * pc = (const char *) pi;
	put4BytesBigEndian(pc, po);
}


static void put4ByteU32LittleEndian(const GMSEC_U32 * pi, char *&po)
{
	const char * pc = (const char *) pi;
	put4BytesLittleEndian(pc, po);
}


static void put8ByteI32BigEndian(const GMSEC_I32 * pi, char *&po)
{
	const char * pc = (const char *) pi;
	/* constrain value? */
	put4BytesBigEndian(pc + 4, po);
}


static void put8ByteI32LittleEndian(const GMSEC_I32 * pi, char *&po)
{
	const char * pc = (const char *) pi;
	/* constrain value? */
	put4BytesLittleEndian(pc, po);
}


static void put8ByteU32BigEndian(const GMSEC_U32 * pi, char *&po)
{
	const char * pc = (const char *) pi;
	/* constrain value? */
	put4BytesBigEndian(pc + 4, po);
}


static void put8ByteU32LittleEndian(const GMSEC_U32 * pi, char *&po)
{
	const char * pc = (const char *) pi;
	/* constrain value? */
	put4BytesLittleEndian(pc, po);
}


/* 64 bit value encoding */
static inline void put8BytesBigEndian(const char * pi, char *&po)
{
	po[0] = pi[0];
	po[1] = pi[1];
	po[2] = pi[2];
	po[3] = pi[3];
	po[4] = pi[4];
	po[5] = pi[5];
	po[6] = pi[6];
	po[7] = pi[7];
	po += 8;
}


static inline void put8BytesLittleEndian(const char * pi, char *&po)
{
	po[7] = pi[0];
	po[6] = pi[1];
	po[5] = pi[2];
	po[4] = pi[3];
	po[3] = pi[4];
	po[2] = pi[5];
	po[1] = pi[6];
	po[0] = pi[7];
	po += 8;
}


static void put8ByteI64BigEndian(const GMSEC_I64 * pi, char *&po)
{
	const char * pc = (const char *) pi;
	put8BytesBigEndian(pc, po);
}



static void put8ByteI64LittleEndian(const GMSEC_I64 * pi, char *&po)
{
	const char * pc = (const char *) pi;
	put8BytesLittleEndian(pc, po);
}


static void put8ByteU64BigEndian(const GMSEC_U64 * pi, char *&po)
{
	const char * pc = (const char *) pi;
	put8BytesBigEndian(pc, po);
}


static void put8ByteU64LittleEndian(const GMSEC_U64 * pi, char *&po)
{
	const char * pc = (const char *) pi;
	put8BytesLittleEndian(pc, po);
}


static void put4ByteF32BigEndian(const GMSEC_F32 * pi, char *&po)
{
	const char * pc = (const char *) pi;
	put4BytesBigEndian(pc, po);
}


static void put4ByteF32LittleEndian(const GMSEC_F32 * pi, char *&po)
{
	const char * pc = (const char *) pi;
	put4BytesLittleEndian(pc, po);
}


static void put8ByteF64BigEndian(const GMSEC_F64 * pi, char *&po)
{
	const char * pc = (const char *) pi;
	put8BytesBigEndian(pc, po);
}


static void put8ByteF64LittleEndian(const GMSEC_F64 * pi, char *&po)
{
	const char * pc = (const char *) pi;
	put8BytesLittleEndian(pc, po);
}


MessageEncoder::MessageEncoder()
	:
	buffer(0),
	current(0),
	encoder(Encoder::getEncoder()),
	selector(MessageFieldIterator::ALL_FIELDS),
	altSelector(0)
{
	if (encoder->getProblem())
	{
		setError(encoder->getProblem());
	}
}


MessageEncoder::~MessageEncoder()
{
}


void MessageEncoder::encode(const Message& message, GMSEC_U64& count, char*& data)
{
	count = 0;
	data  = 0;

	DataBuffer tmp;
	encode(message, tmp);

	// increase the scope of the data from the method to the object
	buffer.swap(tmp);

	count = buffer.size();

	data = reinterpret_cast<char*>(buffer.raw());
}


void MessageEncoder::encode(const Message &message, DataBuffer &out)
{
	size_t length = findLength(message);

	if (out.size() < length && !out.resize(length))
	{
		setError("MessageEncoder::encode : Memory allocation failed.");
	}
	else
	{
		buffer.resize(0);

		// accept the passed in buffer
		buffer.swap(out);

		encode(message, (GMSEC_I32) message.getFieldCount());

		// replace- ought to use RAII
		buffer.swap(out);
	}

	// even if there was an error, do not out.resize(0) since they may want the space.

	// compression / encryption is external
}


void MessageEncoder::setSelector(MessageFieldIterator::Selector s)
{
	selector = s;
}


void MessageEncoder::setSelector(FieldSelector s)
{
	altSelector = s;
}


size_t MessageEncoder::findLength(const Message &message)
{
	size_t length = GMSEC_ENCODING_BYTES;

#ifdef GMSEC_ENCODE_HEADER
	/* the message type */
	length += GMSEC_TYPE_BYTES;

	/* the message subject */
	length += GMSEC_LENGTH_BYTES + StringUtil::stringLength(message.getSubject()) + 1;
#endif

	length += GMSEC_FIELD_COUNT_BYTES;

	MessageFieldIterator& iter = message.getFieldIterator(selector);

	while (iter.hasNext())
	{
		const Field& field = iter.next();

		length += updateLength(field);
	}

	if (length > static_cast<size_t>(GMSEC_ENCODED_LIMIT))
	{
		setError("MessageEncoder::findLength : Encoded content too large");
	}

	return length;
}


void MessageEncoder::encode(const Message &message, GMSEC_I32 fieldCount)
{
	current = reinterpret_cast<char*>(buffer.raw());

	GMSEC_U16 version = 1;
	encoder->putU16(&version, current);

#ifdef GMSEC_ENCODE_HEADER
	/* the message kind */
	Message::MessageKind kind = message.getKind();
	GMSEC_U16 type = kind;
	encoder->putU16(&type, current);

	/* the message subject */
	encoder->putString(message.getSubject(), current);
#endif

	/* the message field count*/
	encoder->putI32(&fieldCount, current);

	/* the message field(s) */
	if (altSelector != NULL)
	{
		MessageFieldIterator& iter = message.getFieldIterator();
		while (iter.hasNext())
		{
			const Field& field = iter.next();

			if (altSelector(field))
			{
				encode(field);
			}
		}
	}
	else
	{
		MessageFieldIterator& iter = message.getFieldIterator(selector);
		while (iter.hasNext())
		{
			const Field& field = iter.next();
			encode(field);
		}
	}
}


/* this uses the same approach as the Message Bus serialization.  I suspect
that the compression could be improved by storing all of the field names
together... */


void MessageEncoder::encode(const Field &field)
{
	Field::FieldType type = field.getType();

	GMSEC_U16 tmp = (GMSEC_U16) type;

	if (field.isHeader())
	{
		tmp |= GMSEC_FLAG_HEADER_FIELD;
	}

	encoder->putU16(&tmp, current);
	encoder->putString(field.getName(), current);

	switch (type)
	{
	case Field::CHAR_TYPE:
	{
		GMSEC_CHAR value = dynamic_cast<const CharField&>(field).getValue();
		encoder->putI8((GMSEC_I8*) &value, current);
		break;
	}
	case Field::BOOL_TYPE:
	{
		bool value = dynamic_cast<const BooleanField&>(field).getValue();
		GMSEC_U8 tmp = (GMSEC_U8)(value ? 1 : 0);
		encoder->putU8(&tmp, current);
		break;
	}
	case Field::I8_TYPE:
	{
		GMSEC_I8 value = dynamic_cast<const I8Field&>(field).getValue();
		encoder->putI8((GMSEC_I8*) &value, current);
		break;
	}
	case Field::U8_TYPE:
	{
		GMSEC_U8 value = dynamic_cast<const U8Field&>(field).getValue();
		encoder->putU8((GMSEC_U8*) &value, current);
		break;
	}
	case Field::I16_TYPE:
	{
		GMSEC_I16 value = dynamic_cast<const I16Field&>(field).getValue();
		encoder->putI16(&value, current);
		break;
	}
	case Field::U16_TYPE:
	{
		GMSEC_U16 value = dynamic_cast<const U16Field&>(field).getValue();
		encoder->putU16(&value, current);
		break;
	}
	case Field::I32_TYPE:
	{
		GMSEC_I32 value = dynamic_cast<const I32Field&>(field).getValue();
		encoder->putI32(&value, current);
		break;
	}
	case Field::U32_TYPE:
	{
		GMSEC_U32 value = dynamic_cast<const U32Field&>(field).getValue();
		encoder->putU32(&value, current);
		break;
	}
	case Field::F32_TYPE:
	{
		GMSEC_F32 value = dynamic_cast<const F32Field&>(field).getValue();
		encoder->putF32(&value, current);
		break;
	}
	case Field::F64_TYPE:
	{
		GMSEC_F64 value = dynamic_cast<const F64Field&>(field).getValue();
		encoder->putF64(&value, current);
		break;
	}
	case Field::STRING_TYPE:
	{
		const char* value = dynamic_cast<const StringField&>(field).getValue();
		encoder->putString(value, current);
		break;
	}
	case Field::BIN_TYPE:
	{
		const BinaryField& binField = dynamic_cast<const BinaryField&>(field);

		GMSEC_BIN value = binField.getValue();
		GMSEC_U32 count = (GMSEC_U32) binField.getLength();

		encoder->putCountedBytes((const char *) value, count, current);
		break;
	}
	case Field::I64_TYPE:
	{
		GMSEC_I64 value = dynamic_cast<const I64Field&>(field).getValue();
		encoder->putI64(&value, current);
		break;
	}
	case Field::U64_TYPE:
	{
		GMSEC_U64 value = dynamic_cast<const U64Field&>(field).getValue();
		encoder->putU64(&value, current);
		break;
	}
	default:
		setError("MessageEncoder::encode : Unexpected field type");
		break;
	}
}


size_t MessageEncoder::updateLength(const Field& field)
{
	size_t length = 0;

	length += GMSEC_TYPE_BYTES;
	length += GMSEC_LENGTH_BYTES + StringUtil::stringLength(field.getName()) + 1;

	switch (field.getType())
	{
	case Field::CHAR_TYPE:
	case Field::I8_TYPE:
	case Field::U8_TYPE:
		length += 1;
		break;
	case Field::BOOL_TYPE:
		length += 1;
		break;
	case Field::I16_TYPE:
	case Field::U16_TYPE:
		length += 2;
		break;
	case Field::I32_TYPE:
	case Field::U32_TYPE:
	case Field::F32_TYPE:
		length += 4;
		break;
	case Field::I64_TYPE:
	case Field::U64_TYPE:
	case Field::F64_TYPE:
		length += 8;
		break;
	case Field::STRING_TYPE:
	{
		const StringField& strField = dynamic_cast<const StringField&>(field);
		const size_t       count    = StringUtil::stringLength(strField.getValue());

		if (count > static_cast<size_t>(GMSEC_STRING_LIMIT))
		{
			setError("MessageEncoder::updateLength : Excessive string length");
		}
		else
		{
			length += GMSEC_LENGTH_BYTES + count + 1;
		}
		break;
	}
	case Field::BIN_TYPE:
	{
		const BinaryField& binField = dynamic_cast<const BinaryField&>(field);
		const GMSEC_U64    count    = binField.getLength();

		if (count > static_cast<GMSEC_U64>(GMSEC_BIN_LIMIT))
		{
			setError("MessageEncoder::updateLength : Excessive binary data");
		}
		else
		{
			length += GMSEC_LENGTH_BYTES + size_t(count);
		}
		break;
	}
	default:
		setError("MessageEncoder::updateLength : Unable to encode field into message");
		break;
	}

	return length;
}


static bool isBigEndian()
{
	int x = 1;
	const char * p = (const char *) &x;
	bool bigEndian = p[0] == 0;
	return bigEndian;
}


static void setError(const char *& problem, const char * text)
{
	if (!problem)
	{
		problem = text;
	}
}


static void (*selectPutI16(const char *& error))(const GMSEC_I16 *, char *&)
{
	if (sizeof(GMSEC_U16) == 2)
	{
		if (isBigEndian())
			return &put2ByteI16BigEndian;
		else
			return &put2ByteI16LittleEndian;
	}
	else if (sizeof(GMSEC_U16) == 4)
	{
		if (isBigEndian())
			return &put4ByteI16BigEndian;
		else
			return &put4ByteI16LittleEndian;
	}
	else
	{
		setError(error, "size of GMSEC_U16 is not 2 or 4");
		GMSEC_WARNING << "MessageEncoder::selectPutI16 : Size of GMSEC_U16 is not 2 or 4.";
	}

	return 0;
}


static void (*selectPutU16(const char *&error))(const GMSEC_U16 *, char *&)
{
	if (sizeof(GMSEC_U16) == 2)
	{
		if (isBigEndian())
			return &put2ByteU16BigEndian;
		else
			return &put2ByteU16LittleEndian;
	}
	else if (sizeof(GMSEC_U16) == 4)
	{
		if (isBigEndian())
			return &put4ByteU16BigEndian;
		else
			return &put4ByteU16LittleEndian;
	}
	else
	{
		setError(error, "size of GMSEC_U16 is not 2 or 4");
		GMSEC_WARNING << "MessageEncoder::selectPutI16 : Size of GMSEC_U16 is not 2 or 4.";
	}

	return 0;
}


static void (*selectPutI32(const char *&error))(const GMSEC_I32 *, char *&)
{
	if (sizeof(GMSEC_U32) == 4)
	{
		if (isBigEndian())
			return &put4ByteI32BigEndian;
		else
			return &put4ByteI32LittleEndian;
	}
	else if (sizeof(GMSEC_U32) == 8)
	{
		if (isBigEndian())
			return &put8ByteI32BigEndian;
		else
			return &put8ByteI32LittleEndian;
	}
	else
	{
		setError(error, "size of GMSEC_U32 is not 4 or 8");
		GMSEC_WARNING << "MessageEncoder::selectPutI32 : Size of GMSEC_U32 is not 4 or 8.";
	}

	return 0;
}


static void (*selectPutU32(const char *&error))(const GMSEC_U32 *, char *&)
{
	if (sizeof(GMSEC_U32) == 4)
	{
		if (isBigEndian())
			return &put4ByteU32BigEndian;
		else
			return &put4ByteU32LittleEndian;
	}
	else if (sizeof(GMSEC_U32) == 8)
	{
		if (isBigEndian())
			return &put8ByteU32BigEndian;
		else
			return &put8ByteU32LittleEndian;
	}
	else
	{
		setError(error, "size of GMSEC_U32 is not 2 or 4");
		GMSEC_WARNING << "MessageEncoder::selectPutU32 : Size of GMSEC_U32 is not 2 or 4.";
	}

	return 0;
}


static void (*selectPutI64(const char *&error))(const GMSEC_I64 *, char *&)
{
	if (sizeof(GMSEC_U64) == 8)
	{
		if (isBigEndian())
			return &put8ByteI64BigEndian;
		else
			return &put8ByteI64LittleEndian;
	}
	else
	{
		setError(error, "size of GMSEC_U64 is not 8");
		GMSEC_WARNING << "MessageEncoder::selectPutI64 : Size of GMSEC_U64 is not 8.";
	}

	return 0;
}


static void (*selectPutU64(const char *&error))(const GMSEC_U64 *, char *&)
{
	if (sizeof(GMSEC_U64) == 8)
	{
		if (isBigEndian())
			return &put8ByteU64BigEndian;
		else
			return &put8ByteU64LittleEndian;
	}
	else
	{
		setError(error, "size of GMSEC_U64 is not 8");
		GMSEC_WARNING << "MessageEncoder::selectPutU64 : Size of GMSEC_U64 is not 8.";
	}

	return 0;
}


static void (*selectPutF32(const char *&error))(const GMSEC_F32 *, char *&)
{
	if (sizeof(GMSEC_U32) == 4)
	{
		if (isBigEndian())
			return &put4ByteF32BigEndian;
		else
			return &put4ByteF32LittleEndian;
	}
	else
	{
		setError(error, "size of GMSEC_F32 is not 4");
		GMSEC_WARNING << "MessageEncoder::selectPutF32 : Size of GMSEC_F32 is not 4.";
	}

	return 0;
}


static void (*selectPutF64(const char *&error))(const GMSEC_F64 *, char *&)
{
	if (sizeof(GMSEC_U64) == 8)
	{
		if (isBigEndian())
			return &put8ByteF64BigEndian;
		else
			return &put8ByteF64LittleEndian;
	}
	else
	{
		setError(error, "size of GMSEC_F64 is not 8");
		GMSEC_WARNING << "MessageEncoder::selectPutF64 : Size of GMSEC_F64 is not 8.";
	}

	return 0;
}


Encoder::Encoder()
		:
		problem(0),
		putI8(&put1ByteI8),
		putI16(selectPutI16(problem)),
		putI32(selectPutI32(problem)),
		putI64(selectPutI64(problem)),
		putU8(&put1ByteU8),
		putU16(selectPutU16(problem)),
		putU32(selectPutU32(problem)),
		putU64(selectPutU64(problem)),
		putF32(selectPutF32(problem)),
		putF64(selectPutF64(problem))
{
}


const char* Encoder::getProblem() const
{
	return problem;
}


void Encoder::putRawBytes(const char *pi, GMSEC_U32 count, char *&po)
{
	memcpy(po, pi, count);
	po += count;
}


void Encoder::putCountedBytes(const char *pi, GMSEC_U32 count, char *&po)
{
	putU32(&count, po);
	putRawBytes(pi, count, po);
}


void Encoder::putString(const char *pi, char *&po)
{
	size_t s = StringUtil::stringLength(pi);
	GMSEC_U32 count = (GMSEC_U32) s + 1;
	putCountedBytes(pi, count, po);
}


GMSEC_U32 Encoder::encodeU32(GMSEC_U32 in)
{
	GMSEC_U32 out = 0;
	char* p = (char*) &out;
	getEncoder()->putU32(&in, p);
	return out;
}


GMSEC_U64 Encoder::encodeU64(GMSEC_U64 in)
{
	GMSEC_U64 out = 0;
	char* p = (char*) &out;
	getEncoder()->putU64(&in, p);
	return out;
}


/* decoding */
static void get1ByteI8(const char *&pi, GMSEC_I8 *po)
{
	po[0] = pi[0];
	pi += 1;
}


static void get1ByteU8(const char *&pi, GMSEC_U8 *po)
{
	po[0] = (GMSEC_U8) pi[0];
	pi += 1;
}


/* I16 decoding */
static inline void get2BytesBigEndian(const char *&pi, char *po)
{
	po[0] = pi[0];
	po[1] = pi[1];
	pi += 2;
}


static inline void get2BytesLittleEndian(const char *&pi, char *po)
{
	po[1] = pi[0];
	po[0] = pi[1];
	pi += 2;
}


static void get2ByteI16BigEndian(const char *&pi, GMSEC_I16 *po)
{
	char *pc = (char *) po;
	get2BytesBigEndian(pi, pc);
}


static void get2ByteI16LittleEndian(const char *&pi, GMSEC_I16 *po)
{
	char *pc = (char *) po;
	get2BytesLittleEndian(pi, pc);
}


static void get2ByteU16BigEndian(const char *&pi, GMSEC_U16 *po)
{
	char *pc = (char *) po;
	get2BytesBigEndian(pi, pc);
}


static void get2ByteU16LittleEndian(const char *&pi, GMSEC_U16 *po)
{
	char *pc = (char *) po;
	get2BytesLittleEndian(pi, pc);
}


static void get4ByteI16BigEndian(const char *&pi, GMSEC_I16 *po)
{
	char *pc = (char *) po;
	*po = 0;
	get2BytesBigEndian(pi, pc + 2);
}


static void get4ByteI16LittleEndian(const char *&pi, GMSEC_I16 *po)
{
	char *pc = (char *) po;
	*po = 0;
	get2BytesLittleEndian(pi, pc);
}


static void get4ByteU16BigEndian(const char *&pi, GMSEC_U16 *po)
{
	char *pc = (char *) po;
	*po = 0;
	get2BytesBigEndian(pi, pc + 2);
}


static void get4ByteU16LittleEndian(const char *&pi, GMSEC_U16 *po)
{
	char *pc = (char *) po;
	*po = 0;
	get2BytesLittleEndian(pi, pc);
}


/* 32 bit value decoding */
static inline void get4BytesBigEndian(const char *&pi, char *po)
{
	po[0] = pi[0];
	po[1] = pi[1];
	po[2] = pi[2];
	po[3] = pi[3];
	pi += 4;
}

static inline void get4BytesLittleEndian(const char *&pi, char *po)
{
	po[3] = pi[0];
	po[2] = pi[1];
	po[1] = pi[2];
	po[0] = pi[3];
	pi += 4;
}


static void get4ByteI32BigEndian(const char *&pi, GMSEC_I32 *po)
{
	char *pc = (char *) po;
	get4BytesBigEndian(pi, pc);
}


static void get4ByteI32LittleEndian(const char *&pi, GMSEC_I32 *po)
{
	char *pc = (char *) po;
	get4BytesLittleEndian(pi, pc);
}


static void get4ByteU32BigEndian(const char *&pi, GMSEC_U32 *po)
{
	char *pc = (char *) po;
	get4BytesBigEndian(pi, pc);
}


static void get4ByteU32LittleEndian(const char *&pi, GMSEC_U32 *po)
{
	char *pc = (char *) po;
	get4BytesLittleEndian(pi, pc);
}


static void get8ByteI32BigEndian(const char *&pi, GMSEC_I32 *po)
{
	char *pc = (char *) po;
	*po = 0;
	get4BytesBigEndian(pi, pc + 4);
}


static void get8ByteI32LittleEndian(const char *&pi, GMSEC_I32 *po)
{
	char *pc = (char *) po;
	*po = 0;
	get4BytesLittleEndian(pi, pc);
}


static void get8ByteU32BigEndian(const char *&pi, GMSEC_U32 *po)
{
	char * pc = (char *) po;
	*po = 0;
	get4BytesBigEndian(pi, pc + 4);
}


static void get8ByteU32LittleEndian(const char *&pi, GMSEC_U32 *po)
{
	char *pc = (char *) po;
	*po = 0;
	get4BytesLittleEndian(pi, pc);
}


/* 64 bit value encoding */
static inline void get8BytesBigEndian(const char *&pi, char *po)
{
	po[0] = pi[0];
	po[1] = pi[1];
	po[2] = pi[2];
	po[3] = pi[3];
	po[4] = pi[4];
	po[5] = pi[5];
	po[6] = pi[6];
	po[7] = pi[7];
	pi += 8;
}


static inline void get8BytesLittleEndian(const char *&pi, char *po)
{
	po[7] = pi[0];
	po[6] = pi[1];
	po[5] = pi[2];
	po[4] = pi[3];
	po[3] = pi[4];
	po[2] = pi[5];
	po[1] = pi[6];
	po[0] = pi[7];
	pi += 8;
}


static void get8ByteI64BigEndian(const char *&pi, GMSEC_I64 *po)
{
	char *pc = (char *) po;
	get8BytesBigEndian(pi, pc);
}



static void get8ByteI64LittleEndian(const char *&pi, GMSEC_I64 *po)
{
	char *pc = (char *) po;
	get8BytesLittleEndian(pi, pc);
}


static void get8ByteU64BigEndian(const char *&pi, GMSEC_U64 *po)
{
	char *pc = (char *) po;
	get8BytesBigEndian(pi, pc);
}


static void get8ByteU64LittleEndian(const char *&pi, GMSEC_U64 *po)
{
	char *pc = (char *) po;
	get8BytesLittleEndian(pi, pc);
}


static void get4ByteF32BigEndian(const char *&pi, GMSEC_F32 *po)
{
	char *pc = (char *) po;
	get4BytesBigEndian(pi, pc);
}


static void get4ByteF32LittleEndian(const char *&pi, GMSEC_F32 *po)
{
	char *pc = (char *) po;
	get4BytesLittleEndian(pi, pc);
}


static void get8ByteF64BigEndian(const char *&pi, GMSEC_F64 *po)
{
	char *pc = (char *) po;
	get8BytesBigEndian(pi, pc);
}


static void get8ByteF64LittleEndian(const char *&pi, GMSEC_F64 *po)
{
	char *pc = (char *) po;
	get8BytesLittleEndian(pi, pc);
}


bool Decoder::getCountedBytes(MessageDecoder &owner,
                              const char *&pi, GMSEC_U32 &count, const char *&po)
{
	if (!owner.checkAvailable(4))
		return false;

	getU32(pi, &count);
	if (count > GMSEC_BIN_LIMIT)
		return false;

	if (!owner.checkAvailable(count))
		return false;

	po = pi;
	pi += count;
	return true;
}


bool Decoder::getString(MessageDecoder &owner,
                        const char *&pi, GMSEC_U32 &count, const char *&po)
{
	if (!owner.checkAvailable(4))
		return false;

	getU32(pi, &count);
	if (count > GMSEC_STRING_LIMIT)
		return false;

	if (count < 1)
		return false;

	if (!owner.checkAvailable(count))
		return false;

	/* check null terminator */
	if (pi[count-1])
		return false;

	po = pi;
	pi += count;
	return true;
}


bool Decoder::getString(const char *&pi, GMSEC_U32 &count, const char *&po)
{
    getU32(pi, &count);
    if (count > GMSEC_STRING_LIMIT)
        return false;

    if (count < 1)
        return false;

    /* check null terminator */
    if (pi[count-1])
        return false;

    po = pi;
    pi += count;
    return true;
}


GMSEC_U32 Decoder::decodeU32(GMSEC_U32 in)
{
	GMSEC_U32 out = 0;
	const char* p = (const char*) &in;
	getDecoder()->getU32(p, &out);
	return out;
}


GMSEC_U64 Decoder::decodeU64(GMSEC_U64 in)
{
	GMSEC_U64 out = 0;
	const char* p = (const char*) &in;
	getDecoder()->getU64(p, &out);
	return out;
}


static void (*selectGetI16(const char *& error))(const char *&, GMSEC_I16 *)
{
	if (sizeof(GMSEC_U16) == 2)
	{
		if (isBigEndian())
			return &get2ByteI16BigEndian;
		else
			return &get2ByteI16LittleEndian;
	}
	else if (sizeof(GMSEC_U16) == 4)
	{
		if (isBigEndian())
			return &get4ByteI16BigEndian;
		else
			return &get4ByteI16LittleEndian;
	}
	else
	{
		setError(error, "size of GMSEC_U16 is not 2 or 4");
		GMSEC_WARNING << "MessageEncoder::selectGetI16 : Size of GMSEC_U16 is not 2 or 4.";
	}

	return 0;
}


static void (*selectGetU16(const char *&error))(const char *&, GMSEC_U16 *)
{
	if (sizeof(GMSEC_U16) == 2)
	{
		if (isBigEndian())
			return &get2ByteU16BigEndian;
		else
			return &get2ByteU16LittleEndian;
	}
	else if (sizeof(GMSEC_U16) == 4)
	{
		if (isBigEndian())
			return &get4ByteU16BigEndian;
		else
			return &get4ByteU16LittleEndian;
	}
	else
	{
		setError(error, "size of GMSEC_U16 is not 2 or 4");
		GMSEC_WARNING << "MessageEncoder::selectGetU16 : Size of GMSEC_U16 is not 2 or 4.";
	}

	return 0;
}


static void (*selectGetI32(const char *&error))(const char *&, GMSEC_I32 *)
{
	if (sizeof(GMSEC_U32) == 4)
	{
		if (isBigEndian())
			return &get4ByteI32BigEndian;
		else
			return &get4ByteI32LittleEndian;
	}
	else if (sizeof(GMSEC_U32) == 8)
	{
		if (isBigEndian())
			return &get8ByteI32BigEndian;
		else
			return &get8ByteI32LittleEndian;
	}
	else
	{
		setError(error, "size of GMSEC_U32 is not 4 or 8");
		GMSEC_WARNING << "MessageEncoder::selectGetI32 : Size of GMSEC_U32 is not 4 or 8.";
	}

	return 0;
}


static void (*selectGetU32(const char *&error))(const char *&, GMSEC_U32 *)
{
	if (sizeof(GMSEC_U32) == 4)
	{
		if (isBigEndian())
			return &get4ByteU32BigEndian;
		else
			return &get4ByteU32LittleEndian;
	}
	else if (sizeof(GMSEC_U32) == 8)
	{
		if (isBigEndian())
			return &get8ByteU32BigEndian;
		else
			return &get8ByteU32LittleEndian;
	}
	else
	{
		setError(error, "size of GMSEC_U32 is not 2 or 4");
		GMSEC_WARNING << "MessageEncoder::selectGetU32 : Size of GMSEC_U32 is not 2 or 4.";
	}

	return 0;
}


static void (*selectGetI64(const char *&error))(const char *&, GMSEC_I64 *)
{
	if (sizeof(GMSEC_U64) == 8)
	{
		if (isBigEndian())
			return &get8ByteI64BigEndian;
		else
			return &get8ByteI64LittleEndian;
	}
	else
	{
		setError(error, "size of GMSEC_I64 is not 8");
		GMSEC_WARNING << "MessageEncoder::selectGetI64 : Size of GMSEC_I64 is not 8.";
	}

	return 0;
}


static void (*selectGetU64(const char *&error))(const char *&, GMSEC_U64 *)
{
	if (sizeof(GMSEC_U64) == 8)
	{
		if (isBigEndian())
			return &get8ByteU64BigEndian;
		else
			return &get8ByteU64LittleEndian;
	}
	else
	{
		setError(error, "size of GMSEC_U64 is not 8");
		GMSEC_WARNING << "MessageEncoder::selectGetU64 : Size of GMSEC_U64 is not 8.";
	}

	return 0;
}


static void (*selectGetF32(const char *&error))(const char *&, GMSEC_F32 *po)
{
	if (sizeof(GMSEC_U32) == 4)
	{
		if (isBigEndian())
			return &get4ByteF32BigEndian;
		else
			return &get4ByteF32LittleEndian;
	}
	else
	{
		setError(error, "size of GMSEC_F32 is not 4");
		GMSEC_WARNING << "MessageEncoder::selectGetF32 : Size of GMSEC_F32 is not 4.";
	}

	return 0;
}


static void (*selectGetF64(const char *&error))(const char *&, GMSEC_F64 *)
{
	if (sizeof(GMSEC_U64) == 8)
	{
		if (isBigEndian())
			return &get8ByteF64BigEndian;
		else
			return &get8ByteF64LittleEndian;
	}
	else
	{
		setError(error, "size of GMSEC_F64 is not 8");
		GMSEC_WARNING << "MessageEncoder::selectGetF64 : Size of GMSEC_F64 is not 8.";
	}

	return 0;
}


Decoder::Decoder()
		:
		problem(0),
		getI8(&get1ByteI8),
		getI16(selectGetI16(problem)),
		getI32(selectGetI32(problem)),
		getI64(selectGetI64(problem)),
		getU8(&get1ByteU8),
		getU16(selectGetU16(problem)),
		getU32(selectGetU32(problem)),
		getU64(selectGetU64(problem)),
		getF32(selectGetF32(problem)),
		getF64(selectGetF64(problem))
{
}


MessageDecoder::MessageDecoder()
	:
	buffer(0),
	current(0),
	length(0),
	decoder(Decoder::getDecoder())
{
}


MessageDecoder::~MessageDecoder()
{
}


void MessageDecoder::decode(Message &message, GMSEC_U64 count, const char *data)
{
	if (data == NULL)
	{
		setError("Null pointer to data to decode");
	}
	else if (count >= static_cast<GMSEC_U64>(GMSEC_ENCODED_LIMIT))
	{
		setError("Data length exceeded limit");
	}
	else
	{
		DataBuffer tmp(reinterpret_cast<const DataBuffer::data_t*>(data), int(count), false);

		decode(message, tmp);
	}
}


void MessageDecoder::decode(Message &message, const DataBuffer &in)
{
	length = in.size();
	buffer = current = reinterpret_cast<const char *>(in.get());

	/* determine the encoding version */
	if (checkAvailable(GMSEC_ENCODING_BYTES))
	{
		GMSEC_I16 encoding = 0;

		decoder->getI16(current, &encoding);

		/* determine the number of fields in the message */
		if (checkAvailable(GMSEC_FIELD_COUNT_BYTES))
		{
			GMSEC_I32 count = 0;

			decoder->getI32(current, &count);

			for (GMSEC_I32 i = 0; i < count; ++i)
			{
				Field* field = decodeField();

				if (field)
				{
					MessageBuddy::getInternal(message).addField(*field, false);

					// Note: We do NOT delete field object here; ownership is held
					// by the Message object.
				}
			}
		}
		else
		{
			setError("Not enough data for field count");
		}
	}
	else
	{
		setError("Not enough data for decoding");
	}
}


bool MessageDecoder::checkAvailable(GMSEC_U32 count)
{
	return static_cast<GMSEC_U64>(current - buffer + count) <= length;
}


Field* MessageDecoder::decodeField()
{
	Field* field = NULL;

	Field::FieldType type;
	const char*      name = 0;
	bool             isHeader = false;

	if (checkAvailable(GMSEC_TYPE_BYTES))
	{
		GMSEC_U16 tmp;
		decoder->getU16(current, &tmp);
		if (tmp & GMSEC_FLAG_HEADER_FIELD)
		{
			isHeader = true;
			tmp ^= GMSEC_FLAG_HEADER_FIELD;
		}
		type = static_cast<Field::FieldType>(tmp);
	}
	else
	{
		setError("Not enough data for field type");
	}

	GMSEC_U32 count = 0;
	if (!decoder->getString(*this, current, count, name))
	{
		setError("Invalid field name");
	}

	switch (type)
	{
	case Field::CHAR_TYPE:
	{
		GMSEC_I8 value;
		if (checkAvailable(1))
		{
			decoder->getI8(current, &value);
			field = new CharField(name, value);
		}
		else
		{
			setError("MessageEncoder::decode : Not enough data for CHAR_TYPE.");
		}
		break;
	}
	case Field::BOOL_TYPE:
	{
		GMSEC_U8 value;
		if (checkAvailable(1))
		{
			decoder->getU8(current, &value);
			field = new BooleanField(name, (value ? true : false));
		}
		else
		{
			setError("MessageEncoder::decode : Not enough data for BOOL_TYPE.");
		}
		break;
	}
	case Field::I8_TYPE:
	{
		GMSEC_I8 value;
		if (checkAvailable(1))
		{
			decoder->getI8(current, &value);
			field = new I8Field(name, value);
		}
		else
		{
			setError("MessageEncoder::decode : Not enough data for I8_TYPE.");
		}
		break;
	}
	case Field::U8_TYPE:
	{
		GMSEC_U8 value;
		if (checkAvailable(1))
		{
			decoder->getU8(current, (GMSEC_U8*) &value);
			field = new U8Field(name, value);
		}
		else
		{
			setError("MessageEncoder::decode : Not enough data for U8_TYPE.");
		}
		break;
	}
	case Field::I16_TYPE:
	{
		GMSEC_I16 value;
		if (checkAvailable(2))
		{
			decoder->getI16(current, &value);
			field = new I16Field(name, value);
		}
		else
		{
			setError("MessageEncoder::decode : Not enough data for I16_TYPE.");
		}

		break;
	}
	case Field::U16_TYPE:
	{
		GMSEC_U16 value;
		if (checkAvailable(2))
		{
			decoder->getU16(current, &value);
			field = new U16Field(name, value);
		}
		else
		{
			setError("MessageEncoder::decode : Not enough data for U16_TYPE.");
		}
		break;
	}
	case Field::I32_TYPE:
	{
		GMSEC_I32 value;
		if (checkAvailable(4))
		{
			decoder->getI32(current, &value);
			field = new I32Field(name, value);
		}
		else
		{
			setError("MessageEncoder::decode : Not enough data for I32_TYPE.");
		}
		break;
	}
	case Field::U32_TYPE:
	{
		GMSEC_U32 value;
		if (checkAvailable(4))
		{
			decoder->getU32(current, &value);
			field = new U32Field(name, value);
		}
		else
		{
			setError("MessageEncoder::decode : Not enough data for U32_TYPE.");
		}
		break;
	}
	case Field::I64_TYPE:
	{
		GMSEC_I64 value;
		if (checkAvailable(8))
		{
			decoder->getI64(current, &value);
			field = new I64Field(name, value);
		}
		else
		{
			setError("MessageEncoder::decode : Not enough data for I64_TYPE.");
		}
		break;
	}
	case Field::U64_TYPE:
	{
		GMSEC_U64 value;
		if (checkAvailable(8))
		{
			decoder->getU64(current, &value);
			field = new U64Field(name, value);
		}
		else
		{
			setError("MessageEncoder::decode : Not enough data for U64_TYPE.");
		}
		break;
	}
	case Field::F32_TYPE:
	{
		GMSEC_F32 value;
		if (checkAvailable(4))
		{
			decoder->getF32(current, &value);
			field = new F32Field(name, value);
		}
		else
		{
			setError("MessageEncoder::decode : Not enough data for F32_TYPE.");
		}
		break;
	}
	case Field::F64_TYPE:
	{
		GMSEC_F64 value;
		if (checkAvailable(8))
		{
			decoder->getF64(current, &value);
			field = new F64Field(name, value);
		}
		else
		{
			setError("MessageEncoder::decode : Not enough data for F64_TYPE.");
		}
		break;
	}
	case Field::STRING_TYPE:
	{
		GMSEC_U32 count;
		const char* value = 0;
		if (decoder->getString(*this, current, count, value))
		{
			field = new StringField(name, value);
		}
		else
		{
			setError("MessageEncoder::decode : Bad string for STRING_TYPE.");
		}
		break;
	}
	case Field::BIN_TYPE:
	{
		GMSEC_U32 count = 0;
		const char* value = 0;
		if (decoder->getCountedBytes(*this, current, count, value))
		{
			field = new BinaryField(name, (GMSEC_BIN) value, count);
		}
		else
		{
			setError("MessageEncoder::decode : Bad blob for BIN_TYPE.");
		}
		break;
	}
	default:
		setError("MessageEncoder::decode : Unexpected field type decoding field.");
	}

	if (field)
	{
		field->isHeader(isHeader);
	}

	return field;
}


} // end namespace internal
} // end namespace api
} // end namespace gmsec
