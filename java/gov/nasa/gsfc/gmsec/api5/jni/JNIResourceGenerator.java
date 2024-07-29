/*
 * Copyright 2007-2024 United States Government as represented by the
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

import gov.nasa.gsfc.gmsec.api5.jni.ArrayListConverter;

import java.util.Collection;
import java.util.ArrayList;


public class JNIResourceGenerator
{
    protected long    swigCPtr;
    protected boolean swigCMemOwn;


	public JNIResourceGenerator(long cPtr, boolean cMemoryOwn)
	{
		swigCPtr    = cPtr;
		swigCMemOwn = cMemoryOwn;
	}


	protected void finalize() throws Throwable
	{
		delete();
		super.finalize();
	}


	private synchronized void delete()
	{
		if (swigCPtr != 0 && swigCMemOwn)
		{
			gmsecJNI.ResourceGenerator_Destroy(swigCPtr, this);
		}

		swigCPtr    = 0;
		swigCMemOwn = false;
	}


	private static long getCPtr(JNIResourceGenerator obj)
	{
		return (obj == null) ? 0 : obj.swigCPtr;
	}


	public static ResourceGenerator create(Config config, int pubRate, int sampleInterval, int averageInterval, Collection<Field> fields)
		throws GmsecException
	{
		long[]     jFieldPtrs = null;
		JNIField[] jFields    = null;
		int        numFields  = (fields == null ? 0 : fields.size());

		if (numFields > 0)
		{
			jFieldPtrs = ArrayListConverter.listToFieldPtrs(new ArrayList<Field>(fields));
			jFields    = ArrayListConverter.listToJNIFields(new ArrayList<Field>(fields));
		}

		long cPtr = gmsecJNI.ResourceGenerator_Create(JNIConfig.getCPtr(Config.getInternal(config)), Config.getInternal(config),
		                                              pubRate, sampleInterval, averageInterval, jFieldPtrs, jFields, numFields);

		return new ResourceGenerator( new JNIResourceGenerator(cPtr, true) );
	}


	public static void destroy(ResourceGenerator rsrcGen)
	{
		JNIResourceGenerator jRsrcGen = ResourceGenerator.getInternal(rsrcGen);

		gmsecJNI.ResourceGenerator_Destroy(JNIResourceGenerator.getCPtr(jRsrcGen), jRsrcGen);

		jRsrcGen.swigCPtr    = 0;
		jRsrcGen.swigCMemOwn = false;
	}


	public boolean start()
		throws GmsecException
	{
		return gmsecJNI.ResourceGenerator_Start(swigCPtr, this);
	}


	public boolean stop()
	{
		return gmsecJNI.ResourceGenerator_Stop(swigCPtr, this);
	}


	public boolean isRunning()
	{
		return gmsecJNI.ResourceGenerator_IsRunning(swigCPtr, this);
	}


	public boolean setField(Field field)
		throws GmsecException
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
				result = gmsecJNI.ResourceGenerator_SetField(swigCPtr, this, JNIBinaryField.getCPtr(tmp), tmp);
				break;
			}

			case BOOL:
			{
				JNIBooleanField tmp = (JNIBooleanField) Field.getInternal(field);
				result = gmsecJNI.ResourceGenerator_SetField(swigCPtr, this, JNIBooleanField.getCPtr(tmp), tmp);
				break;
			}

			case CHAR:
			{
				JNICharField tmp = (JNICharField) Field.getInternal(field);
				result = gmsecJNI.ResourceGenerator_SetField(swigCPtr, this, JNICharField.getCPtr(tmp), tmp);
				break;
			}

			case F32:
			{
				JNIF32Field tmp = (JNIF32Field) Field.getInternal(field);
				result = gmsecJNI.ResourceGenerator_SetField(swigCPtr, this, JNIF32Field.getCPtr(tmp), tmp);
				break;
			}

			case F64:
			{
				JNIF64Field tmp = (JNIF64Field) Field.getInternal(field);
				result = gmsecJNI.ResourceGenerator_SetField(swigCPtr, this, JNIF64Field.getCPtr(tmp), tmp);
				break;
			}

			case I8:
			{
				JNII8Field tmp = (JNII8Field) Field.getInternal(field);
				result = gmsecJNI.ResourceGenerator_SetField(swigCPtr, this, JNII8Field.getCPtr(tmp), tmp);
				break;
			}

			case I16:
			{
				JNII16Field tmp = (JNII16Field) Field.getInternal(field);
				result = gmsecJNI.ResourceGenerator_SetField(swigCPtr, this, JNII16Field.getCPtr(tmp), tmp);
				break;
			}

			case I32:
			{
				JNII32Field tmp = (JNII32Field) Field.getInternal(field);
				result = gmsecJNI.ResourceGenerator_SetField(swigCPtr, this, JNII32Field.getCPtr(tmp), tmp);
				break;
			}

			case I64:
			{
				JNII64Field tmp = (JNII64Field) Field.getInternal(field);
				result = gmsecJNI.ResourceGenerator_SetField(swigCPtr, this, JNII64Field.getCPtr(tmp), tmp);
				break;
			}

			case U8:
			{
				JNIU8Field tmp = (JNIU8Field) Field.getInternal(field);
				result = gmsecJNI.ResourceGenerator_SetField(swigCPtr, this, JNIU8Field.getCPtr(tmp), tmp);
				break;
			}

			case U16:
			{
				JNIU16Field tmp = (JNIU16Field) Field.getInternal(field);
				result = gmsecJNI.ResourceGenerator_SetField(swigCPtr, this, JNIU16Field.getCPtr(tmp), tmp);
				break;
			}

			case U32:
			{
				JNIU32Field tmp = (JNIU32Field) Field.getInternal(field);
				result = gmsecJNI.ResourceGenerator_SetField(swigCPtr, this, JNIU32Field.getCPtr(tmp), tmp);
				break;
			}

			case U64:
			{
				JNIU64Field tmp = (JNIU64Field) Field.getInternal(field);
				result = gmsecJNI.ResourceGenerator_SetField(swigCPtr, this, JNIU64Field.getCPtr(tmp), tmp);
				break;
			}

			case STRING:
			{
				JNIStringField tmp = (JNIStringField) Field.getInternal(field);
				result = gmsecJNI.ResourceGenerator_SetField(swigCPtr, this, JNIStringField.getCPtr(tmp), tmp);
				break;
			}

			default:
				// We should never be here, but just in case.
				result = false;
				break;
		}

		return result;
	} 


    public static Message createResourceMessage(MessageFactory factory, int sampleInterval, int averageInterval)
	{
		JNIMessageFactory intMsgFactory = MessageFactory.getInternal(factory);

		long cptr = gmsecJNI.ResourceGenerator_CreateResourceMessage( JNIMessageFactory.getCPtr(intMsgFactory), intMsgFactory, sampleInterval, averageInterval);

		return new Message( new JNIMessage(cptr, true) );
	}
}
