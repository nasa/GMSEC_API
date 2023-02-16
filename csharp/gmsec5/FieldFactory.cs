/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

namespace GMSEC.API5
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
            case Field.Type.BOOL:
                return new BooleanField(cPtr, owned);

            case Field.Type.CHAR:
                return new CharField(cPtr, owned);

            case Field.Type.I16:
                return new I16Field(cPtr, owned);

            case Field.Type.I32:
                return new I32Field(cPtr, owned);

            case Field.Type.I64:
                return new I64Field(cPtr, owned);

            case Field.Type.I8:
                return new I8Field(cPtr, owned);

            case Field.Type.F32:
                return new F32Field(cPtr, owned);

            case Field.Type.F64:
                return new F64Field(cPtr, owned);

            case Field.Type.STRING:
                return new StringField(cPtr, owned);

            case Field.Type.BINARY:
                return new BinaryField(cPtr, owned);

            case Field.Type.U16:
                return new U16Field(cPtr, owned);

            case Field.Type.U32:
                return new U32Field(cPtr, owned);

            case Field.Type.U64:
                return new U64Field(cPtr, owned);

            case Field.Type.U8:
                return new U8Field(cPtr, owned);
            }

            return tmp;
        }
    }
}

