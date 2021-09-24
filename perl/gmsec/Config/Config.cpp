
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/*----------------------------------------------------------------------------
 * Config.cpp
 *
 * Description: Class for managing configurations
 *
 *--------------------------------------------------------------------------*/

// GMSEC API and Perl support library
#include "gmsec_perl.h"


#ifndef PERL_OBJECT
#ifndef MULTIPLICITY
LINKAGE GMSEC_XS_EXPORT(void) boot_GMSECAPI__Config(CV* cv);
#else
LINKAGE GMSEC_XS_EXPORT(void) boot_GMSECAPI__Config(pTHXo_ CV* cv);
#endif
#else
LINKAGE GMSEC_XS_EXPORT(void) boot_GMSECAPI__Config(CV *cv, CPerlObj *);
#endif


GMSEC_XS_STATIC XS(_PGA_Config_Create)
{
   HV *self;
   SV *hvPointer;
   AV *pARGV;
   SV *arrayElement;
   int argc, i;
   IV pointer;
   char **argv = NULL;
   Config *cfg = (Config *) 0;
   dXSARGS;
   Status result;

   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {


	if (items != 1 && items != 2)
	{
		result.Set(GMSEC_STATUS_CONFIG_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::Config] Usage: Config($self); OR Config($self, [ @ARGV ]);");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	if (!sv_isa((SV*) ST(0), "GMSECAPI::Config"))
	{
		result.Set(GMSEC_STATUS_CONFIG_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::Config] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	if (items == 2)
	{
		pARGV = (AV *) SvRV(ST(1));
		argc = av_len(pARGV);

		if (argc >= 0)
		{
			argv = new char * [argc+1];

			for (i = 0; i <= argc; i++)
			{

				arrayElement = sv_2mortal(av_shift(pARGV));
				const char *s = SvPV_nolen(arrayElement);
				int slen = strlen(s);
				argv[i] = new char [slen+1];
				strcpy(argv[i], s);
			}

			cfg = new Config(argc+1, argv);

			for (i = 0; i <= argc; i++)
			{
				char * temp = argv[i];
				delete [] temp;
			}

			delete [] argv;

		}
		else
		{
			cfg = new Config();
		}

	}
	else
	{
		cfg = new Config();
	}

	// store object pointer
	self = (HV *) SvRV(ST(0));
	pointer = (IV) cfg;
	hvPointer = newSVuv(pointer);
	hv_store(self, "pointer", 7, hvPointer, 0);

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   XSRETURN_YES;
}


GMSEC_XS_STATIC XS(_PGA_Config_AddValue)
{
   Config *cfg = (Config *) 0;
   const char *name = 0;
   const char *value = 0;
   Status result;
   dXSARGS;

   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {

	   if (items != 3)
	   {
		result.Set(GMSEC_STATUS_CONFIG_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::Config] Usage: $cfg->AddValue($name, $value);");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	   }

	   if (!sv_isa((SV*) ST(0), "GMSECAPI::Config"))
	   {
		result.Set(GMSEC_STATUS_CONFIG_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::Config] [AddValue] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	   }

	   getObject(ST(0), cfg);
	   name = SvPV_nolen(ST(1));
	   value = SvPV_nolen(ST(2));

	   result = cfg->AddValue(name, value);

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


GMSEC_XS_STATIC XS(_PGA_Config_ClearValue)
{
   Config *cfg = (Config *) 0;
   const char *name = 0;
   Status result;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
         if (items != 2)
	 {
		result.Set(GMSEC_STATUS_CONFIG_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::Config] Usage: $cfg->ClearValue($name);");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Config"))
	 {
		result.Set(GMSEC_STATUS_CONFIG_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::Config] [ClearValue] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), cfg);
	 name = SvPV_nolen(ST(1));

	 result = cfg->ClearValue(name);

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


GMSEC_XS_STATIC XS(_PGA_Config_GetValue)
{
   Config *cfg = (Config *) 0;
   const char *name = 0;
   const char *value = 0;
   Status result;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
         if (items != 2)
	 {
		result.Set(GMSEC_STATUS_CONFIG_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::Config] Usage: $value = $cfg->GetValue($name);");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Config"))
	 {
		result.Set(GMSEC_STATUS_CONFIG_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::Config] [GetValue] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), cfg);
	 name = SvPV_nolen(ST(1));

	 result = cfg->GetValue(name, value);

	 storeStatus(ST(0), result);

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   if (result.GetCode() == 0 && value != NULL)
   {
	XSRETURN_PV(value);
   }
   else
   {
	XSRETURN_UNDEF;
   }
}


GMSEC_XS_STATIC XS(_PGA_Config_Clear)
{
   Config *cfg = (Config *) 0;
   Status result;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
         if (items != 1)
	 {
		result.Set(GMSEC_STATUS_CONFIG_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::Config] Usage: $cfg->Clear();");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Config"))
	 {
		result.Set(GMSEC_STATUS_CONFIG_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::Config] [Clear] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), cfg);

	 result = cfg->Clear();

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


GMSEC_XS_STATIC XS(_PGA_Config_GetFirst)
{
   Config *cfg = (Config *) 0;
   const char *name = 0;
   const char *value = 0;
   Status result;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
         if (items != 1)
	 {
		result.Set(GMSEC_STATUS_CONFIG_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::Config] Usage: ($name, $value) = $cfg->GetFirst();");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Config"))
	 {
		result.Set(GMSEC_STATUS_CONFIG_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::Config] [GetFirst] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), cfg);

	 result = cfg->GetFirst(name, value);

	 storeStatus(ST(0), result);

       }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   if (result.GetCode() == 0 && name != NULL && value != NULL)
   {
      XST_mPV(0, name);
      XST_mPV(1, value);
   }
   else
   {
      ST(0) = &PL_sv_undef;
      ST(1) = &PL_sv_undef;
   }

   XSRETURN(2);
}


GMSEC_XS_STATIC XS(_PGA_Config_GetNext)
{
   Config *cfg = (Config *) 0;
   const char *name = 0;
   const char *value = 0;
   Status result;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
         if (items != 1)
	 {
		result.Set(GMSEC_STATUS_CONFIG_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::Config] Usage: ($name, $value) = $cfg->GetNext();");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Config"))
	 {
		result.Set(GMSEC_STATUS_CONFIG_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::Config] [GetNext] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), cfg);

	 result = cfg->GetNext(name, value);

	 storeStatus(ST(0), result);

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   if (result.GetCode() == 0 && name != NULL && value != NULL)
   {
      XST_mPV(0, name);
      XST_mPV(1, value);
   }
   else
   {
      ST(0) = &PL_sv_undef;
      ST(1) = &PL_sv_undef;
   }

   XSRETURN(2);
}


GMSEC_XS_STATIC XS(_PGA_Config_FromXML)
{
   Config *cfg = (Config *) 0;
   Status result;
   const char * xml = 0;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
         if (items != 2)
	 {
		result.Set(GMSEC_STATUS_CONFIG_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::Config] Usage: $cfg->FromXML();");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Config"))
	 {
		result.Set(GMSEC_STATUS_CONFIG_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::Config] [FromXML] first ergument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), cfg);
	 xml = SvPV_nolen(ST(1));

	 result = cfg->FromXML(xml);
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


GMSEC_XS_STATIC XS(_PGA_Config_ToXML)
{
   Config *cfg = (Config *) 0;
   const char * xml = 0;
   Status result;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 1)
	 {
		result.Set(GMSEC_STATUS_CONFIG_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::Config] Usage: $cfg->ToXML();");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Config"))
	 {
		result.Set(GMSEC_STATUS_CONFIG_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::Config] [ToXML] first ergument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), cfg);

	 result = cfg->ToXML(xml);
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

GMSEC_XS_STATIC XS(_PGA_Config_DESTROY)
{
   Status * objStatus = NULL;
   Config *cfg = (Config *) 0;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 // warn("[GMSECAPI::Config] $obj->DESTROY();");
	 if (items != 1)
	 {
		warn("[GMSECAPI::Config] Usage: $cfg->DESTROY();");
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::Config"))
	 {
		warn("[GMSECAPI::Config] [DESTROY] first argument is not a blessed object");
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), cfg);
	 //Status * objStatus = NULL;
	 getStatus(ST(0), objStatus);

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   if (objStatus) delete objStatus;

   if (cfg) delete cfg;
}



LINKAGE XS(boot_GMSECAPI__Config)
{
   dXSARGS;
   dXCPT;

   USE_ITEMS;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
         newXS("GMSECAPI::Config::Config", _PGA_Config_Create, __FILE__);

	 newXS("GMSECAPI::Config::AddValue", _PGA_Config_AddValue, __FILE__);
	 newXS("GMSECAPI::Config::ClearValue", _PGA_Config_ClearValue, __FILE__);
	 newXS("GMSECAPI::Config::GetValue", _PGA_Config_GetValue, __FILE__);

	 newXS("GMSECAPI::Config::Clear", _PGA_Config_Clear, __FILE__);

	 newXS("GMSECAPI::Config::GetFirst", _PGA_Config_GetFirst, __FILE__);
	 newXS("GMSECAPI::Config::GetNext", _PGA_Config_GetNext, __FILE__);

	 newXS("GMSECAPI::Config::FromXML", _PGA_Config_FromXML, __FILE__);
	 newXS("GMSECAPI::Config::ToXML", _PGA_Config_ToXML, __FILE__);

	 newXS("GMSECAPI::Config::DESTROY", _PGA_Config_DESTROY, __FILE__);

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH


   XSRETURN_YES;
}
