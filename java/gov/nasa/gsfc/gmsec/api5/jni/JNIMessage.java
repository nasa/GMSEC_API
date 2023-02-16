/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api5.jni;

import gov.nasa.gsfc.gmsec.api5.*;
import gov.nasa.gsfc.gmsec.api5.field.*;

import gov.nasa.gsfc.gmsec.api5.jni.field.JNIField;
import gov.nasa.gsfc.gmsec.api5.jni.field.JNIBinaryField;
import gov.nasa.gsfc.gmsec.api5.jni.field.JNIBooleanField;
import gov.nasa.gsfc.gmsec.api5.jni.field.JNICharField;
import gov.nasa.gsfc.gmsec.api5.jni.field.JNII16Field;
import gov.nasa.gsfc.gmsec.api5.jni.field.JNII32Field;
import gov.nasa.gsfc.gmsec.api5.jni.field.JNII64Field;
import gov.nasa.gsfc.gmsec.api5.jni.field.JNII8Field;
import gov.nasa.gsfc.gmsec.api5.jni.field.JNIF32Field;
import gov.nasa.gsfc.gmsec.api5.jni.field.JNIF64Field;
import gov.nasa.gsfc.gmsec.api5.jni.field.JNIStringField;
import gov.nasa.gsfc.gmsec.api5.jni.field.JNIU16Field;
import gov.nasa.gsfc.gmsec.api5.jni.field.JNIU32Field;
import gov.nasa.gsfc.gmsec.api5.jni.field.JNIU64Field;
import gov.nasa.gsfc.gmsec.api5.jni.field.JNIU8Field;
import gov.nasa.gsfc.gmsec.api5.jni.field.JNIFieldConverter;

import java.util.Collection;


public class JNIMessage
{
	protected long    swigCPtr;
	protected boolean swigCMemOwn;

	private MessageValidator validator;


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


	protected void dispose()
	{
		swigCPtr    = 0;
		swigCMemOwn = false;
	}


	public synchronized void delete()
	{
		if (swigCPtr != 0 && swigCMemOwn)
		{
			gmsecJNI.delete_Message(swigCPtr, this);
		}

		dispose();
	}


	public static long getCPtr(JNIMessage obj)
	{
		return (obj == null) ? 0 : obj.swigCPtr;
	}


	public JNIMessage()
	{
		this(gmsecJNI.new_Message(), true);
	}


	public JNIMessage(JNIMessage other)
	{
		this(gmsecJNI.new_Message_Copy(JNIMessage.getCPtr(other), other), true);
	}


	public void acknowledge()
	{
		gmsecJNI.Message_Acknowledge(swigCPtr, this);
	}


	public static void destroy(Message msg)
	{
		JNIMessage jMsg = Message.getInternal(msg);

		gmsecJNI.Message_Destroy(JNIMessage.getCPtr(jMsg), jMsg);

		jMsg.dispose();
	}


	public String getSchemaID()
	{
		return gmsecJNI.Message_GetSchemaID(swigCPtr, this);
	}


	public int getVersion()
	{
		return gmsecJNI.Message_GetVersion(swigCPtr, this);
	}


	public Specification.SchemaLevel getSchemaLevel()
	{
		int level = gmsecJNI.Message_GetSchemaLevel(swigCPtr, this);

		return Specification.SchemaLevel.valueOf(level);
	}


	public Status isCompliant()
	{
		return (Status) gmsecJNI.Message_IsCompliant(swigCPtr, this);
	}


	public void registerMessageValidator(MessageValidator val)
	{
		long valPtr = JNIMessageValidator.getCPtr(MessageValidator.getInternal(val));

		gmsecJNI.Message_RegisterMessageValidator(swigCPtr, this, valPtr);

		// We store a handle to the MessageValidator (in case the user has declared it
		// as an anonymous object) so that the JVM garbage collector does not dispose of it after it
		// has been sent over the JNI layer.
		validator = val;
	}


	public boolean setFieldValue(String fieldName, String value)
	{
    	return gmsecJNI.MessageSetFieldValueString(swigCPtr, this, fieldName, value);
	}


	public boolean setFieldValue(String fieldName, int value)
	{
    	return gmsecJNI.MessageSetFieldValueInt(swigCPtr, this, fieldName, value);
	}


	public boolean setFieldValue(String fieldName, long value)
	{
    	return gmsecJNI.MessageSetFieldValueLong(swigCPtr, this, fieldName, value);
	}


	public boolean setFieldValue(String fieldName, double value)
	{
    	return gmsecJNI.MessageSetFieldValueDouble(swigCPtr, this, fieldName, value);
	}


	public void setConfig(Config config)
	{
		JNIConfig cfg = new JNIConfig(config);

		gmsecJNI.Message_SetConfig(swigCPtr, this, JNIConfig.getCPtr(cfg), cfg);
	}


	public Config getConfig()
	{
		long cPtr = gmsecJNI.Message_GetConfig(swigCPtr, this);

		return new Config(new JNIConfig(cPtr, false));
	}


	public void setSubject(String subject)
	{
		gmsecJNI.Message_SetSubject(swigCPtr, this, subject);
	}


	public String getSubject()
	{
		return gmsecJNI.Message_GetSubject(swigCPtr, this);
	}


	public void setKind(Message.Kind kind)
	{
		gmsecJNI.Message_SetKind(swigCPtr, this, kind.getValue());
	}


	public Message.Kind getKind()
	{
		int kind = gmsecJNI.Message_GetKind(swigCPtr, this);

        return Message.Kind.valueOf(kind);
	}


	public boolean addField(Field field)
	{
		// By deducing the Field type, we can then perform a legit cast to the
		// appropriate JNI-Field object, so that we can acquire the underlying
		// address associated with the C++ Field, and then call AddField().
		//
		Field.Type type = field.getType();

		boolean result = false;

		switch (type)
		{
			case BINARY:
			{
				JNIBinaryField tmp = (JNIBinaryField) Field.getInternal(field);
				result = gmsecJNI.Message_AddField(swigCPtr, this, JNIBinaryField.getCPtr(tmp), tmp);
				break;
			}

			case BOOL:
			{
				JNIBooleanField tmp = (JNIBooleanField) Field.getInternal(field);
				result = gmsecJNI.Message_AddField(swigCPtr, this, JNIBooleanField.getCPtr(tmp), tmp);
				break;
			}

			case CHAR:
			{
				JNICharField tmp = (JNICharField) Field.getInternal(field);
				result = gmsecJNI.Message_AddField(swigCPtr, this, JNICharField.getCPtr(tmp), tmp);
				break;
			}

			case F32:
			{
				JNIF32Field tmp = (JNIF32Field) Field.getInternal(field);
				result = gmsecJNI.Message_AddField(swigCPtr, this, JNIF32Field.getCPtr(tmp), tmp);
				break;
			}

			case F64:
			{
				JNIF64Field tmp = (JNIF64Field) Field.getInternal(field);
				result = gmsecJNI.Message_AddField(swigCPtr, this, JNIF64Field.getCPtr(tmp), tmp);
				break;
			}

			case I8:
			{
				JNII8Field tmp = (JNII8Field) Field.getInternal(field);
				result = gmsecJNI.Message_AddField(swigCPtr, this, JNII8Field.getCPtr(tmp), tmp);
				break;
			}

			case I16:
			{
				JNII16Field tmp = (JNII16Field) Field.getInternal(field);
				result = gmsecJNI.Message_AddField(swigCPtr, this, JNII16Field.getCPtr(tmp), tmp);
				break;
			}

			case I32:
			{
				JNII32Field tmp = (JNII32Field) Field.getInternal(field);
				result = gmsecJNI.Message_AddField(swigCPtr, this, JNII32Field.getCPtr(tmp), tmp);
				break;
			}

			case I64:
			{
				JNII64Field tmp = (JNII64Field) Field.getInternal(field);
				result = gmsecJNI.Message_AddField(swigCPtr, this, JNII64Field.getCPtr(tmp), tmp);
				break;
			}

			case U8:
			{
				JNIU8Field tmp = (JNIU8Field) Field.getInternal(field);
				result = gmsecJNI.Message_AddField(swigCPtr, this, JNIU8Field.getCPtr(tmp), tmp);
				break;
			}

			case U16:
			{
				JNIU16Field tmp = (JNIU16Field) Field.getInternal(field);
				result = gmsecJNI.Message_AddField(swigCPtr, this, JNIU16Field.getCPtr(tmp), tmp);
				break;
			}

			case U32:
			{
				JNIU32Field tmp = (JNIU32Field) Field.getInternal(field);
				result = gmsecJNI.Message_AddField(swigCPtr, this, JNIU32Field.getCPtr(tmp), tmp);
				break;
			}

			case U64:
			{
				JNIU64Field tmp = (JNIU64Field) Field.getInternal(field);
				result = gmsecJNI.Message_AddField(swigCPtr, this, JNIU64Field.getCPtr(tmp), tmp);
				break;
			}

			case STRING:
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
		return gmsecJNI.Message_AddFieldDouble(swigCPtr, this, name, data);
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


	public String getStringValue(String fieldName) throws GmsecException
	{
		return gmsecJNI.Message_GetStringValue(swigCPtr, this, fieldName);
	}


	public boolean getBooleanValue(String fieldName) throws GmsecException
	{
		return gmsecJNI.Message_GetBooleanValue(swigCPtr, this, fieldName);
	}


	public short getI16Value(String fieldName) throws GmsecException
	{
		return gmsecJNI.Message_GetI16Value(swigCPtr, this, fieldName);
	}


	public int getI32Value(String fieldName) throws GmsecException
	{
		return gmsecJNI.Message_GetI32Value(swigCPtr, this, fieldName);
	}


	public long getI64Value(String fieldName) throws GmsecException
	{
		return gmsecJNI.Message_GetI64Value(swigCPtr, this, fieldName);
	}


	public U16 getU16Value(String fieldName) throws GmsecException
	{
		return gmsecJNI.Message_GetU16Value(swigCPtr, this, fieldName);
	}


	public U32 getU32Value(String fieldName) throws GmsecException
	{
		return gmsecJNI.Message_GetU32Value(swigCPtr, this, fieldName);
	}


	public U64 getU64Value(String fieldName) throws GmsecException
	{
		return gmsecJNI.Message_GetU64Value(swigCPtr, this, fieldName);
	}


	public double getF64Value(String fieldName) throws GmsecException
	{
		return gmsecJNI.Message_GetF64Value(swigCPtr, this, fieldName);
	}


	public boolean hasField(String name)
	{
		return gmsecJNI.Message_HasField(swigCPtr, this, name);
	}


	public Field getField(String name)
	{
		long cPtr = gmsecJNI.Message_GetField(swigCPtr, this, name);

		return JNIFieldConverter.cloneField(cPtr);
	}


	public Field.Type getFieldType(String name) throws GmsecException
	{
		int type = gmsecJNI.Message_GetFieldType(swigCPtr, this, name);

		return Field.Type.valueOf(type);
	}


	public BinaryField getBinaryField(String name) throws GmsecException
	{
		long CPtr = gmsecJNI.Message_GetBinaryField(swigCPtr, this, name);

		return new BinaryField( new JNIBinaryField(CPtr, false) );
	}


	public BooleanField getBooleanField(String name) throws GmsecException
	{
		long CPtr = gmsecJNI.Message_GetBooleanField(swigCPtr, this, name);

		return new BooleanField( new JNIBooleanField(CPtr, false) );
	}


	public CharField getCharField(String name) throws GmsecException
	{
		long CPtr = gmsecJNI.Message_GetCharField(swigCPtr, this, name);

		return new CharField( new JNICharField(CPtr, false) );
	}


	public F32Field getF32Field(String name) throws GmsecException
	{
		long CPtr = gmsecJNI.Message_GetF32Field(swigCPtr, this, name);

		return new F32Field( new JNIF32Field(CPtr, false) );
	}


	public F64Field getF64Field(String name) throws GmsecException
	{
		long CPtr = gmsecJNI.Message_GetF64Field(swigCPtr, this, name);

		return new F64Field( new JNIF64Field(CPtr, false) );
	}


	public I8Field getI8Field(String name) throws GmsecException
	{
		long CPtr = gmsecJNI.Message_GetI8Field(swigCPtr, this, name);

		return new I8Field( new JNII8Field(CPtr, false) );
	}


	public I16Field getI16Field(String name) throws GmsecException
	{
		long CPtr = gmsecJNI.Message_GetI16Field(swigCPtr, this, name);

		return new I16Field( new JNII16Field(CPtr, false) );
	}


	public I32Field getI32Field(String name) throws GmsecException
	{
		long CPtr = gmsecJNI.Message_GetI32Field(swigCPtr, this, name);

		return new I32Field( new JNII32Field(CPtr, false) );
	}


	public I64Field getI64Field(String name) throws GmsecException
	{
		long CPtr = gmsecJNI.Message_GetI64Field(swigCPtr, this, name);

		return new I64Field( new JNII64Field(CPtr, false) );
	}


	public U8Field getU8Field(String name) throws GmsecException
	{
		long CPtr = gmsecJNI.Message_GetU8Field(swigCPtr, this, name);

		return new U8Field( new JNIU8Field(CPtr, false) );
	}


	public U16Field getU16Field(String name) throws GmsecException
	{
		long CPtr = gmsecJNI.Message_GetU16Field(swigCPtr, this, name);

		return new U16Field( new JNIU16Field(CPtr, false) );
	}


	public U32Field getU32Field(String name) throws GmsecException
	{
		long CPtr = gmsecJNI.Message_GetU32Field(swigCPtr, this, name);

		return new U32Field( new JNIU32Field(CPtr, false) );
	}


	public U64Field getU64Field(String name) throws GmsecException
	{
		long CPtr = gmsecJNI.Message_GetU64Field(swigCPtr, this, name);

		return new U64Field( new JNIU64Field(CPtr, false) );
	}


	public StringField getStringField(String name) throws GmsecException
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
