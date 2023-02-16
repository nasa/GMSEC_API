/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file FieldConverter.java
 */

package gov.nasa.gsfc.gmsec.api5.jni.field;


import gov.nasa.gsfc.gmsec.api5.field.*;

import gov.nasa.gsfc.gmsec.api5.jni.gmsecJNI;
import gov.nasa.gsfc.gmsec.api5.jni.field.*;


public class JNIFieldConverter
{
	public static Field cloneField(long cPtr)
	{
		Field field = null;

		if (cPtr != 0)
		{
			int intType = gmsecJNI.Field_GetType(cPtr, null);

			Field.Type type = Field.Type.valueOf(intType);

			// Determine the field type, and return a copy of the field.
			//
			switch (type)
			{
				case STRING:
				{
					StringField tmp = new StringField(new JNIStringField(cPtr, false));
					field = new StringField(tmp);
					break;
				}

				case BINARY:
				{
					BinaryField tmp = new BinaryField(new JNIBinaryField(cPtr, false));
					field = new BinaryField(tmp);
					break;
				}

				case BOOL:
				{
					BooleanField tmp = new BooleanField(new JNIBooleanField(cPtr, false));
					field = new BooleanField(tmp);
					break;
				}

				case F32:
				{
					F32Field tmp = new F32Field(new JNIF32Field(cPtr, false));
					field = new F32Field(tmp);
					break;
				}

				case F64:
				{
					F64Field tmp = new F64Field(new JNIF64Field(cPtr, false));
					field = new F64Field(tmp);
					break;
				}

				case I16:
				{
					I16Field tmp = new I16Field(new JNII16Field(cPtr, false));
					field = new I16Field(tmp);
					break;
				}

				case I32:
				{
					I32Field tmp = new I32Field(new JNII32Field(cPtr, false));
					field = new I32Field(tmp);
					break;
				}

				case U16:
				{
					U16Field tmp = new U16Field(new JNIU16Field(cPtr, false));
					field = new U16Field(tmp);
					break;
				}

				case U32:
				{
					U32Field tmp = new U32Field(new JNIU32Field(cPtr, false));
					field = new U32Field(tmp);
					break;
				}

				case CHAR:
				{
					CharField tmp = new CharField(new JNICharField(cPtr, false));
					field = new CharField(tmp);
					break;
				}

				case I8:
				{
					I8Field tmp = new I8Field(new JNII8Field(cPtr, false));
					field = new I8Field(tmp);
					break;
				}

				case U8:
				{
					U8Field tmp = new U8Field(new JNIU8Field(cPtr, false));
					field = new U8Field(tmp);
					break;
				}

				case I64:
				{
					I64Field tmp = new I64Field(new JNII64Field(cPtr, false));
					field = new I64Field(tmp);
					break;
				}

				case U64:
				{
					U64Field tmp = new U64Field(new JNIU64Field(cPtr, false));
					field = new U64Field(tmp);
					break;
				}
			}
		}

		return field;
	}
}
