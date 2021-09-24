/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni;

import gov.nasa.gsfc.gmsec.api.*;
import gov.nasa.gsfc.gmsec.api.field.*;

import gov.nasa.gsfc.gmsec.api.jni.field.JNIField;
import gov.nasa.gsfc.gmsec.api.jni.field.JNIBinaryField;
import gov.nasa.gsfc.gmsec.api.jni.field.JNIBooleanField;
import gov.nasa.gsfc.gmsec.api.jni.field.JNICharField;
import gov.nasa.gsfc.gmsec.api.jni.field.JNII16Field;
import gov.nasa.gsfc.gmsec.api.jni.field.JNII32Field;
import gov.nasa.gsfc.gmsec.api.jni.field.JNII64Field;
import gov.nasa.gsfc.gmsec.api.jni.field.JNII8Field;
import gov.nasa.gsfc.gmsec.api.jni.field.JNIF32Field;
import gov.nasa.gsfc.gmsec.api.jni.field.JNIF64Field;
import gov.nasa.gsfc.gmsec.api.jni.field.JNIStringField;
import gov.nasa.gsfc.gmsec.api.jni.field.JNIU16Field;
import gov.nasa.gsfc.gmsec.api.jni.field.JNIU32Field;
import gov.nasa.gsfc.gmsec.api.jni.field.JNIU64Field;
import gov.nasa.gsfc.gmsec.api.jni.field.JNIU8Field;
import gov.nasa.gsfc.gmsec.api.jni.field.JNIFieldConverter;

import java.util.Collection;


public class JNIMessage
{
	protected long    swigCPtr;
	protected boolean swigCMemOwn;


	public JNIMessage(long cPtr, boolean cMemoryOwn)
	{
		swigCPtr    = cPtr;
		swigCMemOwn = cMemoryOwn;
	}


	protected void finalize() throws Throwable
	{
		delete();
		super.finalize();
	}


	public synchronized void delete()
	{
		if (swigCPtr != 0 && swigCMemOwn)
		{
			gmsecJNI.delete_Message(swigCPtr, this);
		}

		swigCPtr    = 0;
		swigCMemOwn = false;
	}


	public static long getCPtr(JNIMessage obj)
	{
		return (obj == null) ? 0 : obj.swigCPtr;
	}


	public JNIMessage(String subject, Message.MessageKind kind)
	{
		this(gmsecJNI.new_Message(subject, kind.ordinal()), true);
	}


	public JNIMessage(String subject, Message.MessageKind kind, Config config)
	{
		this(gmsecJNI.new_Message(subject, kind.ordinal(), JNIConfig.getCPtr(Config.getInternal(config)), Config.getInternal(config)), true);
	}


	public JNIMessage(String data) throws GMSEC_Exception
	{
		this(gmsecJNI.new_Message(data), true);
	}


	public JNIMessage(JNIMessage other)
	{
		this(gmsecJNI.new_Message_Copy(JNIMessage.getCPtr(other), other), true);
	}


	public void addConfig(Config config)
	{
		JNIConfig cfg = new JNIConfig(config);

		gmsecJNI.Message_AddConfig(swigCPtr, this, JNIConfig.getCPtr(cfg), cfg);
	}


	public Config getConfig()
	{
		long cPtr = gmsecJNI.Message_GetConfig(swigCPtr, this);

		return new Config(new JNIConfig(cPtr, false));
	}


	public String getSubject()
	{
		return gmsecJNI.Message_GetSubject(swigCPtr, this);
	}


	public void setSubject(String subject)
	{
		gmsecJNI.Message_SetSubject(swigCPtr, this, subject);
	}


	public Message.MessageKind getKind()
	{
		int rawKind = gmsecJNI.Message_GetKind(swigCPtr, this);

		return Message.MessageKind.values()[rawKind];
	}


	public boolean addField(Field field)
	{
		// By deducing the Field type, we can then perform a legit cast to the
		// appropriate JNI-Field object, so that we can acquire the underlying
		// address associated with the C++ Field, and then call AddField().
		//
		Field.FieldType type = field.getType();

		boolean result = false;

		switch (type)
		{
			case BIN_TYPE:
			{
				JNIBinaryField tmp = (JNIBinaryField) Field.getInternal(field);
				result = gmsecJNI.Message_AddField(swigCPtr, this, JNIBinaryField.getCPtr(tmp), tmp);
				break;
			}

			case BOOL_TYPE:
			{
				JNIBooleanField tmp = (JNIBooleanField) Field.getInternal(field);
				result = gmsecJNI.Message_AddField(swigCPtr, this, JNIBooleanField.getCPtr(tmp), tmp);
				break;
			}

			case CHAR_TYPE:
			{
				JNICharField tmp = (JNICharField) Field.getInternal(field);
				result = gmsecJNI.Message_AddField(swigCPtr, this, JNICharField.getCPtr(tmp), tmp);
				break;
			}

			case F32_TYPE:
			{
				JNIF32Field tmp = (JNIF32Field) Field.getInternal(field);
				result = gmsecJNI.Message_AddField(swigCPtr, this, JNIF32Field.getCPtr(tmp), tmp);
				break;
			}

			case F64_TYPE:
			{
				JNIF64Field tmp = (JNIF64Field) Field.getInternal(field);
				result = gmsecJNI.Message_AddField(swigCPtr, this, JNIF64Field.getCPtr(tmp), tmp);
				break;
			}

			case I8_TYPE:
			{
				JNII8Field tmp = (JNII8Field) Field.getInternal(field);
				result = gmsecJNI.Message_AddField(swigCPtr, this, JNII8Field.getCPtr(tmp), tmp);
				break;
			}

			case I16_TYPE:
			{
				JNII16Field tmp = (JNII16Field) Field.getInternal(field);
				result = gmsecJNI.Message_AddField(swigCPtr, this, JNII16Field.getCPtr(tmp), tmp);
				break;
			}

			case I32_TYPE:
			{
				JNII32Field tmp = (JNII32Field) Field.getInternal(field);
				result = gmsecJNI.Message_AddField(swigCPtr, this, JNII32Field.getCPtr(tmp), tmp);
				break;
			}

			case I64_TYPE:
			{
				JNII64Field tmp = (JNII64Field) Field.getInternal(field);
				result = gmsecJNI.Message_AddField(swigCPtr, this, JNII64Field.getCPtr(tmp), tmp);
				break;
			}

			case U8_TYPE:
			{
				JNIU8Field tmp = (JNIU8Field) Field.getInternal(field);
				result = gmsecJNI.Message_AddField(swigCPtr, this, JNIU8Field.getCPtr(tmp), tmp);
				break;
			}

			case U16_TYPE:
			{
				JNIU16Field tmp = (JNIU16Field) Field.getInternal(field);
				result = gmsecJNI.Message_AddField(swigCPtr, this, JNIU16Field.getCPtr(tmp), tmp);
				break;
			}

			case U32_TYPE:
			{
				JNIU32Field tmp = (JNIU32Field) Field.getInternal(field);
				result = gmsecJNI.Message_AddField(swigCPtr, this, JNIU32Field.getCPtr(tmp), tmp);
				break;
			}

			case U64_TYPE:
			{
				JNIU64Field tmp = (JNIU64Field) Field.getInternal(field);
				result = gmsecJNI.Message_AddField(swigCPtr, this, JNIU64Field.getCPtr(tmp), tmp);
				break;
			}

			case STRING_TYPE:
			{
				JNIStringField tmp = (JNIStringField) Field.getInternal(field);
				result = gmsecJNI.Message_AddField(swigCPtr, this, JNIStringField.getCPtr(tmp), tmp);
				break;
			}

			default:
				// We should never be here, but just in case.
				result = false;
				break;
		}

		return result;
	}


	public boolean addField(String name, byte[] data)
	{
		return gmsecJNI.Message_AddFieldByteArray(swigCPtr, this, name, data);
	}


	public boolean addField(String name, boolean data)
	{
		return gmsecJNI.Message_AddFieldBoolean(swigCPtr, this, name, data);
	}


	public boolean addField(String name, char data)
	{
		return gmsecJNI.Message_AddFieldChar(swigCPtr, this, name, data);
	}


	public boolean addField(String name, byte data)
	{
		return gmsecJNI.Message_AddFieldByte(swigCPtr, this, name, data);
	}


	public boolean addField(String name, short data)
	{
		return gmsecJNI.Message_AddFieldShort(swigCPtr, this, name, data);
	}


	public boolean addField(String name, int data)
	{
		return gmsecJNI.Message_AddFieldInt(swigCPtr, this, name, data);
	}


	public boolean addField(String name, long data)
	{
		return gmsecJNI.Message_AddFieldLong(swigCPtr, this, name, data);
	}


	public boolean addField(String name, float data)
	{
		return gmsecJNI.Message_AddFieldFloat(swigCPtr, this, name, data);
	}


	public boolean addField(String name, double data)
	{
		return gmsecJNI.Message_AddFieldDouble(swigCPtr, this, name, (double) data);
	}


	public boolean addField(String name, String data)
	{
		return gmsecJNI.Message_AddFieldString(swigCPtr, this, name, data);
	}


	public boolean addField(String name, U8 data)
	{
		return addField(new U8Field(name, data));
	}


	public boolean addField(String name, U16 data)
	{
		return addField(new U16Field(name, data));
	}


	public boolean addField(String name, U32 data)
	{
		return addField(new U32Field(name, data));
	}


	public boolean addField(String name, U64 data)
	{
		return addField(new U64Field(name, data));
	}


	public boolean addFields(Collection<Field> fields)
	{
		boolean fieldReplaced = false;

		for (Field field : fields)
		{
			if (field != null)
			{
				fieldReplaced |= addField(field);
			}
		}

		return fieldReplaced;
	}


	public void clearFields()
	{
		gmsecJNI.Message_ClearFields(swigCPtr, this);
	}


	public boolean clearField(String name)
	{
		return gmsecJNI.Message_ClearField(swigCPtr, this, name);
	}


	public long getIntegerValue(String fieldName) throws GMSEC_Exception
	{
		return gmsecJNI.Message_GetIntegerValue(swigCPtr, this, fieldName);
	}


	public U64 getUnsignedIntegerValue(String fieldName) throws GMSEC_Exception
	{
		return gmsecJNI.Message_GetUnsignedIntegerValue(swigCPtr, this, fieldName);
	}


	public double getDoubleValue(String fieldName) throws GMSEC_Exception
	{
		return gmsecJNI.Message_GetDoubleValue(swigCPtr, this, fieldName);
	}


	public String getStringValue(String fieldName) throws GMSEC_Exception
	{
		return gmsecJNI.Message_GetStringValue(swigCPtr, this, fieldName);
	}


	public Field getField(String name)
	{
		long cPtr = gmsecJNI.Message_GetField(swigCPtr, this, name);

		return JNIFieldConverter.cloneField(cPtr);
	}


	public Field.FieldType getFieldType(String name) throws GMSEC_Exception
	{
		int intType = gmsecJNI.Message_GetFieldType(swigCPtr, this, name);

		return Field.FieldType.values()[intType];
	}


	public BinaryField getBinaryField(String name) throws GMSEC_Exception
	{
		long CPtr = gmsecJNI.Message_GetBinaryField(swigCPtr, this, name);

		return new BinaryField( new JNIBinaryField(CPtr, false) );
	}


	public BooleanField getBooleanField(String name) throws GMSEC_Exception
	{
		long CPtr = gmsecJNI.Message_GetBooleanField(swigCPtr, this, name);

		return new BooleanField( new JNIBooleanField(CPtr, false) );
	}


	public CharField getCharField(String name) throws GMSEC_Exception
	{
		long CPtr = gmsecJNI.Message_GetCharField(swigCPtr, this, name);

		return new CharField( new JNICharField(CPtr, false) );
	}


	public F32Field getF32Field(String name) throws GMSEC_Exception
	{
		long CPtr = gmsecJNI.Message_GetF32Field(swigCPtr, this, name);

		return new F32Field( new JNIF32Field(CPtr, false) );
	}


	public F64Field getF64Field(String name) throws GMSEC_Exception
	{
		long CPtr = gmsecJNI.Message_GetF64Field(swigCPtr, this, name);

		return new F64Field( new JNIF64Field(CPtr, false) );
	}


	public I8Field getI8Field(String name) throws GMSEC_Exception
	{
		long CPtr = gmsecJNI.Message_GetI8Field(swigCPtr, this, name);

		return new I8Field( new JNII8Field(CPtr, false) );
	}


	public I16Field getI16Field(String name) throws GMSEC_Exception
	{
		long CPtr = gmsecJNI.Message_GetI16Field(swigCPtr, this, name);

		return new I16Field( new JNII16Field(CPtr, false) );
	}


	public I32Field getI32Field(String name) throws GMSEC_Exception
	{
		long CPtr = gmsecJNI.Message_GetI32Field(swigCPtr, this, name);

		return new I32Field( new JNII32Field(CPtr, false) );
	}


	public I64Field getI64Field(String name) throws GMSEC_Exception
	{
		long CPtr = gmsecJNI.Message_GetI64Field(swigCPtr, this, name);

		return new I64Field( new JNII64Field(CPtr, false) );
	}


	public U8Field getU8Field(String name) throws GMSEC_Exception
	{
		long CPtr = gmsecJNI.Message_GetU8Field(swigCPtr, this, name);

		return new U8Field( new JNIU8Field(CPtr, false) );
	}


	public U16Field getU16Field(String name) throws GMSEC_Exception
	{
		long CPtr = gmsecJNI.Message_GetU16Field(swigCPtr, this, name);

		return new U16Field( new JNIU16Field(CPtr, false) );
	}


	public U32Field getU32Field(String name) throws GMSEC_Exception
	{
		long CPtr = gmsecJNI.Message_GetU32Field(swigCPtr, this, name);

		return new U32Field( new JNIU32Field(CPtr, false) );
	}


	public U64Field getU64Field(String name) throws GMSEC_Exception
	{
		long CPtr = gmsecJNI.Message_GetU64Field(swigCPtr, this, name);

		return new U64Field( new JNIU64Field(CPtr, false) );
	}


	public StringField getStringField(String name) throws GMSEC_Exception
	{
		long CPtr = gmsecJNI.Message_GetStringField(swigCPtr, this, name);

		return new StringField( new JNIStringField(CPtr, false) );
	}


	public int getFieldCount()
	{
		return gmsecJNI.Message_GetFieldCount(swigCPtr, this);
	}


	public void	copyFields(JNIMessage toMsg)
	{
		gmsecJNI.Message_CopyFields(swigCPtr, this, JNIMessage.getCPtr(toMsg), toMsg);
	}


	public String toXML()
	{
		return gmsecJNI.Message_ToXML(swigCPtr, this);
	}


	public String toJSON()
	{
		return gmsecJNI.Message_ToJSON(swigCPtr, this);
	}


	public long getSize()
	{
		return gmsecJNI.Message_GetSize(swigCPtr, this);
	}


	public MessageFieldIterator getFieldIterator()
	{
		long cPtr = gmsecJNI.Message_GetFieldIterator(swigCPtr, this, MessageFieldIterator.Selector.ALL_FIELDS.ordinal());

		return new MessageFieldIterator(new JNIMessageFieldIterator(cPtr, false));
	}


	public MessageFieldIterator getFieldIterator(MessageFieldIterator.Selector selector)
	{
		long cPtr = gmsecJNI.Message_GetFieldIterator(swigCPtr, this, selector.ordinal());

		return new MessageFieldIterator(new JNIMessageFieldIterator(cPtr, false));
	}
}
