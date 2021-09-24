/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file FieldConverter.java
 */

package gov.nasa.gsfc.gmsec.api.jni.field;


import gov.nasa.gsfc.gmsec.api.field.*;

import gov.nasa.gsfc.gmsec.api.jni.gmsecJNI;
import gov.nasa.gsfc.gmsec.api.jni.field.*;


public class JNIFieldConverter
{
	public static Field cloneField(long cPtr)
	{
		Field field = null;

		if (cPtr != 0)
		{
			int intType = gmsecJNI.Field_GetType(cPtr, null);

			Field.FieldType type = Field.FieldType.values()[intType];

			// Determine the field type, and return a copy of the field.
			//
			switch (type)
			{
				case STRING_TYPE:
				{
					StringField tmp = new StringField(new JNIStringField(cPtr, false));
					field = new StringField(tmp.getName(), tmp.getValue());
					break;
				}

				case BIN_TYPE:
				{
					BinaryField tmp = new BinaryField(new JNIBinaryField(cPtr, false));
					field = new BinaryField(tmp.getName(), tmp.getValue());
					break;
				}

				case BOOL_TYPE:
				{
					BooleanField tmp = new BooleanField(new JNIBooleanField(cPtr, false));
					field = new BooleanField(tmp.getName(), tmp.getValue());
					break;
				}

				case F32_TYPE:
				{
					F32Field tmp = new F32Field(new JNIF32Field(cPtr, false));
					field = new F32Field(tmp.getName(), tmp.getValue());
					break;
				}

				case F64_TYPE:
				{
					F64Field tmp = new F64Field(new JNIF64Field(cPtr, false));
					field = new F64Field(tmp.getName(), tmp.getValue());
					break;
				}

				case I16_TYPE:
				{
					I16Field tmp = new I16Field(new JNII16Field(cPtr, false));
					field = new I16Field(tmp.getName(), tmp.getValue());
					break;
				}

				case I32_TYPE:
				{
					I32Field tmp = new I32Field(new JNII32Field(cPtr, false));
					field = new I32Field(tmp.getName(), tmp.getValue());
					break;
				}

				case U16_TYPE:
				{
					U16Field tmp = new U16Field(new JNIU16Field(cPtr, false));
					field = new U16Field(tmp.getName(), tmp.getValue());
					break;
				}

				case U32_TYPE:
				{
					U32Field tmp = new U32Field(new JNIU32Field(cPtr, false));
					field = new U32Field(tmp.getName(), tmp.getValue());
					break;
				}

				case CHAR_TYPE:
				{
					CharField tmp = new CharField(new JNICharField(cPtr, false));
					field = new CharField(tmp.getName(), tmp.getValue());
					break;
				}

				case I8_TYPE:
				{
					I8Field tmp = new I8Field(new JNII8Field(cPtr, false));
					field = new I8Field(tmp.getName(), tmp.getValue());
					break;
				}

				case U8_TYPE:
				{
					U8Field tmp = new U8Field(new JNIU8Field(cPtr, false));
					field = new U8Field(tmp.getName(), tmp.getValue());
					break;
				}

				case I64_TYPE:
				{
					I64Field tmp = new I64Field(new JNII64Field(cPtr, false));
					field = new I64Field(tmp.getName(), tmp.getValue());
					break;
				}

				case U64_TYPE:
				{
					U64Field tmp = new U64Field(new JNIU64Field(cPtr, false));
					field = new U64Field(tmp.getName(), tmp.getValue());
					break;
				}
			}

			if (field != null)
			{
				field.isHeader( gmsecJNI.Field_IsHeader(cPtr, null) );
			}
		}

		return field;
	}
}
