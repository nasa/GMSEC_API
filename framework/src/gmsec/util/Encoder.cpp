/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







#define GMSEC_WARN_MISSING_LLONG 1
#include <gmsec/util/Encoder.h>

#include <gmsec/internal/strutil.h>

#include <gmsec/internal/Log.h>


using namespace gmsec::util;
namespace gmsec
{
namespace util
{


Encoder *Encoder::getEncoder()
{
	static Encoder* encoder = 0;
	if (!encoder)
	{
		AutoMutex hold(getSharedMutex());
		if (!encoder)
		{
			encoder = new Encoder;
		}
	}
	return encoder;
}


const char * Encoder::getProblem() const
{
	return problem;
}


Decoder *Decoder::getDecoder()
{
	static Decoder* decoder = 0;
	if (!decoder)
	{
		AutoMutex hold(getSharedMutex());
		if (!decoder)
		{
			decoder = new Decoder;
		}
	}
	return decoder;
}



const char * Decoder::getProblem() const
{
	return problem;
}



MessageEncoder::MessageEncoder()
	:
	buffer(0),
	current(0),
	length(0),
	encoder(Encoder::getEncoder()),
	selector(0)
{
	if (encoder->getProblem())
		setError(encoder->getProblem());
}


MessageEncoder::~MessageEncoder()
{
}


void MessageEncoder::setError(const char *text)
{
	if (!status.isError())
		status = Status(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_ENCODING_ERROR, text);
	/* log error? */
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


Status MessageEncoder::encode(Message &message,
                              GMSEC_U64 &count, char *&data)
{
	count = 0;
	data = 0;

	DataBuffer tmp;
	status = encode(message, tmp);

	if (!status.isError())
	{
		// increase the scope of the data from the method to the object
		buffer.swap(tmp);
		count = buffer.size();
		data = reinterpret_cast<char*>(buffer.raw());

	}

	return status;
}


Status MessageEncoder::encode(Message &message, DataBuffer &out)
{
	if (status.isError())
		return status;

	if (!selector)
		selector = &selectAllFields;

	// ick- two pass approach
	GMSEC_I32 fieldCount = 0;
	status = findLength(message, fieldCount);
	if (status.isError())
		return status;

	int ilen = int(length);
	if (out.size() < ilen && !out.resize(ilen))
	{
		setError("memory allocation failed");
		LOG_ERROR << "MessageEncoder::encode : Memory allocation failed.";
	}
	else
	{
		buffer.resize(0);

		// accept the passed in buffer
		buffer.swap(out);
		status = encode(message, fieldCount);
		// replace- ought to use RAII
		buffer.swap(out);
	}

	// even if there was an error, do not out.resize(0) since they may want the space.

	// compression / encryption is external

	return status;
}


void MessageEncoder::setSelector(FieldSelector s)
{
	selector = s;
}


Status MessageEncoder::findLength(Message &message, GMSEC_I32 &fieldCount)
{
	length = GMSEC_ENCODING_BYTES;

#ifdef GMSEC_ENCODE_HEADER
	/* the message type */
	length += GMSEC_TYPE_BYTES;

	/* the subject */
	const char * subject = 0;
	status = message.GetSubject(subject);
	if (status.isError())
		return status;
	length += GMSEC_LENGTH_BYTES + stringLength(subject) + 1;
#endif

	length += GMSEC_FIELD_COUNT_BYTES;

	FieldIterator iterator(selector);
	Field field;
	status = message.Reset(iterator);

	fieldCount = 0;

	while (!status.isError() && iterator.HasNext())
	{
		status = iterator.Next(field);
		++fieldCount;

		if (!status.isError())
			updateLength(field);
	}

	if (length > size_t(GMSEC_ENCODED_LIMIT))
	{
		status.Set(GMSEC_STATUS_MESSAGE_ERROR,
			GMSEC_ENCODING_ERROR, "Encoded content too large");
		LOG_WARNING << "MessageEncoder::findLength : " << status.Get();
	}

	return status;
}


Status MessageEncoder::encode(Message &message, GMSEC_I32 fieldCount)
{
	current = reinterpret_cast<char*>(buffer.raw());

	GMSEC_U16 version = 1;
	encoder->putU16(&version, current);

#ifdef GMSEC_ENCODE_HEADER
	GMSEC_U16 type = GMSEC_MSG_UNSET;
	status = message.GetType(type);
	if (status.isError())
		return status;
	encoder->putU16(&type, current);

	/* the subject */
	const char * subject = 0;
	status = message.GetSubject(subject);
	if (status.isError())
		return status;
	encoder->putString(subject, current);
#endif

	encoder->putI32(&fieldCount, current);

	Field field;
	FieldIterator iterator(selector);
	status = message.Reset(iterator);
	while (!status.isError() && iterator.HasNext())
	{
		status = iterator.Next(field);

		if (!status.isError())
			encode(field);
	}

	return status;
}


/* this uses the same approach as the Message Bus serialization.  I suspect
that the compression could be improved by storing all of the field names
together... */


bool MessageEncoder::encode(Field &field)
{
	GMSEC_TYPE type = GMSEC_TYPE_UNSET;
	status = field.GetType(type);
	if (status.isError())
		return false;
	{
		GMSEC_U16 tmp = type;
#ifdef GMSEC_C2
		if (field.isHeader())
			tmp |= GMSEC_FLAG_HEADER_FIELD;
#endif
		encoder->putU16(&tmp, current);
	}

	const char *name = 0;
	status = field.GetName(name);
	if (status.isError())
		return false;
	encoder->putString(name, current);

	switch (type)
	{
	case GMSEC_TYPE_CHAR:
	{
		GMSEC_CHAR value;
		status = field.GetValue(value);
		if (!status.isError())
			encoder->putI8((GMSEC_I8 *) &value, current);
		break;
	}
	case GMSEC_TYPE_BOOL:
	{
		GMSEC_BOOL value = GMSEC_FALSE;
		status = field.GetValue(value);
		if (!status.isError())
		{
			GMSEC_U8 tmp = value ? 1 : 0;
			encoder->putU8(&tmp, current);
		}
		break;
	}
	case GMSEC_TYPE_I8:
	{
		GMSEC_I8 value;
		status = field.GetValue(value);
		if (!status.isError())
			encoder->putI8((GMSEC_I8 *) &value, current);
		break;
	}
	case GMSEC_TYPE_U8:
	{
		GMSEC_U8 value;
		status = field.GetValue(value);
		if (!status.isError())
			encoder->putU8((GMSEC_U8 *) &value, current);
		break;
	}
	case GMSEC_TYPE_I16:
	{
		GMSEC_I16 value;
		status = field.GetValue(value);
		if (!status.isError())
			encoder->putI16(&value, current);
		break;
	}
	case GMSEC_TYPE_U16:
	{
		GMSEC_U16 value;
		status = field.GetValue(value);
		if (!status.isError())
			encoder->putU16(&value, current);
		break;
	}
	case GMSEC_TYPE_I32:
	{
		GMSEC_I32 value;
		status = field.GetValue(value);
		if (!status.isError())
			encoder->putI32(&value, current);
		break;
	}
	case GMSEC_TYPE_U32:
	{
		GMSEC_U32 value;
		status = field.GetValue(value);
		if (!status.isError())
			encoder->putU32(&value, current);
		break;
	}
	case GMSEC_TYPE_F32:
	{
		GMSEC_F32 value;
		status = field.GetValue(value);
		if (!status.isError())
			encoder->putF32(&value, current);
		break;
	}
	case GMSEC_TYPE_F64:
	{
		GMSEC_F64 value;
		status = field.GetValue(value);
		if (!status.isError())
			encoder->putF64(&value, current);
		break;
	}
	case GMSEC_TYPE_STRING:
	{
		GMSEC_STRING value = 0;
		status = field.GetValue(value);
		if (!status.isError())
			encoder->putString(value, current);
		break;
	}
	case GMSEC_TYPE_BIN:
	{
		GMSEC_BIN value = 0;
		GMSEC_U32 count = 0;
		status = field.GetValue(&value, count);
		if (!status.isError())
			encoder->putCountedBytes((const char *) value, count, current);
		break;
	}
	case GMSEC_TYPE_I64:
	{
		GMSEC_I64 value;
		status = field.GetValue(value);
		if (!status.isError())
			encoder->putI64(&value, current);
		break;
	}
	case GMSEC_TYPE_U64:
	{
		GMSEC_U64 value;
		status = field.GetValue(value);
		if (!status.isError())
			encoder->putU64(&value, current);
		break;
	}
#ifdef GMSEC_COMPOUND_FIELDS
	case GMSEC_TYPE_COMPOUND:
	{
		GMSEC_I32 count = 0;
		FieldIterator iterator;
		status = field.GetValue(iterator, count);
		if (!status.isError())
		{
			encoder->putI32(count, current);
			while (!status.isError() && iterator.hasNext())
			{
				Field subfield;
				status = iterator.GetNextField(subfield);
				if (!status.isError())
					status = encode(fields[i]);
			}
		}
		break;
	}
#endif /* GMSEC_COMPOUND_FIELDS */
	default:
		setError("Unexpected field type");
		LOG_WARNING << "MessageEncoder::encode : Unexpected field type. ";
		break;
	}

	return !status.isError();
}


bool MessageEncoder::updateLength(Field &field)
{
	GMSEC_TYPE type = GMSEC_TYPE_UNSET;
	status = field.GetType(type);
	if (status.isError())
		return false;

	length += GMSEC_TYPE_BYTES;

	const char *name = 0;
	status = field.GetName(name);
	if (status.isError())
		return false;

	length += GMSEC_LENGTH_BYTES + stringLength(name) + 1;

	switch (type)
	{
	case GMSEC_TYPE_CHAR:
		length += 1;
		break;
	case GMSEC_TYPE_I8:
	case GMSEC_TYPE_U8:
		length += 1;
		break;
	case GMSEC_TYPE_BOOL:
		length += 1;
		break;
	case GMSEC_TYPE_I16:
		length += 2;
		break;
	case GMSEC_TYPE_U16:
		length += 2;
		break;
	case GMSEC_TYPE_I32:
		length += 4;
		break;
	case GMSEC_TYPE_U32:
		length += 4;
		break;
	case GMSEC_TYPE_F32:
		length += 4;
		break;
	case GMSEC_TYPE_F64:
		length += 8;
		break;
	case GMSEC_TYPE_STRING:
	{
		GMSEC_STRING value = 0;
		status = field.GetValue(value);
		if (!status.isError())
		{
			size_t count = stringLength(value);
			if (count > size_t(GMSEC_STRING_LIMIT))
			{
				setError("excessive string length");
				LOG_WARNING << "MessageEncoder::updateLength : Excessive string length ";
			}
			else
				length += GMSEC_LENGTH_BYTES + count + 1;
		}
		break;
	}
	case GMSEC_TYPE_BIN:
	{
		GMSEC_BIN value = 0;
		GMSEC_U32 count = 0;
		status = field.GetValue(&value, count);
		if (!status.isError())
		{
			if (count > size_t(GMSEC_BIN_LIMIT))
			{
				setError("excessive binary data");
				LOG_WARNING << "MessageEncoder::updateLength : Excessive binary data";
			}
			else
				length += GMSEC_LENGTH_BYTES + count;
		}
		break;
	}
	case GMSEC_TYPE_I64:
		length += 8;
		break;
	case GMSEC_TYPE_U64:
		length += 8;
		break;
#ifdef GMSEC_COMPOUND_FIELDS
	case GMSEC_TYPE_COMPOUND:
	{
		FieldIterator iterator;
		GMSEC_I32 count = 0;
		status = field.GetValue(iterator, count);
		if (!status.isError())
		{
			length += GMSEC_COMPOUND_BYTES;
			while (!status.isError() && iterator.hasNext())
			{
				Field subfield;
				status = iterator.GetNextField(subfield);
				if (!status.isError())
					updateLength(subfield);
			}
		}
		break;
	}
#endif /* GMSEC_COMPOUND_FIELDS */
	default:
		setError("Unable to encode field into message");
		LOG_WARNING << "MessageEncoder::updateLength : Unable to encode field into message.";
		break;
	}

	return !status.isError();
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
		problem = text;
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
		LOG_WARNING << "MessageEncoder::selectPutI16 : Size of GMSEC_U16 is not 2 or 4.";
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
		LOG_WARNING << "MessageEncoder::selectPutI16 : Size of GMSEC_U16 is not 2 or 4.";
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
		LOG_WARNING << "MessageEncoder::selectPutI32 : Size of GMSEC_U32 is not 4 or 8.";
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
		LOG_WARNING << "MessageEncoder::selectPutU32 : Size of GMSEC_U32 is not 2 or 4.";
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
		LOG_WARNING << "MessageEncoder::selectPutI64 : Size of GMSEC_U64 is not 8.";
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
		LOG_WARNING << "MessageEncoder::selectPutU64 : Size of GMSEC_U64 is not 8.";
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
		LOG_WARNING << "MessageEncoder::selectPutF32 : Size of GMSEC_F32 is not 4.";
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
		LOG_WARNING << "MessageEncoder::selectPutF64 : Size of GMSEC_F64 is not 8.";
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
	size_t s = stringLength(pi);
	GMSEC_U32 count = GMSEC_U32(s) + 1;
	putCountedBytes(pi, count, po);
}


GMSEC_U32 Encoder::encodeU32(GMSEC_U32 in)
{
	GMSEC_U32 out = 0;
	char* p = (char*) &out;
	getEncoder()->putU32(&in, p);
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
	po[0] = pi[0];
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
	if (count > size_t(GMSEC_BIN_LIMIT))
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
	if (count > size_t(GMSEC_STRING_LIMIT))
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
	if (count > size_t(GMSEC_STRING_LIMIT))
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
		LOG_WARNING << "MessageEncoder::selectGetI16 : Size of GMSEC_U16 is not 2 or 4.";
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
		LOG_WARNING << "MessageEncoder::selectGetU16 : Size of GMSEC_U16 is not 2 or 4.";
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
		LOG_WARNING << "MessageEncoder::selectGetI32 : Size of GMSEC_U32 is not 4 or 8.";
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
		LOG_WARNING << "MessageEncoder::selectGetU32 : Size of GMSEC_U32 is not 2 or 4.";
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
		LOG_WARNING << "MessageEncoder::selectGetI64 : Size of GMSEC_I64 is not 8.";
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
		LOG_WARNING << "MessageEncoder::selectGetU64 : Size of GMSEC_U64 is not 8.";
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
		LOG_WARNING << "MessageEncoder::selectGetF32 : Size of GMSEC_F32 is not 4.";
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
		LOG_WARNING << "MessageEncoder::selectGetF64 : Size of GMSEC_F64 is not 8.";
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


bool MessageDecoder::checkAvailable(int count)
{
	if (GMSEC_U64(current - buffer + count) <= length)
		return true;
	setError("Out of data");
	return false;
}


void MessageDecoder::setError(const char *text)
{
	status = Status(GMSEC_STATUS_MESSAGE_ERROR,
	                GMSEC_ENCODING_ERROR, text);
}



Status MessageDecoder::decode(Message &message,
                              GMSEC_U64 count, const char *data)
{
	Status result;

	if (data == NULL)
		setError("Null pointer to data to decode");
	else if (count >= GMSEC_U32(GMSEC_ENCODED_LIMIT))
		setError("Data length exceeded limit");
	else
	{
		DataBuffer tmp(reinterpret_cast<const DataBuffer::data_t*>(data), int(count), false);
		result = decode(message, tmp);
	}

	if (!result.isError() && status.isError())
		result = status;

	if (result.isError())
	{
		LOG_WARNING << "MessageDecoder::decode: " << result.Get();
	}

	return result;
}


Status MessageDecoder::decode(Message &message, const DataBuffer &in)
{
	Status result = decodeAux(message, in);

	if (!result.isError() && status.isError())
		result = status;

	if (result.isError())
	{
		LOG_WARNING << "MessageDecoder::decode: " << result.Get();
	}

	return result;
}


Status MessageDecoder::decodeAux(Message &message, const DataBuffer &in)
{
	length = in.size();
	buffer = current = reinterpret_cast<const char *>(in.get());

	/* determine the encoding version */
	GMSEC_I16 encoding;
	if (checkAvailable(GMSEC_ENCODING_BYTES))
		decoder->getI16(current, &encoding);
	else
	{
		setError("Not enough data for encoding");
		return status;
	}

	/* determine the number of fields in the message */
	GMSEC_I32 count;
	if (checkAvailable(GMSEC_FIELD_COUNT_BYTES))
		decoder->getI32(current, &count);
	else
	{
		setError("Not enough data for field count");
		return status;
	}

	GMSEC_I32 i = 0;
	Field field;
	while (!status.isError() && i < count)
	{
		++i;
		if (decode(field))
			status = message.AddField(field);
		else
			setError("Error decoding field");
	}

	return status;
}


bool MessageDecoder::decode(Field &field)
{
	if (status.isError())
		return false;

	GMSEC_TYPE type = GMSEC_TYPE_UNSET;
	if (checkAvailable(GMSEC_TYPE_BYTES))
	{
		GMSEC_U16 tmp;
		decoder->getU16(current, &tmp);
		if (tmp & GMSEC_FLAG_HEADER_FIELD)
		{
#ifdef GMSEC_C2
			field.setHeader(true);
#endif
			tmp ^= GMSEC_FLAG_HEADER_FIELD;
		}
		type = tmp;
		status = field.SetType(type);
	}
	else
		setError("Not enough data for field type");

	if (status.isError())
		return false;

	const char *name = 0;
	GMSEC_U32 count = 0;
	if (decoder->getString(*this, current, count, name))
		status = field.SetName(name);
	else
		setError("Invalid field name");

	if (status.isError())
		return false;

	switch (type)
	{
	case GMSEC_TYPE_CHAR:
	{
		GMSEC_I8 value;
		if (checkAvailable(1))
		{
			decoder->getI8(current, &value);
			status = field.SetValue((GMSEC_CHAR) value);
		}
		else
		{
			LOG_WARNING << "MessageEncoder::decode : Not enough data for GMSEC_TYPE_CHAR.";
		}
		break;
	}
	case GMSEC_TYPE_BOOL:
	{
		GMSEC_U8 value;
		if (checkAvailable(1))
		{
			decoder->getU8(current, &value);
			status = field.SetValue((GMSEC_BOOL) value);
		}
		else
		{
			LOG_WARNING << "MessageEncoder::decode : Not enough data for GMSEC_TYPE_BOOL.";
		}
		break;
	}
	case GMSEC_TYPE_I8:
	{
		GMSEC_I8 value;
		if (checkAvailable(1))
		{
			decoder->getI8(current, &value);
			status = field.SetValue((GMSEC_I8) value);
		}
		else
		{
			LOG_WARNING << "MessageEncoder::decode : Not enough data for GMSEC_TYPE_I8.";
		}
		break;
	}
	case GMSEC_TYPE_U8:
	{
		GMSEC_U8 value;
		if (checkAvailable(1))
		{
			decoder->getU8(current, &value);
			status = field.SetValue((GMSEC_U8) value);
		}
		else
		{
			LOG_WARNING << "MessageEncoder::decode : Not enough data for GMSEC_TYPE_U8.";
		}
		break;
	}
	case GMSEC_TYPE_I16:
	{
		GMSEC_I16 value;
		if (checkAvailable(2))
		{
			decoder->getI16(current, &value);
			status = field.SetValue(value);
		}
		else
		{
			LOG_WARNING << "MessageEncoder::decode : Not enough data for GMSEC_TYPE_I16.";
		}

		break;
	}
	case GMSEC_TYPE_U16:
	{
		GMSEC_U16 value;
		if (checkAvailable(2))
		{
			decoder->getU16(current, &value);
			status = field.SetValue(value);
		}
		else
		{
			LOG_WARNING << "MessageEncoder::decode : Not enough data for GMSEC_TYPE_U16.";
		}
		break;
	}
	case GMSEC_TYPE_I32:
	{
		GMSEC_I32 value;
		if (checkAvailable(4))
		{
			decoder->getI32(current, &value);
			status = field.SetValue(value);
		}
		else
		{
			LOG_WARNING << "MessageEncoder::decode : Not enough data for GMSEC_TYPE_I32.";
		}
		break;
	}
	case GMSEC_TYPE_U32:
	{
		GMSEC_U32 value;
		if (checkAvailable(4))
		{
			decoder->getU32(current, &value);
			status = field.SetValue(value);
		}
		else
		{
			LOG_WARNING << "MessageEncoder::decode : Not enough data for GMSEC_TYPE_U32.";
		}
		break;
	}
	case GMSEC_TYPE_I64:
	{
		GMSEC_I64 value;
		if (checkAvailable(8))
		{
			decoder->getI64(current, &value);
			status = field.SetValue(value);
		}
		else
		{
			LOG_WARNING << "MessageEncoder::decode : Not enough data for GMSEC_TYPE_I64.";
		}
		break;
	}
	case GMSEC_TYPE_U64:
	{
		GMSEC_U64 value;
		if (checkAvailable(8))
		{
			decoder->getU64(current, &value);
			status = field.SetValue(value);
		}
		else
		{
			LOG_WARNING << "MessageEncoder::decode : Not enough data for GMSEC_TYPE_U64.";
		}
		break;
	}
	case GMSEC_TYPE_F32:
	{
		GMSEC_F32 value;
		if (checkAvailable(4))
		{
			decoder->getF32(current, &value);
			status = field.SetValue(value);
		}
		else
		{
			LOG_WARNING << "MessageEncoder::decode : Not enough data for GMSEC_TYPE_F32.";
		}
		break;
	}
	case GMSEC_TYPE_F64:
	{
		GMSEC_F64 value;
		if (checkAvailable(8))
		{
			decoder->getF64(current, &value);
			status = field.SetValue(value);
		}
		else
		{
			LOG_WARNING << "MessageEncoder::decode : Not enough data for GMSEC_TYPE_F64.";
		}
		break;
	}
	case GMSEC_TYPE_STRING:
	{
		GMSEC_U32 count;
		const char *value = 0;
		if (decoder->getString(*this, current, count, value))
			status = field.SetValue((GMSEC_STRING) value);
		else
		{
			setError("Bad string");
			LOG_WARNING << "MessageEncoder::decode : Bad string.";
		}
		break;
	}
	case GMSEC_TYPE_BIN:
	{
		GMSEC_U32 count = 0;
		const char *value = 0;
		if (decoder->getCountedBytes(*this, current, count, value))
			status = field.SetValue((GMSEC_BIN) value, count);
		else
		{
			setError("Bad blob");
			LOG_WARNING << "MessageEncoder::decode : Bad blob.";
		}
		break;
	}
#ifdef GMSEC_COMPOUND_FIELDS
	case GMSEC_TYPE_COMPOUND:
	{
		GMSEC_I32 count = 0;
		if (checkAvailable(4))
			decoder->getI32(current, &count);
		GMSEC_I32 i = 0;
		while (!status.isError && i < count)
		{
			++i;
			Field subfield;
			if (decode(subfield))
				status = field.AddField(subfield);
		}
		break;
	}
#endif /* GMSEC_COMPOUND_FIELDS */
	default:
		setError("Unexpected field type calculating field size");
		LOG_WARNING << "MessageEncoder::decode : Unexpected field type decoding field.";
		break;
	}

	return !status.isError();
}


} // namespace util
} // namespace gmsec
