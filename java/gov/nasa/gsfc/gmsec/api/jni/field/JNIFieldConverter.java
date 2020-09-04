package gov.nasa.gsfc.gmsec.api.jni;

import gov.nasa.gsfc.gmsec.api.field.*;
import gov.nasa.gsfc.gmsec.api.jni.*;


public class JNIFieldConverter
{
	public static Field createFieldReference(long cPtr)
	{
		Field field = null;

		if (cPtr != 0)
		{
			int intType = gmsecJNI.Field_GetType(cPtr, null);

			Field.FieldType type = Field.FieldType.values()[intType];

			switch (type)
			{
				case BIN_TYPE:    field = new BinaryField(new JNIBinaryField(cPtr, false)); break;
				case BOOL_TYPE:   field = new BooleanField(new JNIBooleanField(cPtr, false)); break;
				case CHAR_TYPE:   field = new CharField(new JNICharField(cPtr, false)); break;
				case F32_TYPE:    field = new F32Field(new JNIF32Field(cPtr, false)); break;
				case F64_TYPE:    field = new F64Field(new JNIF64Field(cPtr, false)); break;
				case I8_TYPE:     field = new I8Field(new JNII8Field(cPtr, false)); break;
				case I16_TYPE:    field = new I16Field(new JNII16Field(cPtr, false)); break;
				case I32_TYPE:    field = new I32Field(new JNII32Field(cPtr, false)); break;
				case I64_TYPE:    field = new I64Field(new JNII64Field(cPtr, false)); break;
				case U8_TYPE:     field = new U8Field(new JNIU8Field(cPtr, false)); break;
				case U16_TYPE:    field = new U16Field(new JNIU16Field(cPtr, false)); break;
				case U32_TYPE:    field = new U32Field(new JNIU32Field(cPtr, false)); break;
				case U64_TYPE:    field = new U64Field(new JNIU64Field(cPtr, false)); break;
				case STRING_TYPE: field = new StringField(new JNIStringField(cPtr, false)); break;
			}
		}

		return field;
	}
}
