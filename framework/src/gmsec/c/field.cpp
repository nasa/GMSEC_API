
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/**
 *  @file field.cpp
 *
 *  This file contains the C-wrapper for functions in the Field object.
*/

#include <gmsec/c/field.h>
#include <gmsec/Field.h>

#include <gmsec/internal/Log.h>


using namespace gmsec::util;
using namespace gmsec;

void CALL_TYPE gmsec_CreateField(GMSEC_FIELD_OBJECT *fld, GMSEC_STATUS_OBJECT status)
{
	Status result;
	*fld = (GMSEC_FIELD_OBJECT)new Field();
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_DestroyField(GMSEC_FIELD_OBJECT *fld, GMSEC_STATUS_OBJECT status)
{
	Status result;

	if (NULL == fld)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Field handle is NULL");
                LOG_WARNING << "field_c::DestroyField : " << result.Get();
	}
	else if (NULL == *fld)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Field handle is NULL");
                LOG_WARNING << "field_c::DestroyField : " << result.Get();
            
	}
	else
	{
		Field *tmpFld = (Field *)*fld;
		delete tmpFld;
		*fld = NULL;
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_GetFieldName(GMSEC_FIELD_OBJECT fld, const char **name, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == fld)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Field handle is NULL");
                LOG_WARNING << "field_c::GetFieldName : " << result.Get();
		*name = NULL;
	}
	else
	{
		Field *tmpMsg = (Field *)fld;
		result = tmpMsg->GetName((const char *&)*name);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_SetFieldName(GMSEC_FIELD_OBJECT fld, const char *name, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == fld)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Field handle is NULL");
                 LOG_WARNING << "field_c::SetFieldName : " << result.Get();
	}
	else
	{
		Field *tmpMsg = (Field *)fld;
		result = tmpMsg->SetName(name);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_SetFieldType(GMSEC_FIELD_OBJECT fld, GMSEC_TYPE ftype, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == fld)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Field handle is NULL");
                LOG_WARNING << "field_c::SetFieldType : " << result.Get();
	}
	else
	{
		Field *tmpMsg = (Field *)fld;
		result = tmpMsg->SetType(ftype);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_GetFieldType(GMSEC_FIELD_OBJECT fld, GMSEC_TYPE *ftype, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == fld)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Field handle is NULL");
                LOG_WARNING << "field_c::GetFieldType : " << result.Get();
		*ftype = GMSEC_TYPE_UNSET;
	}
	else
	{
		Field *tmpMsg = (Field *)fld;
		result = tmpMsg->GetType(*ftype);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_GetFieldValueCHAR(GMSEC_FIELD_OBJECT fld, GMSEC_CHAR *value, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == fld)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Field handle is NULL");
                LOG_WARNING << "field_c::GetFieldValueCHAR : " << result.Get();
		*value = 0;
	}
	else
	{
		Field *tmpMsg = (Field *)fld;
		result = tmpMsg->GetValue(*value);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_GetFieldValueBOOL(GMSEC_FIELD_OBJECT fld, GMSEC_BOOL *value, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == fld)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Field handle is NULL");
                LOG_WARNING << "field_c::GetFieldValueBOOL : " << result.Get();
		*value = (GMSEC_BOOL) 0;
	}
	else
	{
		Field *tmpMsg = (Field *)fld;
		result = tmpMsg->GetValue(*value);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_GetFieldValueSHORT(GMSEC_FIELD_OBJECT fld, GMSEC_SHORT *value, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == fld)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Field handle is NULL");
                LOG_WARNING << "field_c::GetFieldValueSHORT : " << result.Get();
		*value = 0;
	}
	else
	{
		Field *tmpMsg = (Field *)fld;
		result = tmpMsg->GetValue(*value);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_GetFieldValueUSHORT(GMSEC_FIELD_OBJECT fld, GMSEC_USHORT *value, GMSEC_STATUS_OBJECT status)
{
	// This method has been deprecated; call the preferred method.
	gmsec_GetFieldValueU16(fld, value, status);
}

void CALL_TYPE gmsec_GetFieldValueLONG(GMSEC_FIELD_OBJECT fld, GMSEC_LONG *value, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == fld)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Field handle is NULL");
                LOG_WARNING << "field_c::GetFieldValueLONG : " << result.Get();
		*value = 0;
	}
	else
	{
		Field *tmpMsg = (Field *)fld;
		result = tmpMsg->GetValue(*value);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_GetFieldValueULONG(GMSEC_FIELD_OBJECT fld, GMSEC_ULONG *value, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == fld)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Field handle is NULL");
                LOG_WARNING << "field_c::GetFieldValueULONG : " << result.Get();
		*value = 0;
	}
	else
	{
		Field *tmpMsg = (Field *)fld;
		result = tmpMsg->GetValue(*value);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_GetFieldValueFLOAT(GMSEC_FIELD_OBJECT fld, GMSEC_FLOAT *value, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == fld)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Field handle is NULL");
                LOG_WARNING << "field_c::GetFieldValueFLOAT : " << result.Get();
		*value = 0;
	}
	else
	{
		Field *tmpMsg = (Field *)fld;
		result = tmpMsg->GetValue(*value);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_GetFieldValueDOUBLE(GMSEC_FIELD_OBJECT fld, GMSEC_DOUBLE *value, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == fld)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Field handle is NULL");
                LOG_WARNING << "field_c::GetFieldValueDOUBLE : " << result.Get();
		*value = 0;
	}
	else
	{
		Field *tmpMsg = (Field *)fld;
		result = tmpMsg->GetValue(*value);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_GetFieldValueSTRING(GMSEC_FIELD_OBJECT fld, GMSEC_STRING *value, GMSEC_STATUS_OBJECT status)
{
	Status result;

	static int warned = 0;
	if (!warned)
	{
		warned = 1;
		LOG_WARNING << "gmsec_GetFieldValueSTRING is obsolete; please use gmsec_GetFieldValueSTR";
	}

	if (NULL == fld)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Field handle is NULL");
                LOG_WARNING << "field_c::GetFieldValueSTRING : " << result.Get();
		*value = NULL;
	}
	else
	{
		Field *tmpMsg = (Field *)fld;
		result = tmpMsg->GetValue(*value);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_GetFieldValueSTR(GMSEC_FIELD_OBJECT fld, GMSEC_STR *value, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == fld)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Field handle is NULL");
                LOG_WARNING << "field_c::GetFieldValueSTR : " << result.Get();
		*value = NULL;
	}
	else
	{
		Field *tmpMsg = (Field *)fld;
		result = tmpMsg->GetValue(*value);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_GetFieldValueBIN(GMSEC_FIELD_OBJECT fld, GMSEC_BIN *value, GMSEC_U32 *size, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == fld)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Field handle is NULL");
                LOG_WARNING << "field_c::GetFieldValueBIN : " << result.Get();
		*value = NULL;
		*size = 0;
	}
	else
	{
		Field *tmpMsg = (Field *)fld;
		result = tmpMsg->GetValue(value,*size);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_GetFieldValueI16(GMSEC_FIELD_OBJECT fld, GMSEC_I16 *value, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == fld)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Field handle is NULL");
                LOG_WARNING << "field_c::GetFieldValueI16 : " << result.Get();
		*value = 0;
	}
	else
	{
		Field *tmpMsg = (Field *)fld;
		result = tmpMsg->GetValue(*value);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_GetFieldValueU16(GMSEC_FIELD_OBJECT fld, GMSEC_U16 *value, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == fld)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Field handle is NULL");
                LOG_WARNING << "field_c::GetFieldValueU16 : " << result.Get();
		*value = 0;
	}
	else
	{
		Field *tmpMsg = (Field *)fld;
		result = tmpMsg->GetValue(*value);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_GetFieldValueI32(GMSEC_FIELD_OBJECT fld, GMSEC_I32 *value, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == fld)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Field handle is NULL");
                LOG_WARNING << "field_c::GetFieldValueI32 : " << result.Get();
		*value = 0;
	}
	else
	{
		Field *tmpMsg = (Field *)fld;
		result = tmpMsg->GetValue(*value);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_GetFieldValueU32(GMSEC_FIELD_OBJECT fld, GMSEC_U32 *value, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == fld)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Field handle is NULL");
                LOG_WARNING << "field_c::GetFieldValueU32 : " << result.Get();
		*value = 0;
	}
	else
	{
		Field *tmpMsg = (Field *)fld;
		result = tmpMsg->GetValue(*value);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_GetFieldValueF32(GMSEC_FIELD_OBJECT fld, GMSEC_F32 *value, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == fld)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Field handle is NULL");
                LOG_WARNING << "field_c::GetFieldValueF32 : " << result.Get();
		*value = 0;
	}
	else
	{
		Field *tmpMsg = (Field *)fld;
		result = tmpMsg->GetValue(*value);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_GetFieldValueF64(GMSEC_FIELD_OBJECT fld, GMSEC_F64 *value, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == fld)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Field handle is NULL");
                LOG_WARNING << "field_c::GetFieldValueF64 : " << result.Get();
		*value = 0;
	}
	else
	{
		Field *tmpMsg = (Field *)fld;
		result = tmpMsg->GetValue(*value);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_GetFieldValueI64(GMSEC_FIELD_OBJECT fld, GMSEC_I64 *value, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == fld)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Field handle is NULL");
                LOG_WARNING << "field_c::GetFieldValueI64 : " << result.Get();
		*value = 0;
	}
	else
	{
		Field *tmpMsg = (Field *)fld;
		result = tmpMsg->GetValue(*value);
	}
	if (NULL != status)
		*((Status *)status) = result;
}


void CALL_TYPE gmsec_SetFieldValueCHAR(GMSEC_FIELD_OBJECT fld, GMSEC_CHAR value, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == fld)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Field handle is NULL");
                LOG_WARNING << "field_c::SetFieldValueCHAR : " << result.Get();
	}
	else
	{
		Field *tmpMsg = (Field *)fld;
		result = tmpMsg->SetValue(value);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_SetFieldValueBOOL(GMSEC_FIELD_OBJECT fld, GMSEC_BOOL value, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == fld)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Field handle is NULL");
                LOG_WARNING << "field_c::SetFieldValueBOOL : " << result.Get();
	}
	else
	{
		Field *tmpMsg = (Field *)fld;
		result = tmpMsg->SetValue(value);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_SetFieldValueSHORT(GMSEC_FIELD_OBJECT fld, GMSEC_SHORT value, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == fld)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Field handle is NULL");
                LOG_WARNING << "field_c::SetFieldValueSHORT : " << result.Get();
	}
	else
	{
		Field *tmpMsg = (Field *)fld;
		result = tmpMsg->SetValue(value);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_SetFieldValueUSHORT(GMSEC_FIELD_OBJECT fld, GMSEC_USHORT value, GMSEC_STATUS_OBJECT status)
{
	// This method has been deprecated; call the preferred method.
	gmsec_SetFieldValueU16(fld, value, status);
}

void CALL_TYPE gmsec_SetFieldValueLONG(GMSEC_FIELD_OBJECT fld, GMSEC_LONG value, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == fld)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Field handle is NULL");
                LOG_WARNING << "field_c::SetFieldValueLONG : " << result.Get();
	}
	else
	{
		Field *tmpMsg = (Field *)fld;
		result = tmpMsg->SetValue(value);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_SetFieldValueULONG(GMSEC_FIELD_OBJECT fld, GMSEC_ULONG value, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == fld)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Field handle is NULL");
                LOG_WARNING << "field_c::SetFieldValueULONG : " << result.Get();
	}
	else
	{
		Field *tmpMsg = (Field *)fld;
		result = tmpMsg->SetValue(value);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_SetFieldValueFLOAT(GMSEC_FIELD_OBJECT fld, GMSEC_FLOAT value, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == fld)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Field handle is NULL");
                LOG_WARNING << "field_c::SetFieldValueFLOAT : " << result.Get();
	}
	else
	{
		Field *tmpMsg = (Field *)fld;
		result = tmpMsg->SetValue(value);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_SetFieldValueDOUBLE(GMSEC_FIELD_OBJECT fld, GMSEC_DOUBLE value, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == fld)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Field handle is NULL");
                LOG_WARNING << "field_c::SetFieldValueDOUBLE : " << result.Get();
	}
	else
	{
		Field *tmpMsg = (Field *)fld;
		result = tmpMsg->SetValue(value);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_SetFieldValueSTRING(GMSEC_FIELD_OBJECT fld, GMSEC_STR value, GMSEC_STATUS_OBJECT status)
{
	return gmsec_SetFieldValueSTR(fld, value, status);
}

void CALL_TYPE gmsec_SetFieldValueSTR(GMSEC_FIELD_OBJECT fld, GMSEC_STR value, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == fld)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Field handle is NULL");
                LOG_WARNING << "field_c::SetFieldValueSTRING : " << result.Get();
	}
	else
	{
		Field *tmpMsg = (Field *)fld;
		result = tmpMsg->SetValue(value);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_SetFieldValueBIN(GMSEC_FIELD_OBJECT fld, GMSEC_BIN value, GMSEC_U32 size, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == fld)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Field handle is NULL");
                LOG_WARNING << "field_c::SetFieldValueBIN : " << result.Get();
	}
	else
	{
		Field *tmpMsg = (Field *)fld;
		result = tmpMsg->SetValue(value,size);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_SetFieldValueI16(GMSEC_FIELD_OBJECT fld, GMSEC_I16 value, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == fld)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Field handle is NULL");
                LOG_WARNING << "field_c::SetFieldValueI16 : " << result.Get();
	}
	else
	{
		Field *tmpMsg = (Field *)fld;
		result = tmpMsg->SetValue(value);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_SetFieldValueU16(GMSEC_FIELD_OBJECT fld, GMSEC_U16 value, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == fld)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Field handle is NULL");
                LOG_WARNING << "field_c::SetFieldValueU16 : " << result.Get();
	}
	else
	{
		Field *tmpMsg = (Field *)fld;
		result = tmpMsg->SetValue(value);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_SetFieldValueI32(GMSEC_FIELD_OBJECT fld, GMSEC_I32 value, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == fld)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Field handle is NULL");
                LOG_WARNING << "field_c::SetFieldValueI32 : " << result.Get();
	}
	else
	{
		Field *tmpMsg = (Field *)fld;
		result = tmpMsg->SetValue(value);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_SetFieldValueU32(GMSEC_FIELD_OBJECT fld, GMSEC_U32 value, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == fld)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Field handle is NULL");
                LOG_WARNING << "field_c::SetFieldValueU32 : " << result.Get();
	}
	else
	{
		Field *tmpMsg = (Field *)fld;
		result = tmpMsg->SetValue(value);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_SetFieldValueF32(GMSEC_FIELD_OBJECT fld, GMSEC_F32 value, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == fld)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Field handle is NULL");
                LOG_WARNING << "field_c::SetFieldValueF32 : " << result.Get();
	}
	else
	{
		Field *tmpMsg = (Field *)fld;
		result = tmpMsg->SetValue(value);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_SetFieldValueF64(GMSEC_FIELD_OBJECT fld, GMSEC_F64 value, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == fld)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Field handle is NULL");
                LOG_WARNING << "field_c::SetFieldValueF32 : " << result.Get();
	}
	else
	{
		Field *tmpMsg = (Field *)fld;
		result = tmpMsg->SetValue(value);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_SetFieldValueI64(GMSEC_FIELD_OBJECT fld, GMSEC_I64 value, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == fld)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Field handle is NULL");
                LOG_WARNING << "field_c::SetFieldValueI64 : " << result.Get();
	}
	else
	{
		Field *tmpMsg = (Field *)fld;
		result = tmpMsg->SetValue(value);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_UnSetField(GMSEC_FIELD_OBJECT fld, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == fld)
	{
		result.Set(GMSEC_STATUS_FIELD_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Field handle is NULL");
                LOG_WARNING << "field_c::UnSetField : " << result.Get();
	}
	else
	{
		Field *tmpMsg = (Field *)fld;
		result = tmpMsg->UnSet();
	}
	if (NULL != status)
		*((Status *)status) = result;
}


GMSEC_TYPE CALL_TYPE gmsec_LookupFieldType(const char *s)
{
	return Field::LookupType(s);
}

