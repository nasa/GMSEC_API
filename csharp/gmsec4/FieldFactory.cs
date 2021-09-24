/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

namespace GMSEC.API
{
    public class FieldFactory
    {
        public static Field BuildField(global::System.IntPtr cPtr, bool owned)
        {
            if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();

            if (cPtr == global::System.IntPtr.Zero) return null;

            Field tmp = new Field(cPtr, owned); // temp object so that we can get field type

            switch (tmp.GetFieldType())
            {
            case Field.FieldType.BOOL_TYPE:
                return new BooleanField(cPtr, owned);

            case Field.FieldType.CHAR_TYPE:
                return new CharField(cPtr, owned);

            case Field.FieldType.I16_TYPE:
                return new I16Field(cPtr, owned);

            case Field.FieldType.I32_TYPE:
                return new I32Field(cPtr, owned);

            case Field.FieldType.I64_TYPE:
                return new I64Field(cPtr, owned);

            case Field.FieldType.I8_TYPE:
                return new I8Field(cPtr, owned);

            case Field.FieldType.F32_TYPE:
                return new F32Field(cPtr, owned);

            case Field.FieldType.F64_TYPE:
                return new F64Field(cPtr, owned);

            case Field.FieldType.STRING_TYPE:
                return new StringField(cPtr, owned);

            case Field.FieldType.BIN_TYPE:
                return new BinaryField(cPtr, owned);

            case Field.FieldType.U16_TYPE:
                return new U16Field(cPtr, owned);

            case Field.FieldType.U32_TYPE:
                return new U32Field(cPtr, owned);

            case Field.FieldType.U64_TYPE:
                return new U64Field(cPtr, owned);

            case Field.FieldType.U8_TYPE:
                return new U8Field(cPtr, owned);
            }

            return tmp;
        }
    }
}

