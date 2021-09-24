/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







#ifndef gmsec_util_Encoder_h
#define gmsec_util_Encoder_h


#include <gmsec/Message.h>
#include <gmsec/util/Buffer.h>
#include <gmsec/util/Deprecated.h>

#include <string.h>	// memcpy


namespace gmsec
{

namespace util
{

/**	@class Encoder
 *
 *	@brief This class is responsible for converting data into different formats
 *
**/
class GMSEC_API Encoder
{
private:
	const char * problem;

public:
	/**	@fn getEncoder()
	 *
	 *	@brief Returns a pointer to the singleton encoder object
	 *
	**/
	GMSEC_DEPRECATED static Encoder * getEncoder();

	/**	@fn getProblem() const
	 *
	 *	@brief Returns the char* containing the problem string
	 *
	**/
	const char * getProblem() const;

	/** @fn putI8
	 *
	 *	@brief These function pointers are used to reference whichever encoding format
	 *	is applicable in a given situation
	 *
	**/
	void (* const putI8)(const GMSEC_I8  *pi, char *&po);
	/** @fn putI16
	 *
	 *	@brief These function pointers are used to reference whichever encoding format
	 *	is applicable in a given situation
	 *
	**/
	void (* const putI16)(const GMSEC_I16 *pi, char *&po);
	/** @fn putI32
	 *
	 *	@brief These function pointers are used to reference whichever encoding format
	 *	is applicable in a given situation
	 *
	**/
	void (* const putI32)(const GMSEC_I32 *pi, char *&po);
	/** @fn putI64
	 *
	 *	@brief These function pointers are used to reference whichever encoding format
	 *	is applicable in a given situation
	 *
	**/
	void (* const putI64)(const GMSEC_I64 *pi, char *&po);

	/** @fn putU8
	 *
	 *	@brief These function pointers are used to reference whichever encoding format
	 *	is applicable in a given situation
	 *
	**/
	void (* const putU8)(const GMSEC_U8  *pi, char *&po);
	/** @fn putU16
	 *
	 *	@brief These function pointers are used to reference whichever encoding format
	 *	is applicable in a given situation
	 *
	**/
	void (* const putU16)(const GMSEC_U16 *pi, char *&po);
	/** @fn putU32
	 *
	 *	@brief These function pointers are used to reference whichever encoding format
	 *	is applicable in a given situation
	 *
	**/
	void (* const putU32)(const GMSEC_U32 *pi, char *&po);
	/** @fn putU64
	 *
	 *	@brief These function pointers are used to reference whichever encoding format
	 *	is applicable in a given situation
	 *
	**/
	void (* const putU64)(const GMSEC_U64 *pi, char *&po);

	/** @fn putF32
	 *
	 *	@brief These function pointers are used to reference whichever encoding format
	 *	is applicable in a given situation
	 *
	**/
	void (* const putF32)(const GMSEC_F32 *pi, char *&po);
	/** @fn putF64
	 *
	 *	@brief These function pointers are used to reference whichever encoding format
	 *	is applicable in a given situation
	 *
	**/
	void (* const putF64)(const GMSEC_F64 *pi, char *&po);

	/**	@fn putRawBytes(const char *pi, GMSEC_U32 count, char *&po)
	 *
	 *	@brief This function copies memory from one char* to the other
	 *
	 *	@param pi - source of bytes to be copied
	 *	@param count - number of bytes to copy
	 *	@param po - destination where bytes are to be copied
	 *
	**/
	void putRawBytes(const char *pi, GMSEC_U32 count, char *&po);

	/**	@fn putCountedBytes (const char *pi, GMSEC_U32 count, char *&po)
	 *
	 *	@brief This function calls putRawBytes with the count variable set as the
	 *	length of the destination.
	 *
	 *	@param pi - source of bytes to be copied
	 *	@param count - number of bytes available for po
	 *	@param po - destination where bytes are to be copied
	 *
	**/
	void putCountedBytes(const char *pi, GMSEC_U32 count, char *&po);

	/**	@fn putString (const char *pi, char *&po)
	 *
	 *	@brief This functino accepts a c formatted string and copies it
	 *
	 *	@param pi - a c formatted string
	 *	@param po - the destination where the string is to be copied
	 *
	**/
	void putString(const char *pi, char *&po);

	/** @fn encodeU32 (GMSEC_U32 in)
	 *
	 *  @brief Encodes an unsigned 32-bit value into network ready format
	 *
	 *  @param in - the value to convert.
	**/
	GMSEC_DEPRECATED static GMSEC_U32 encodeU32(GMSEC_U32 in);


private:

	Encoder();
	Encoder(const Encoder &other);
	Encoder &operator=(const Encoder &other);
};


class MessageDecoder;

/** @class Decoder
 *
 *	@brief This class decodes data from the applicable format
 *
**/
class GMSEC_API Decoder
{
private:
	const char *problem;

public:

	/**	@fn getDecoder()
	 *
	 *	@brief This function returns the singleton decoder.
	 *
	**/
	GMSEC_DEPRECATED static Decoder * getDecoder();

	/**	@fn getProblem() const
	 *
	 *	@brief This function returns a char* containing the problem string.
	 *
	**/
	const char * getProblem() const;

	/** @fn getI8
	 *
	 *	@brief These function pointers are used to reference whichever decoding format
	 *	is applicable in a given situation
	 *
	**/
	void (*getI8)(const char *&pi, GMSEC_I8  *po);
	/** @fn getI16
	 *
	 *	@brief These function pointers are used to reference whichever decoding format
	 *	is applicable in a given situation
	 *
	**/
	void (*getI16)(const char *&pi, GMSEC_I16 *po);
	/** @fn getI32
	 *
	 *	@brief These function pointers are used to reference whichever decoding format
	 *	is applicable in a given situation
	 *
	**/
	void (*getI32)(const char *&pi, GMSEC_I32 *po);
	/** @fn getI64
	 *
	 *	@brief These function pointers are used to reference whichever decoding format
	 *	is applicable in a given situation
	 *
	**/
	void (*getI64)(const char *&pi, GMSEC_I64 *po);

	/** @fn getU8
	 *
	 *	@brief These function pointers are used to reference whichever decoding format
	 *	is applicable in a given situation
	 *
	**/
	void (*getU8)(const char *&pi, GMSEC_U8  *po);
	/** @fn getU16
	 *
	 *	@brief These function pointers are used to reference whichever decoding format
	 *	is applicable in a given situation
	 *
	**/
	void (*getU16)(const char *&pi, GMSEC_U16 *po);
	/** @fn getU32
	 *
	 *	@brief These function pointers are used to reference whichever decoding format
	 *	is applicable in a given situation
	 *
	**/
	void (*getU32)(const char *&pi, GMSEC_U32 *po);
	/** @fn getU64
	 *
	 *	@brief These function pointers are used to reference whichever decoding format
	 *	is applicable in a given situation
	 *
	**/
	void (*getU64)(const char *&pi, GMSEC_U64 *po);

	/** @fn getF32
	 *
	 *	@brief These function pointers are used to reference whichever decoding format
	 *	is applicable in a given situation
	 *
	**/
	void (*getF32)(const char *&pi, GMSEC_F32 *po);
	/** @fn getF64
	 *
	 *	@brief These function pointers are used to reference whichever decoding format
	 *	is applicable in a given situation
	 *
	**/
	void (*getF64)(const char *&pi, GMSEC_F64 *po);

	/**	@fn getCountedBytes (MessageDecoder &owner,
	 *		const char *&pi, GMSEC_U32 &count, const char *&po)
	 *
	 *	@brief This function copies the bytes from pi to po, if pi is not
	 *	greater than the buffer size
	 *
	 *  @param owner - the encoder owner
	 *	@param pi - the bytes to be copied
	 *	@param count - a reference to the location where the size of pi will be stored
	 *	@param po - the destination
	 *
	**/
	bool getCountedBytes(MessageDecoder &owner,
	                     const char *&pi, GMSEC_U32 &count, const char *&po);

	/**	@fn getString (MessageDecoder &owner,
	 *		const char *&pi, GMSEC_U32 &count, const char *&po)
	 *
	 *	@brief This function copies the c formatting string
	 *
	 *  @param owner - the decoder owner
	 *	@param pi - The location of the c formatted string
	 *	@param count - A reference to the location where the length of the
	 *	string will be stored
	 *	@param po - The location where the c formatted string will be copied
	 *
	**/
	bool getString(MessageDecoder &owner,
	               const char *&pi, GMSEC_U32 &count, const char *&po);

	bool getString(const char *&pi, GMSEC_U32 &count, const char *&po);

	/** @fn decodeU32 (GMSEC_U32 in)
	 *
	 *  @brief Decodes an unsigned 32-bit value into host native format
	 *
	 *  @param in - the value to convert.
	**/
	GMSEC_DEPRECATED static GMSEC_U32 decodeU32(GMSEC_U32 in);


private:
	Decoder();
	Decoder &operator= (const Decoder &other);

	bool initialize();
	void setError(const char *text);

};


/**	@class MessageEncoder
 *
 *	@brief This class is responsible for encoding a message
 *
**/
class GMSEC_API MessageEncoder
{
public:

	GMSEC_DEPRECATED MessageEncoder();
	~MessageEncoder();

	/**	@fn encode(Message &message, GMSEC_U64 &bytes, char *&data)
	 *
	 *	@brief This function encodes a message.
	 *
	 *	@param message - The message to be encoded
	 *	@param bytes - Once the message is encoded, this field will be populated
	 *	with the length of the message.
	 *	@param data - The referenced pointer will be directed at a char* with a
	 *	length able to hold the content of the message. WARNING - the encoder manages
	 *	the data buffer internally. When the encoder is deleted or goes out of scope,
	 *	the data buffer will be deleted automatically and further attempts to access
	 *	the data will be undefined.
	**/
	Status encode(Message &message, GMSEC_U64 &bytes, char *&data);

	/**	@fn encode(Message &message, GMSEC_U64 &bytes, char *&data)
	 *
	 *	@brief This function encodes a message.
	 *
	 *	@param message - The message to be encoded
	 *	@param out - This caller provided buffer will own the encoded data.
	**/
	Status encode(Message &message, DataBuffer &out);

	void setSelector(FieldSelector selector);

private:

	MessageEncoder(MessageEncoder &encoder);
	MessageEncoder& operator= (const MessageEncoder &encoder);


	Status findLength(Message &message, GMSEC_I32 &fieldCount);
	bool updateLength(Field &field);

	Status encode(Message &message, GMSEC_I32 fieldCount);
	bool encode(Field &field);

	void setError(const char *text);


	Status status;
	DataBuffer buffer;
	char * current;
	GMSEC_U64 length;
	Encoder * encoder;
	FieldSelector selector;

};

/**	@class MessageDecoder
 *
 *	@brief This class is responsible for decoding data and placing it into messages
 *
**/
class GMSEC_API MessageDecoder
{
public:

	GMSEC_DEPRECATED MessageDecoder();
	~MessageDecoder();

	/**	@fn decode(Message &message, GMSEC_U64 bytes, const char *data)
	 *
	 *	@brief This function accepts a reference to a message to place the decoded data
	 *	into
	 *
	 *	@param message - The message to insert data into
	 *	@param bytes - The number of bytes contained in the data
	 *	@param data - This is the data to be decoded
	 *
	**/
	Status decode(Message &message, GMSEC_U64 bytes, const char *data);

	/**	@fn decode(Message &message, const DataBuffer &in)
	 *
	 *	@brief This function accepts a reference to a message into which the decoded
	 *	data is stored.
	 *
	 *	@param message - The message to insert data into
	 *	@param in - This is the data to be decoded
	 *
	**/
	Status decode(Message &message, const DataBuffer &in);

	/**	@fn checkAvailable(int count)
	 *
	 *	@brief This function checks to see if there is enough memory available for
	 *	provided number of bytes
	 *
	**/
	bool checkAvailable(int count);


private:

	MessageDecoder(MessageDecoder &decoder);
	MessageDecoder& operator=(const MessageDecoder &decoder);


	void setError(const char *text);
	Status decode(Message &message);
	Status decodeAux(Message &message, const DataBuffer &in);
	bool decode(Field &field);


	Status status;
	const char * buffer;
	const char * current;
	GMSEC_U64 length;
	Decoder * decoder;

};


const char GMSEC_ENCODED_CONTENT_FIELD[] = "GMSEC_CONTENT";

/* largest encoded blob (2GB) */
const int GMSEC_ENCODED_LIMIT = 0x80000000;


/* largest blob of data (1GB) */
const int GMSEC_BIN_LIMIT = 0x40000000;

/* maximum length of GMSEC_STRING (16MB) */
const int GMSEC_STRING_LIMIT = 0x01000000;

/* how many bytes are used to indicate the encoding */
const int GMSEC_ENCODING_BYTES = 2;

/* how many bytes are used to indicate the field type */
const int GMSEC_TYPE_BYTES = 2;

/* how many bytes are used to indicate the number of fields */
const int GMSEC_FIELD_COUNT_BYTES = 4;

/* how many bytes are used to store a string/blob length */
const int GMSEC_LENGTH_BYTES = 4;

/* how many bytes are used to store a COMPOUND_FIELD count */
const int GMSEC_COMPOUND_BYTES = 4;

/* header fields have this bit applied to their type */
const GMSEC_U16 GMSEC_FLAG_HEADER_FIELD = 0x8000;


} /* namespace util */
} /* namespace gmsec */


#endif /* gmsec_util_Encoder_h */
