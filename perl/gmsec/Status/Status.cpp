
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/*----------------------------------------------------------------------------
 * Status.cpp
 *
 * Description: Class for holding status information
 *
 *--------------------------------------------------------------------------*/

// GMSEC API and Perl support library
#include "gmsec_perl.h"


#ifndef PERL_OBJECT
#ifndef MULTIPLICITY
LINKAGE GMSEC_XS_EXPORT(void) boot_GMSECAPI__Status(CV* cv);
#else
LINKAGE GMSEC_XS_EXPORT(void) boot_GMSECAPI__Status(pTHXo_ CV* cv);
#endif
#else
LINKAGE GMSEC_XS_EXPORT(void) boot_GMSECAPI__Status(CV *cv, CPerlObj *);
#endif


GMSEC_XS_STATIC XS(_PGA_Status_Create)
{
   HV *self;
   SV *hvPointer;
   IV pointer;
   Status *status = (Status *) 0;
   GMSEC_STATUS_CLASS eclass;
   unsigned int code;
   const char *text;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 1 && items != 4)
	 {
		warn("[GMSECAPI::Status] Usage: Status($self); OR Status($self,$eclass,$code,$text);");
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Status"))
	 {
		warn("[GMSECAPI::Status] first argument is not a blessed object");
		XSRETURN_UNDEF;
	 }

	 if (items == 4)
	 {
		eclass = (GMSEC_STATUS_CLASS) SvUV(ST(1));
		code = (unsigned int) SvUV(ST(2));
		text = SvPV_nolen(ST(3));

		status = new Status(eclass, code, text);

	 }
	 else
	 {
		status = new Status();
	 }

	 // store object pointer
	 self = (HV *) SvRV(ST(0));
	 pointer = (IV) status;
	 hvPointer = newSVuv(pointer);
	 hv_store(self, "pointer", 7, hvPointer, 0);

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   XSRETURN_YES;
}


GMSEC_XS_STATIC XS(_PGA_Status_GetString)
{
   Status *status = (Status *) 0;
   const char *statusStr = NULL;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 1)
	 {
		warn("[GMSECAPI::Status] Usage: $string = $status->GetString();");
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Status"))
	 {
		warn("[GMSECAPI::Status] [GetString] first argument is not a blessed object");
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), status);

	 statusStr = status->GetString();

       }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   if (statusStr != NULL)
   {
	XSRETURN_PV(statusStr);
   }
   else
   {
	XSRETURN_UNDEF;
   }
}


GMSEC_XS_STATIC XS(_PGA_Status_Get)
{
   Status *status = (Status *) 0;
   const char *statusStr = NULL;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 1)
	 {
		warn("[GMSECAPI::Status] Usage: $string = $status->Get();");
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Status"))
	 {
		warn("[GMSECAPI::Status] [Get] first argument is not a blessed object");
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), status);

	 statusStr = status->GetString();

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   if (statusStr != NULL)
   {
	statusStr = status->Get();
	XSRETURN_PV(statusStr);
   }
   else
   {
	XSRETURN_UNDEF;
   }
}


GMSEC_XS_STATIC XS(_PGA_Status_GetClass)
{
   Status *status = (Status *) 0;
   GMSEC_STATUS_CLASS statusClass = 0;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 1)
	 {
		warn("[GMSECAPI::Status] Usage: $class = $status->GetClass();");
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Status"))
	 {
		warn("[GMSECAPI::Status] [GetClass] first argument is not a blessed object");
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), status);

	 statusClass = status->GetClass();

       }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   XSRETURN_IV(statusClass);
}


GMSEC_XS_STATIC XS(_PGA_Status_GetCode)
{
   Status *status = (Status *) 0;
   int statusCode = 0;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 1)
	 {
		warn("[GMSECAPI::Status] Usage: $code = $status->GetCode();");
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Status"))
	 {
		warn("[GMSECAPI::Status] [GetCode] first argument is not a blessed object");
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), status);

	 statusCode = status->GetCode();

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   XSRETURN_IV(statusCode);
}


GMSEC_XS_STATIC XS(_PGA_Status_GetCustomCode)
{
   Status *status = (Status *) 0;
   int customCode = 0;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 1)
	 {
		warn("[GMSECAPI::Status] Usage: $code = $status->GetCustomCode();");
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Status"))
	 {
		warn("[GMSECAPI::Status] [GetCustomCode] first argument is not a blessed object");
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), status);

	 customCode = status->GetCustomCode();

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   XSRETURN_IV(customCode);
}


GMSEC_XS_STATIC XS(_PGA_Status_SetString)
{
   Status *status = (Status *) 0;
   const char *text;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 2)
	 {
		warn("[GMSECAPI::Status] Usage: $status->SetString($text);");
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Status"))
	 {
		warn("[GMSECAPI::Status] [SetString] first argument is not a blessed object");
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), status);

	 text = SvPV_nolen(ST(1));

	 status->SetString(text);
 
      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   XSRETURN_YES;
}


GMSEC_XS_STATIC XS(_PGA_Status_SetClass)
{
   Status *status = (Status *) 0;
   GMSEC_STATUS_CLASS statusClass;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 2)
	 {
		warn("[GMSECAPI::Status] Usage: $status->SetClass($class);");
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Status"))
	 {
		warn("[GMSECAPI::Status] [SetClass] first argument is not a blessed object");
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), status);

	 statusClass = (GMSEC_STATUS_CLASS) SvIV(ST(1));

	 status->SetClass(statusClass);

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   XSRETURN_YES;
}


GMSEC_XS_STATIC XS(_PGA_Status_SetCode)
{
   Status *status = (Status *) 0;
   int statusCode;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 2)
	 {
		warn("[GMSECAPI::Status] Usage: $status->SetCode($code);");
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Status"))
	 {
		warn("[GMSECAPI::Status] [SetCode] first argument is not a blessed object");
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), status);

	 statusCode = SvIV(ST(1));

	 status->SetCode(statusCode);

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   XSRETURN_YES;
}


GMSEC_XS_STATIC XS(_PGA_Status_SetCustomCode)
{
   Status *status = (Status *) 0;
   int customCode;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 2)
	 {
		warn("[GMSECAPI::Status] Usage: $status->SetCustomCode($code);");
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Status"))
	 {
		warn("[GMSECAPI::Status] [SetCustomCode] first argument is not a blessed object");
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), status);

	 customCode = SvIV(ST(1));

	 status->SetCustomCode(customCode);

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   XSRETURN_YES;
}


GMSEC_XS_STATIC XS(_PGA_Status_Set)
{
   Status *status = (Status *) 0;
   GMSEC_STATUS_CLASS statusClass;
   unsigned int statusCode;
   const char *text;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 4)
	 {
		warn("[GMSECAPI::Status] Usage: $status->Set($class, $code, $text);");
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Status"))
	 {
		warn("[GMSECAPI::Status] [Set] first argument is not a blessed object");
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), status);

	 statusClass = (GMSEC_STATUS_CLASS) SvIV(ST(1));
	 statusCode = SvUV(ST(2));
	 text = SvPV_nolen(ST(3));

	 status->Set(statusClass, statusCode, text);

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   XSRETURN_YES;
}


GMSEC_XS_STATIC XS(_PGA_Status_ReSet)
{
   Status *status = (Status *) 0;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 1)
	 {
		warn("[GMSECAPI::Status] Usage: $status->ReSet();");
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Status"))
	 {
		warn("[GMSECAPI::Status] [ReSet] first argument is not a blessed object");
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), status);

	 status->ReSet();

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   XSRETURN_YES;
}



GMSEC_XS_STATIC XS(_PGA_Status_isError)
{
   Status *status = (Status *) 0;
   int isError = 0;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 1)
	 {
		warn("[GMSECAPI::Status] Usage: $code = $status->isError();");
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Status"))
	 {
		warn("[GMSECAPI::Status] [isError] first argument is not a blessed object");
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), status);

	 isError = status->isError();

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   XSRETURN_IV(isError);
}


GMSEC_XS_STATIC XS(_PGA_Status_DESTROY)
{
   Status *status = (Status *) 0;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 // warn("[GMSECAPI::Status] $obj->DESTROY();");
	 if (items != 1)
	 {
		warn("[GMSECAPI::Status] Usage: $status->DESTROY();");
		XSRETURN_UNDEF;
	 }

	 SV *sv = ST(0);

	 if (!sv_isa(sv, "GMSECAPI::Status"))
	 {
		warn("[GMSECAPI::Status] [DESTROY] first argument is not a blessed object");
		XSRETURN_UNDEF;
	 }

	 getObject(sv, status);

	/* Note: this does not deallocate the Status object (memory leak).
	The code should test for a non-null pointer.  If it is non-null, it
	should delete the Status pointer and set it to null. */

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH
}


LINKAGE XS(boot_GMSECAPI__Status)
{
   dXSARGS;
   dXCPT;

   USE_ITEMS;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 newXS("GMSECAPI::Status::Status", _PGA_Status_Create, __FILE__);

	 newXS("GMSECAPI::Status::GetString", _PGA_Status_GetString, __FILE__);
	 newXS("GMSECAPI::Status::Get", _PGA_Status_Get, __FILE__);
	 newXS("GMSECAPI::Status::GetClass", _PGA_Status_GetClass, __FILE__);
	 newXS("GMSECAPI::Status::GetCode", _PGA_Status_GetCode, __FILE__);
	 newXS("GMSECAPI::Status::GetCustomCode", _PGA_Status_GetCustomCode, __FILE__);

	 newXS("GMSECAPI::Status::SetString", _PGA_Status_SetString, __FILE__);
	 newXS("GMSECAPI::Status::SetClass", _PGA_Status_SetClass, __FILE__);
	 newXS("GMSECAPI::Status::SetCode", _PGA_Status_SetCode, __FILE__);
	 newXS("GMSECAPI::Status::SetCustomCode", _PGA_Status_SetCustomCode, __FILE__);
	 newXS("GMSECAPI::Status::Set", _PGA_Status_Set, __FILE__);

	 newXS("GMSECAPI::Status::ReSet", _PGA_Status_ReSet, __FILE__);
	 newXS("GMSECAPI::Status::isError", _PGA_Status_isError, __FILE__);

	 newXS("GMSECAPI::Status::DESTROY", _PGA_Status_DESTROY, __FILE__);

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   XSRETURN_YES;
}
