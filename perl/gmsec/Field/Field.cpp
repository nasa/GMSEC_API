
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/*----------------------------------------------------------------------------
 * Field.cpp
 *
 * Description: Base class for message fields
 *
 *--------------------------------------------------------------------------*/

// GMSEC API and Perl support library
#include "gmsec_perl.h"
#include <gmsec/internal/mathutil.h>


#ifndef PERL_OBJECT
#ifndef MULTIPLICITY
LINKAGE GMSEC_XS_EXPORT(void) boot_GMSECAPI__Field(CV* cv);
#else
LINKAGE GMSEC_XS_EXPORT(void) boot_GMSECAPI__Field(pTHXo_ CV* cv);
#endif
#else
LINKAGE GMSEC_XS_EXPORT(void) boot_GMSECAPI__Field(CV *cv, CPerlObj *);
#endif


GMSEC_XS_STATIC XS(_PGA_Field_Create)
{
   HV *self;
   SV *hvPointer;
   IV pointer;
   GMSEC_TYPE type = 0;
   const char *name = NULL;

   size_t size_bin;
   const char *tmp_char;
   short tmp_short;

   GMSEC_BIN value_bin;
   GMSEC_CHAR value_char;
   GMSEC_BOOL value_bool;
   GMSEC_SHORT value_short;
   GMSEC_U16 value_ushort;
   GMSEC_LONG value_long;
   GMSEC_ULONG value_ulong;
   GMSEC_FLOAT value_float;
   GMSEC_DOUBLE value_double;
   GMSEC_I64 value_i64;
   GMSEC_STRING value_string;

   Field *fld = (Field *) 0;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 1 && items != 4 && items != 5)
	 {
		warn("[GMSECAPI::Field] Usage: Field($self); OR Field($self, $type, $name, $value); OR Field($self, $type, $name, $value, $size);");
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Field"))
	 {
		warn("[GMSECAPI::Field] first argument is not a blessed object");

		XSRETURN_UNDEF;
	 }

	 if (items == 4 || items == 5)
	 {
		type = (GMSEC_TYPE) SvUV(ST(1));
		name = SvPV_nolen(ST(2));
	 }

	 if (items == 5 && type == GMSEC_TYPE_BIN)
	 {
		// only a BIN field needs this many arguments
		size_bin = SvUV(ST(4));
		value_bin = (GMSEC_BIN) SvPV(ST(3), size_bin);

		fld = new Field(name, value_bin, (unsigned long) size_bin);

	 }
	 else if (items == 4)
	 {

		switch (type)
		{
		case GMSEC_TYPE_CHAR:
			tmp_char = SvPV_nolen(ST(3));
			value_char = (GMSEC_CHAR) tmp_char[0];
			fld = new Field(name, value_char);
			break;

		case GMSEC_TYPE_BOOL:
			tmp_short = SvIV(ST(3));
			value_bool = (GMSEC_BOOL) tmp_short;
			fld = new Field(name, value_bool);
			break;

		case GMSEC_TYPE_SHORT:
			value_short = (GMSEC_SHORT) SvIV(ST(3));
			fld = new Field(name, value_short);
			break;

		case GMSEC_TYPE_USHORT:
			value_ushort = (GMSEC_U16) SvUV(ST(3));
			fld = new Field(name, value_ushort);
			break;

		case GMSEC_TYPE_LONG:
			value_long = (GMSEC_LONG) SvIV(ST(3));
			fld = new Field(name, value_long);
			break;

		case GMSEC_TYPE_ULONG:
			value_ulong = (GMSEC_ULONG) SvUV(ST(3));
			fld = new Field(name, value_ulong);
			break;

		case GMSEC_TYPE_I64:
			value_i64 = (GMSEC_I64) SvIV(ST(3));
			fld = new Field(name, value_i64);
			break;

		case GMSEC_TYPE_FLOAT:
			value_float = (GMSEC_FLOAT) SvNV(ST(3));
			fld = new Field(name, value_float);
			break;

		case GMSEC_TYPE_DOUBLE:
			value_double = (GMSEC_DOUBLE) SvNV(ST(3));
			fld = new Field(name, value_double);
			break;

		case GMSEC_TYPE_STRING:
			value_string = (GMSEC_STRING) SvPV_nolen(ST(3));
			fld = new Field(name, value_string);
			break;

		default:
			fld = new Field();
		}

	 }
	 else
	 {
		fld = new Field();
	 }

	 // store object pointer
	 self = (HV *) SvRV(ST(0));
	 pointer = (IV) fld;
	 hvPointer = newSVuv(pointer);
	 hv_store(self, "pointer", 7, hvPointer, 0);

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   XSRETURN_YES;
}


GMSEC_XS_STATIC XS(_PGA_Field_GetName)
{
   Field *fld = (Field *) 0;
   const char *name = 0;
   Status result;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 1)
	 {
		result.Set(GMSEC_STATUS_FIELD_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Field] Usage: $name = $fld->GetName();");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Field"))
	 {
		result.Set(GMSEC_STATUS_FIELD_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Field] [GetName] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), fld);

	 result = fld->GetName(name);

	 storeStatus(ST(0), result);

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   if (result.GetCode() == 0 && name != NULL)
   {
	XSRETURN_PV(name);
   }
   else
   {
	XSRETURN_UNDEF;
   }
}


GMSEC_XS_STATIC XS(_PGA_Field_SetName)
{
   Field *fld = (Field *) 0;
   Status result;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 2)
	 {
		result.Set(GMSEC_STATUS_FIELD_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Field] Usage: $fld->SetName($name);");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Field"))
	 {
		result.Set(GMSEC_STATUS_FIELD_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Field] [SetName] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), fld);

	 result = fld->SetName(SvPV_nolen(ST(1)));

	 storeStatus(ST(0), result);

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   if (result.GetCode() == 0)
   {
	XSRETURN_YES;
   }
   else
   {
	XSRETURN_NO;
   }
}


GMSEC_XS_STATIC XS(_PGA_Field_GetType)
{
   Field *fld = (Field *) 0;
   GMSEC_TYPE type = GMSEC_TYPE_UNSET;
   Status result;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 1)
	 {
		result.Set(GMSEC_STATUS_FIELD_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Field] Usage: $type = $fld->GetType();");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Field"))
	 {
		result.Set(GMSEC_STATUS_FIELD_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Field] [GetType] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), fld);

	 result = fld->GetType(type);

	 storeStatus(ST(0), result);

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   if (result.GetCode() == 0)
   {
	XSRETURN_IV(type);
   }
   else
   {
	XSRETURN_UNDEF;
   }
}


GMSEC_XS_STATIC XS(_PGA_Field_SetType)
{
   Field *fld = (Field *) 0;
   Status result;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 2)
	 {
		result.Set(GMSEC_STATUS_FIELD_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Field] Usage: $fld->SetType($type);");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Field"))
	 {
		result.Set(GMSEC_STATUS_FIELD_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Field] [SetType] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), fld);

	 result = fld->SetType((GMSEC_TYPE) SvUV(ST(1)));

	 storeStatus(ST(0), result);

       }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   if (result.GetCode() == 0)
   {
	XSRETURN_YES;
   }
   else
   {
	XSRETURN_NO;
   }
}


GMSEC_XS_STATIC XS(_PGA_Field_GetValue)
{
   Field *fld = (Field *) 0;
   GMSEC_TYPE type;

   GMSEC_U32 size_bin  = 0;

   GMSEC_CHAR value_char = 0;
   GMSEC_BOOL value_bool = GMSEC_FALSE;
   GMSEC_SHORT value_short = 0;
   GMSEC_U16 value_ushort = 0;
   GMSEC_LONG value_long = 0;
   GMSEC_ULONG value_ulong = 0;
   GMSEC_I64 value_i64 = 0;
   GMSEC_FLOAT value_float = 0;
   GMSEC_DOUBLE value_double = 0;
   GMSEC_STRING value_string = 0;
   GMSEC_BIN value_bin = 0;

   Status result;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 1)
	 {
		result.Set(GMSEC_STATUS_FIELD_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Field] Usage: $value = $fld->GetValue(); OR ($value, $size) = $fld->GetValue();");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Field"))
	 {
		result.Set(GMSEC_STATUS_FIELD_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Field] [GetValue] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), fld);

	 result = fld->GetType(type);

	 switch (type)
	 {
	 case GMSEC_TYPE_CHAR:
		result = fld->GetValue(value_char);
		break;

	 case GMSEC_TYPE_BOOL:
		result = fld->GetValue(value_bool);
		break;

	 case GMSEC_TYPE_SHORT:
		result = fld->GetValue(value_short);
		break;

	 case GMSEC_TYPE_USHORT:
		result = fld->GetValue(value_ushort);
		break;

	 case GMSEC_TYPE_LONG:
		result = fld->GetValue(value_long);
		break;

	 case GMSEC_TYPE_ULONG:
		result = fld->GetValue(value_ulong);
		break;

	 case GMSEC_TYPE_I64:
		result = fld->GetValue(value_i64);
		break;

	 case GMSEC_TYPE_FLOAT:
		result = fld->GetValue(value_float);
		break;

	 case GMSEC_TYPE_DOUBLE:
		result = fld->GetValue(value_double);
		break;

	 case GMSEC_TYPE_STRING:
		result = fld->GetValue(value_string);
		break;

	 case GMSEC_TYPE_BIN:
		result = fld->GetValue(&value_bin, size_bin);
		if (size_bin == 0)
		{
			result.Set(GMSEC_STATUS_FIELD_ERROR,
			           GMSEC_FIELD_TYPE_MISMATCH,
			           "Field type mismatch");
		}
		break;

	 default:
		XSRETURN_UNDEF;
	 }

	 storeStatus(ST(0), result);

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   if (result.GetCode() == 0)
   {

	switch (type)
	{
	case GMSEC_TYPE_CHAR:
	{
		char tmp[2] = { value_char, 0 };
		XSRETURN_PV(tmp);
	}

	case GMSEC_TYPE_BOOL:
		XSRETURN_IV(value_bool);

	case GMSEC_TYPE_SHORT:
		XSRETURN_IV(value_short);

	case GMSEC_TYPE_USHORT:
		XSRETURN_IV(value_ushort);

	case GMSEC_TYPE_LONG:
		XSRETURN_IV(value_long);

	case GMSEC_TYPE_ULONG:
		XSRETURN_IV(value_ulong);

	case GMSEC_TYPE_I64:
	{
		// enough to hold the value 2^64, and then some
		char buffer[256];
		gmsec::util::I64toString(value_i64, buffer, sizeof(buffer));
		XSRETURN_PV(buffer);
	}

	case GMSEC_TYPE_FLOAT:
		XSRETURN_NV(value_float);

	case GMSEC_TYPE_DOUBLE:
		XSRETURN_NV(value_double);

	case GMSEC_TYPE_STRING:
		XSRETURN_PV(value_string);

	case GMSEC_TYPE_BIN:
		// Old code that was leaky.
		//
		//ST(0) = newSVpv((const char *) value_bin, size_bin);
		//ST(1) = newSViv(size_bin);

		// New code that corrects the memory leaks of above code by
		// using 'sv_2mortal()'.
		//
		ST(0) = sv_2mortal(newSVpv((const char *) value_bin, size_bin));
		ST(1) = sv_2mortal(newSViv(size_bin));
		XSRETURN(2);

	default:
		XSRETURN_UNDEF;
	}

   }
   else
   {
	XSRETURN_UNDEF;
   }
}


GMSEC_XS_STATIC XS(_PGA_Field_SetValue)
{
   Field *fld = (Field *) 0;
   GMSEC_TYPE type;

   const char *tmp_char;
   short tmp_short;
   unsigned int size_bin;
   unsigned int i;
   unsigned char *binBuffer = NULL;
   size_t sizeReturned;

   GMSEC_CHAR value_char;
   GMSEC_BOOL value_bool;
   GMSEC_SHORT value_short;
   GMSEC_U16 value_ushort;
   GMSEC_LONG value_long;
   GMSEC_ULONG value_ulong;
   //GMSEC_I64 value_i64;
   GMSEC_FLOAT value_float;
   GMSEC_DOUBLE value_double;
   GMSEC_STRING value_string;
   GMSEC_BIN value_bin;

   Status result;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 2 && items != 3)
	 {
		result.Set(GMSEC_STATUS_FIELD_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Field] Usage: $fld->SetValue($value); OR $fld->SetValue($value, $size);");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Field"))
	 {
		result.Set(GMSEC_STATUS_FIELD_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Field] [GetValue] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), fld);

	 result = fld->GetType(type);

	 switch (type)
	 {
	 case GMSEC_TYPE_CHAR:
		tmp_char = SvPV_nolen(ST(1));
		value_char = (GMSEC_CHAR) tmp_char[0];
		result = fld->SetValue(value_char);
		break;

	 case GMSEC_TYPE_BOOL:
		tmp_short = SvIV(ST(1));
		value_bool = (GMSEC_BOOL) tmp_short;
		result = fld->SetValue(value_bool);
		break;

	 case GMSEC_TYPE_SHORT:
		value_short = (GMSEC_SHORT) SvIV(ST(1));
		result = fld->SetValue(value_short);
		break;

	 case GMSEC_TYPE_USHORT:
		value_ushort = (GMSEC_U16) SvUV(ST(1));
		result = fld->SetValue(value_ushort);
		break;

	 case GMSEC_TYPE_LONG:
		value_long = (GMSEC_LONG) SvIV(ST(1));
		result = fld->SetValue(value_long);
		break;

	 case GMSEC_TYPE_ULONG:
		value_ulong = (GMSEC_ULONG) SvUV(ST(1));
		result = fld->SetValue(value_ulong);
		break;

	 case GMSEC_TYPE_I64:
	 {
		value_string = (GMSEC_STRING) SvPV_nolen(ST(1));
		result = fld->SetValue(gmsec::util::stringToI64(value_string));
	 }
	 break;

	 case GMSEC_TYPE_FLOAT:
		value_float = (GMSEC_FLOAT) SvNV(ST(1));
		result = fld->SetValue(value_float);
		break;

	 case GMSEC_TYPE_DOUBLE:
		value_double = (GMSEC_DOUBLE) SvNV(ST(1));
		result = fld->SetValue(value_double);
		break;

	 case GMSEC_TYPE_STRING:
		value_string = (GMSEC_STRING) SvPV_nolen(ST(1));
		result = fld->SetValue(value_string);
		break;

	 case GMSEC_TYPE_BIN:
		size_bin = SvUV(ST(2));
		sizeReturned = 0;
		binBuffer = new unsigned char[size_bin];

		value_bin = (GMSEC_BIN) SvPV(ST(1), sizeReturned);

		// padd the end of the buffer with 0x00 if needed
		for (i = sizeReturned; i < size_bin; i++)
		{
			binBuffer[i] = 0;
		}


		memcpy(binBuffer, value_bin,
		       ((sizeReturned <= size_bin)?sizeReturned:size_bin));

		result = fld->SetValue(binBuffer, size_bin);

		delete [] binBuffer;

		break;

	 default:
		XSRETURN_NO;
	 }

	 storeStatus(ST(0), result);

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   if (result.GetCode() == 0)
   {
	XSRETURN_YES;
   }
   else
   {
	XSRETURN_NO;
   }
}


GMSEC_XS_STATIC XS(_PGA_Field_UnSet)
{
   Field *fld = (Field *) 0;
   Status result;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 1)
	 {
		result.Set(GMSEC_STATUS_FIELD_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Field] Usage: $fld->UnSet();");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Field"))
	 {
		result.Set(GMSEC_STATUS_FIELD_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Field] [UnSet] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), fld);

	 result = fld->UnSet();

	 storeStatus(ST(0), result);

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   if (result.GetCode() == 0)
   {
	XSRETURN_YES;
   }
   else
   {
	XSRETURN_NO;
   }
}


GMSEC_XS_STATIC XS(_PGA_Field_ToXML)
{
   Field *fld = (Field *) 0;
   const char *xml = 0;
   Status result;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 1)
	 {
		result.Set(GMSEC_STATUS_FIELD_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Field] Usage: $xml = $fld->ToXML();");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Field"))
	 {
		result.Set(GMSEC_STATUS_FIELD_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Field] [ToXML] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), fld);

	 result = fld->ToXML(xml);

	 storeStatus(ST(0), result);

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   if (xml != NULL)
   {
	XSRETURN_PV(xml);
   }
   else
   {
	XSRETURN_UNDEF;
   }
}


GMSEC_XS_STATIC XS(_PGA_Field_DESTROY)
{
   Field *fld = (Field *) 0;
   Status * objStatus = NULL;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 // warn("[GMSECAPI::Field] $obj->DESTROY();");
	 if (items != 1)
	 {
		warn("[GMSECAPI::Field] Usage: $fld->DESTROY();");
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Field"))
	 {
		warn("[GMSECAPI::Field] [DESTROY] first argument is not a blessed object");
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), fld);
	 getStatus(ST(0), objStatus);

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   if (objStatus)
   {
	delete objStatus;
   }

   if (fld)
   {
	delete fld;
   }
}


LINKAGE XS(boot_GMSECAPI__Field)
{
   dXSARGS;
   dXCPT;

   USE_ITEMS;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {

	 newXS("GMSECAPI::Field::Field", _PGA_Field_Create, __FILE__);

	 newXS("GMSECAPI::Field::GetName", _PGA_Field_GetName, __FILE__);
	 newXS("GMSECAPI::Field::SetName", _PGA_Field_SetName, __FILE__);

	 newXS("GMSECAPI::Field::GetType", _PGA_Field_GetType, __FILE__);
	 newXS("GMSECAPI::Field::SetType", _PGA_Field_SetType, __FILE__);

	 newXS("GMSECAPI::Field::GetValue", _PGA_Field_GetValue, __FILE__);
	 newXS("GMSECAPI::Field::SetValue", _PGA_Field_SetValue, __FILE__);

	 newXS("GMSECAPI::Field::UnSet", _PGA_Field_UnSet, __FILE__);

	 newXS("GMSECAPI::Field::ToXML", _PGA_Field_ToXML, __FILE__);

	 newXS("GMSECAPI::Field::DESTROY", _PGA_Field_DESTROY, __FILE__);

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   XSRETURN_YES;
}
