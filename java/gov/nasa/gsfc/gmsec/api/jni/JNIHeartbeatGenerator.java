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

import gov.nasa.gsfc.gmsec.api.jni.mist.ArrayListConverter;

import java.util.Collection;
import java.util.ArrayList;


public class JNIHeartbeatGenerator
{
    protected long    swigCPtr;
    protected boolean swigCMemOwn;


	public JNIHeartbeatGenerator(long cPtr, boolean cMemoryOwn)
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
			gmsecJNI.delete_HeartbeatGenerator(swigCPtr, this);
		}

		swigCPtr    = 0;
		swigCMemOwn = false;
	}


	public static long getCPtr(JNIHeartbeatGenerator obj)
	{
		return (obj == null) ? 0 : obj.swigCPtr;
	}


	public JNIHeartbeatGenerator(Config config, String hbMsgSubject, int hbPubRate)
		throws GMSEC_Exception
	{
		this(gmsecJNI.new_HeartbeatGenerator(JNIConfig.getCPtr(Config.getInternal(config)), Config.getInternal(config),
		                                     hbMsgSubject, hbPubRate), true);
	}


	public JNIHeartbeatGenerator(Config config, String hbMsgSubject, int hbPubRate, Collection<Field> fields)
		throws GMSEC_Exception
	{
		long[]     jFieldPtrs = ArrayListConverter.listToFieldPtrs(new ArrayList<Field>(fields));
		JNIField[] jFields    = ArrayListConverter.listToJNIFields(new ArrayList<Field>(fields));
		int        numFields  = (fields == null ? 0 : fields.size());

		swigCPtr = gmsecJNI.new_HeartbeatGenerator(JNIConfig.getCPtr(Config.getInternal(config)), Config.getInternal(config),
		                                           hbMsgSubject, hbPubRate, jFieldPtrs, jFields, numFields);

		swigCMemOwn = true;
	}


	public boolean start()
	{
		return gmsecJNI.HeartbeatGenerator_Start(swigCPtr, this);
	}


	public boolean stop()
	{
		return gmsecJNI.HeartbeatGenerator_Stop(swigCPtr, this);
	}


	public void destroy()
	{
		try {
			this.finalize();
		}
		catch (Throwable e) {
		}
	}


	public boolean isRunning()
	{
		return gmsecJNI.HeartbeatGenerator_IsRunning(swigCPtr, this);
	}


	public void changePublishRate(int pubRate)
	{
		gmsecJNI.HeartbeatGenerator_ChangePublishRate(swigCPtr, this, pubRate);
	}


	public boolean setField(Field field)
		throws GMSEC_Exception
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
				result = gmsecJNI.HeartbeatGenerator_SetField(swigCPtr, this, JNIBinaryField.getCPtr(tmp), tmp);
				break;
			}

			case BOOL_TYPE:
			{
				JNIBooleanField tmp = (JNIBooleanField) Field.getInternal(field);
				result = gmsecJNI.HeartbeatGenerator_SetField(swigCPtr, this, JNIBooleanField.getCPtr(tmp), tmp);
				break;
			}

			case CHAR_TYPE:
			{
				JNICharField tmp = (JNICharField) Field.getInternal(field);
				result = gmsecJNI.HeartbeatGenerator_SetField(swigCPtr, this, JNICharField.getCPtr(tmp), tmp);
				break;
			}

			case F32_TYPE:
			{
				JNIF32Field tmp = (JNIF32Field) Field.getInternal(field);
				result = gmsecJNI.HeartbeatGenerator_SetField(swigCPtr, this, JNIF32Field.getCPtr(tmp), tmp);
				break;
			}

			case F64_TYPE:
			{
				JNIF64Field tmp = (JNIF64Field) Field.getInternal(field);
				result = gmsecJNI.HeartbeatGenerator_SetField(swigCPtr, this, JNIF64Field.getCPtr(tmp), tmp);
				break;
			}

			case I8_TYPE:
			{
				JNII8Field tmp = (JNII8Field) Field.getInternal(field);
				result = gmsecJNI.HeartbeatGenerator_SetField(swigCPtr, this, JNII8Field.getCPtr(tmp), tmp);
				break;
			}

			case I16_TYPE:
			{
				JNII16Field tmp = (JNII16Field) Field.getInternal(field);
				result = gmsecJNI.HeartbeatGenerator_SetField(swigCPtr, this, JNII16Field.getCPtr(tmp), tmp);
				break;
			}

			case I32_TYPE:
			{
				JNII32Field tmp = (JNII32Field) Field.getInternal(field);
				result = gmsecJNI.HeartbeatGenerator_SetField(swigCPtr, this, JNII32Field.getCPtr(tmp), tmp);
				break;
			}

			case I64_TYPE:
			{
				JNII64Field tmp = (JNII64Field) Field.getInternal(field);
				result = gmsecJNI.HeartbeatGenerator_SetField(swigCPtr, this, JNII64Field.getCPtr(tmp), tmp);
				break;
			}

			case U8_TYPE:
			{
				JNIU8Field tmp = (JNIU8Field) Field.getInternal(field);
				result = gmsecJNI.HeartbeatGenerator_SetField(swigCPtr, this, JNIU8Field.getCPtr(tmp), tmp);
				break;
			}

			case U16_TYPE:
			{
				JNIU16Field tmp = (JNIU16Field) Field.getInternal(field);
				result = gmsecJNI.HeartbeatGenerator_SetField(swigCPtr, this, JNIU16Field.getCPtr(tmp), tmp);
				break;
			}

			case U32_TYPE:
			{
				JNIU32Field tmp = (JNIU32Field) Field.getInternal(field);
				result = gmsecJNI.HeartbeatGenerator_SetField(swigCPtr, this, JNIU32Field.getCPtr(tmp), tmp);
				break;
			}

			case U64_TYPE:
			{
				JNIU64Field tmp = (JNIU64Field) Field.getInternal(field);
				result = gmsecJNI.HeartbeatGenerator_SetField(swigCPtr, this, JNIU64Field.getCPtr(tmp), tmp);
				break;
			}

			case STRING_TYPE:
			{
				JNIStringField tmp = (JNIStringField) Field.getInternal(field);
				result = gmsecJNI.HeartbeatGenerator_SetField(swigCPtr, this, JNIStringField.getCPtr(tmp), tmp);
				break;
			}

			default:
				// We should never be here, but just in case.
				result = false;
				break;
		}

		return result;
	} 


	public boolean setField(String fieldName, long fieldValue)
		throws GMSEC_Exception
	{
		return gmsecJNI.HeartbeatGenerator_SetField(swigCPtr, this, fieldName, fieldValue);
	}


	public boolean setField(String fieldName, double fieldValue)
		throws GMSEC_Exception
	{
		return gmsecJNI.HeartbeatGenerator_SetField(swigCPtr, this, fieldName, fieldValue);
	}


	public boolean setField(String fieldName, String fieldValue)
		throws GMSEC_Exception
	{
		return gmsecJNI.HeartbeatGenerator_SetField(swigCPtr, this, fieldName, fieldValue);
	}
}
