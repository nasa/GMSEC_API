
/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/*----------------------------------------------------------------------------
 * Message.cpp
 *
 * Description: Base class for GMSEC messages
 *
 *--------------------------------------------------------------------------*/
#include <stdio.h>

// GMSEC API and Perl support library
#include "gmsec_perl.h"


#ifndef PERL_OBJECT
#ifndef MULTIPLICITY
LINKAGE GMSEC_XS_EXPORT(void) boot_GMSECAPI__Message(CV* cv);
#else
LINKAGE GMSEC_XS_EXPORT(void) boot_GMSECAPI__Message(pTHXo_ CV* cv);
#endif
#else
LINKAGE GMSEC_XS_EXPORT(void) boot_GMSECAPI__Message(CV *cv, CPerlObj *);
#endif


SV *makePerlFieldObject(Field &fld)
{
   HV *fldHash;
   SV *fldPointer;
   HV *fldStash;
   SV *fldPerlObject = NULL;
   IV pointer;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 // create the Perl Field object
	 fldHash = newHV();

	 pointer = (IV) new Field(fld);
	 fldPointer = newSVuv(pointer);
	 hv_store(fldHash, "pointer", 7, fldPointer, 0);

	 fldPerlObject = sv_2mortal(newRV_inc(sv_2mortal((SV *) fldHash)));
	 fldStash = gv_stashpv("GMSECAPI::Field", 1);
	 sv_bless(fldPerlObject, fldStash);

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   return fldPerlObject;
}


GMSEC_XS_STATIC XS(_PGA_Message_isValid)
{
   Message *msg = (Message *) 0;
   bool valid = 0;
   dXSARGS;
   Status result;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 1)
	 {
		result.Set(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Message] Usage: $valid = $msg->isValid();");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Message"))
	 {
		result.Set(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Message] [isValid] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), msg);

	 valid = msg->isValid();

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   if (valid == TRUE)
   {
	XSRETURN_YES;
   }
   else
   {
	XSRETURN_NO;
   }
}


GMSEC_XS_STATIC XS(_PGA_Message_GetLibraryRootName)
{
   Message *msg = (Message *) 0;
   const char *libname = NULL;
   dXSARGS;
   Status result;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 1)
	 {
		result.Set(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Message] Usage: $libname = $msg->GetLibraryRootName();");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Message"))
	 {
		result.Set(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Message] [GetLibraryRootName] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), msg);

	 libname = msg->GetLibraryRootName();

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   if (libname != NULL)
   {
	XSRETURN_PV(libname);
   }
   else
   {
	XSRETURN_NO;
   }
}


GMSEC_XS_STATIC XS(_PGA_Message_SetConfig)
{
   Message *msg = (Message *) 0;
   Status result;
   Config *cfg = (Config *) 0;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 2)
	 {
		result.Set(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Message] Usage: $msg->SetKind($kind);");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Message"))
	 {
		result.Set(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Message] [SetKind] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(1), "GMSECAPI::Config"))
	 {
		result.Set(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Message] [SetKind] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), msg);
	 getObject(ST(1), cfg);

	 result = msg->SetConfig(cfg);

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


GMSEC_XS_STATIC XS(_PGA_Message_SetKind)
{
   Message *msg = (Message *) 0;
   Status result;
   GMSEC_MSG_KIND kind;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 2)
	 {
		result.Set(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Message] Usage: $msg->SetKind($kind);");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Message"))
	 {
		result.Set(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Message] [SetKind] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), msg);
	 kind = SvUV(ST(1));

	 result = msg->SetKind(kind);

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


GMSEC_XS_STATIC XS(_PGA_Message_GetKind)
{
   Message *msg = (Message *) 0;
   Status result;
   GMSEC_MSG_KIND kind = GMSEC_MSG_UNSET;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 1)
	 {
		result.Set(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Message] Usage: $kind = $msg->GetKind();");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Message"))
	 {
		result.Set(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Message] [GetKind] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), msg);

	 result = msg->GetKind(kind);

	 storeStatus(ST(0), result);

       }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   if (result.GetCode() == 0)
   {
	XSRETURN_IV(kind);
   }
   else
   {
	XSRETURN_UNDEF;
   }
}


GMSEC_XS_STATIC XS(_PGA_Message_SetSubject)
{
   Message *msg = (Message *) 0;
   Status result;
   const char *subject;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 2)
	 {
		result.Set(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Message] Usage: $msg->SetSubject($subject);");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Message"))
	 {
		result.Set(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Message] [SetSubject] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), msg);
	 subject = SvPV_nolen(ST(1));

	 result = msg->SetSubject(subject);

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


GMSEC_XS_STATIC XS(_PGA_Message_GetSubject)
{
   Message *msg = (Message *) 0;
   Status result;
   const char *subject = NULL;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 1)
	 {
		result.Set(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Message] Usage: $subject = $msg->GetSubject();");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Message"))
	 {
		result.Set(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Message] [GetSubject] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), msg);

	 result = msg->GetSubject(subject);

	 storeStatus(ST(0), result);

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   if (result.GetCode() == 0 && subject != NULL)
   {
	XSRETURN_PV(subject);
   }
   else
   {
	XSRETURN_UNDEF;
   }
}


GMSEC_XS_STATIC XS(_PGA_Message_AddField)
{
   Message *msg = (Message *) 0;
   Status result;
   Field * fld = (Field *) 0;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 2)
	 {
		result.Set(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Message] Usage: $msg->AddField($field);");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Message"))
	 {
		result.Set(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Message] [AddField] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), msg);
	 getObject(ST(1), fld);

	 result = msg->AddField(*fld);

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


GMSEC_XS_STATIC XS(_PGA_Message_GetField)
{
   Message *msg = (Message *) 0;
   Status result;
   const char *name;
   Field fld;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 2)
	 {
		result.Set(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Message] Usage: $field = $msg->GetField($name);");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Message"))
	 {
		result.Set(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Message] [GetField] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), msg);
	 name = SvPV_nolen(ST(1));

	 result = msg->GetField(name, fld);

	 storeStatus(ST(0), result);

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   if (result.GetCode() == 0)
   {
	ST(0) = makePerlFieldObject(fld);
	XSRETURN(1);
   }
   else
   {
	XSRETURN_UNDEF;
   }
}


GMSEC_XS_STATIC XS(_PGA_Message_GetFieldCount)
{
   Message *msg = (Message *) 0;
   Status result;
   GMSEC_I32 count = 0;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 1)
	 {
		result.Set(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Message] Usage: $count = $msg->GetFieldCount();");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Message"))
	 {
		result.Set(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Message] [GetFieldCount] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), msg);

	 result = msg->GetFieldCount(count);

	 storeStatus(ST(0), result);

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   if (result.GetCode() == 0)
   {
	XSRETURN_IV(count);
   }
   else
   {
	XSRETURN_UNDEF;
   }
}


GMSEC_XS_STATIC XS(_PGA_Message_GetFirstField)
{
   Message *msg = (Message *) 0;
   Status result;
   Field fld;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 1)
	 {
		result.Set(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Message] Usage: $field = $msg->GetFirstField();");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Message"))
	 {
		result.Set(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Message] [GetFirstField] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), msg);

	 result = msg->GetFirstField(fld);

	 storeStatus(ST(0), result);

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   if (result.GetCode() == 0)
   {
	ST(0) = makePerlFieldObject(fld);
	XSRETURN(1);
   }
   else
   {
	XSRETURN_UNDEF;
   }
}


GMSEC_XS_STATIC XS(_PGA_Message_GetNextField)
{
   Message *msg = (Message *) 0;
   Status result;
   Field fld;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 1)
	 {
		result.Set(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Message] Usage: $field = $msg->GetNextField();");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Message"))
	 {
		result.Set(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Message] [GetNextField] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), msg);

	 result = msg->GetNextField(fld);

	 storeStatus(ST(0), result);

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   if (result.GetCode() == 0)
   {
	ST(0) = makePerlFieldObject(fld);
	XSRETURN(1);
   }
   else
   {
	XSRETURN_UNDEF;
   }
}


GMSEC_XS_STATIC XS(_PGA_Message_ToXML)
{
   Message *msg = (Message *) 0;
   Status result;
   const char *xml = NULL;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 1)
	 {
		result.Set(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Message] Usage: $xml = $msg->ToXML();");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Message"))
	 {
		result.Set(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Message] [ToXML] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), msg);

	 result = msg->ToXML(xml);

	 storeStatus(ST(0), result);

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   if (xml != NULL)
   {
#if 0
	XSRETURN_PV(xml);
#else
        ST(0) = sv_2mortal(newSVpv(xml, 0));
        XSRETURN(1);
#endif
   }
   else
   {
	XSRETURN_UNDEF;
   }
}


GMSEC_XS_STATIC XS(_PGA_Message_FromXML)
{
   Message *msg = (Message *) 0;
   Status result;
   const char *xml;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 2)
	 {
		result.Set(GMSEC_STATUS_CONFIG_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::Message] Usage: $msg->FromXML();");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Message"))
	 {
		result.Set(GMSEC_STATUS_CONFIG_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::Message] [FromXML] first ergument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), msg);
	 xml = SvPV_nolen(ST(1));

	 result = msg->FromXML(xml);
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



GMSEC_XS_STATIC XS(_PGA_Message_GetMsgSize)
{
   Message *msg = (Message *) 0;
   Status result;
   GMSEC_U32 size = 0;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 1)
	 {
		result.Set(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Message] Usage: $size = $msg->GetMsgSize();");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Message"))
	 {
		result.Set(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Message] [GetMsgSize] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), msg);

	 result = msg->GetMSGSize(size);

	 storeStatus(ST(0), result);

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   if (result.GetCode() == 0)
   {
	XSRETURN_IV(size);
   }
   else
   {
	XSRETURN_UNDEF;
   }
}


GMSEC_XS_STATIC XS(_PGA_Message_GetTime)
{
   Message *msg = (Message *) 0;
   const char *time = NULL;
   dXSARGS;
   Status result;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 1)
	 {
		result.Set(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Message] Usage: $time = $msg->GetTime();");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Message"))
	 {
		result.Set(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Message] [GetTime] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), msg);

	 time = msg->GetTime();

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   if (time != NULL)
   {
	XSRETURN_PV(time);
   }
   else
   {
	XSRETURN_NO;
   }
}

GMSEC_XS_STATIC XS(_PGA_Message_ClearField)
{
   Message *msg = (Message *) 0;
   Status result;
   const char *fldName;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 2)
	 {
		result.Set(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_OTHER_ERROR,"{GMSECAPI::Message] Usage: $msg->ClearField($fieldName);");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Message"))
	 {
		result.Set(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Message] [SetSubject] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), msg);
	 fldName = SvPV_nolen(ST(1));

	 result = msg->ClearField(fldName);

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

GMSEC_XS_STATIC XS(_PGA_Message_ClearFields)
{
   Message *msg = (Message *) 0;
   Status result;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 1)
	 {
		result.Set(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_OTHER_ERROR,"{GMSECAPI::Message] Usage: $msg->ClearFields();");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Message"))
	 {
		result.Set(GMSEC_STATUS_MESSAGE_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Message] [SetSubject] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), msg);

	 result = msg->ClearFields();

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

LINKAGE XS(boot_GMSECAPI__Message)
{
   dXSARGS;
   dXCPT;

   USE_ITEMS;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 newXS("GMSECAPI::Message::isValid", _PGA_Message_isValid, __FILE__);

	 newXS("GMSECAPI::Message::GetLibraryRootName", _PGA_Message_GetLibraryRootName, __FILE__);

	 newXS("GMSECAPI::Message::SetConfig", _PGA_Message_SetConfig, __FILE__);

	 newXS("GMSECAPI::Message::SetKind", _PGA_Message_SetKind, __FILE__);
	 newXS("GMSECAPI::Message::GetKind", _PGA_Message_GetKind, __FILE__);

	 newXS("GMSECAPI::Message::SetSubject", _PGA_Message_SetSubject, __FILE__);
	 newXS("GMSECAPI::Message::GetSubject", _PGA_Message_GetSubject, __FILE__);

	 newXS("GMSECAPI::Message::AddField", _PGA_Message_AddField, __FILE__);
	 newXS("GMSECAPI::Message::GetField", _PGA_Message_GetField, __FILE__);

	 newXS("GMSECAPI::Message::GetFieldCount", _PGA_Message_GetFieldCount, __FILE__);

	 newXS("GMSECAPI::Message::GetFirstField", _PGA_Message_GetFirstField, __FILE__);
	 newXS("GMSECAPI::Message::GetNextField", _PGA_Message_GetNextField, __FILE__);

	 newXS("GMSECAPI::Message::ToXML", _PGA_Message_ToXML, __FILE__);
	 newXS("GMSECAPI::Message::FromXML", _PGA_Message_FromXML, __FILE__);


	 newXS("GMSECAPI::Message::GetMsgSize", _PGA_Message_GetMsgSize, __FILE__);

	 newXS("GMSECAPI::Message::GetTime", _PGA_Message_GetTime, __FILE__);

	 newXS("GMSECAPI::Message::ClearField", _PGA_Message_ClearField, __FILE__);
	 newXS("GMSECAPI::Message::ClearFields", _PGA_Message_ClearFields, __FILE__);

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   XSRETURN_YES;
}
