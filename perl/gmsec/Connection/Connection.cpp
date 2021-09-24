
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/*----------------------------------------------------------------------------
 * Connection.cpp
 *
 * Description: Base class for middleware connections made with the
 *    ConnectionFactory class
 *
 *--------------------------------------------------------------------------*/

// GMSEC API and Perl support library
#include "gmsec_perl.h"


#ifndef PERL_OBJECT
#ifndef MULTIPLICITY
LINKAGE GMSEC_XS_EXPORT(void) boot_GMSECAPI__Connection(CV* cv);
#else
LINKAGE GMSEC_XS_EXPORT(void) boot_GMSECAPI__Connection(pTHXo_ CV* cv);
#endif
#else
LINKAGE GMSEC_XS_EXPORT(void) boot_GMSECAPI__Connection(CV *cv, CPerlObj *);
#endif


static PerlInterpreter* m_threadedDispatchInterpreter = NULL;


SV *getPerlMessageObject(Message *msg)
{
	HV *msgHash;
	SV *msgPointer;
	HV *msgStash;
	SV *msgPerlObject;
	IV pointer;

	// create the Perl Message object
	msgHash = newHV();

	pointer = (IV) msg;
	msgPointer = newSVuv(pointer);
	hv_store(msgHash, "pointer", 7, msgPointer, 0);

	msgPerlObject = sv_2mortal(newRV_inc(sv_2mortal((SV *) msgHash)));
	msgStash = gv_stashpv("GMSECAPI::Message", 1);
	sv_bless(msgPerlObject, msgStash);

	return msgPerlObject;
}


GMSEC_XS_STATIC XS(_PGA_Connection_Connect)
{
	Connection *conn = (Connection *) 0;
	Status result;
	dXSARGS;

	if (items != 1)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::Connection] Usage: $conn->Connect();");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	if (!sv_isa((SV*) ST(0), "GMSECAPI::Connection"))
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::Connection] [Connect] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	getObject(ST(0), conn);

	result = conn->Connect();

	storeStatus(ST(0), result);

	if (result.GetCode() == 0)
	{
		XSRETURN_YES;
	}
	else
	{
		XSRETURN_NO;
	}
}


GMSEC_XS_STATIC XS(_PGA_Connection_Disconnect)
{
	Connection *conn = (Connection *) 0;
	Status result;
	dXSARGS;

	if (items != 1)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::Connection] Usage: $conn->Disconnect();");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	if (!sv_isa((SV*) ST(0), "GMSECAPI::Connection"))
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::Connection] [Disconnect] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	getObject(ST(0), conn);

	result = conn->Disconnect();

	storeStatus(ST(0), result);

	if (result.GetCode() == 0)
	{
		XSRETURN_YES;
	}
	else
	{
		XSRETURN_NO;
	}
}


GMSEC_XS_STATIC XS(_PGA_Connection_GetLibraryRootName)
{
	Connection *conn = (Connection *) 0;
	const char *libname;
	dXSARGS;
	Status result;

	if (items != 1)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::Connection] Usage: $libname = $conn->GetLibraryRootName(); ");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	if (!sv_isa((SV*) ST(0), "GMSECAPI::Connection"))
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::Connection] [GetLibraryRootName] first argument is not a blessed object ");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	getObject(ST(0), conn);

	libname = conn->GetLibraryRootName();

	if (libname != NULL)
	{
		XSRETURN_PV(libname);
	}
	else
	{
		XSRETURN_UNDEF;
	}
}


GMSEC_XS_STATIC XS(_PGA_Connection_GetLibraryVersion)
{
	Connection *conn = (Connection *) 0;
	const char *version;
	dXSARGS;
	Status result;

	if (items != 1)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::Connection] Usage: $version = $conn->GetLibraryVersion();");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	if (!sv_isa((SV*) ST(0), "GMSECAPI::Connection"))
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::Connection] [GetLibraryVersion] first argument is not a blessed object ");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	getObject(ST(0), conn);

	version = conn->GetLibraryVersion();

	if (version != NULL)
	{
		XSRETURN_PV(version);
	}
	else
	{
		XSRETURN_UNDEF;
	}
}


GMSEC_XS_STATIC XS(_PGA_Connection_Subscribe)
{
	SV *self;
	Connection *conn = (Connection *) 0;
	Status result;
	const char *subject;
	dXSARGS;

	if (items < 2 || items > 5)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::Connection] Usage: $conn->Subscribe($subject); "
		           "OR $conn->Subscribe($subject, $callbackObject);"
		           " OR $conn->Subscribe($subject, $callbackObject, $objectPackage);"
		           " OR $conn->Subscribe($subject, $callbackObject, $objectPackage,"
		           " $objectMethod);");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	self = (SV *) ST(0);

	if (!sv_isa(self, "GMSECAPI::Connection"))
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::Connection] [Subscribe] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	getObject(self, conn);

	subject = SvPV_nolen(ST(1));

	if (items == 2)
	{
		result = conn->Subscribe(subject);

	}
	else
	{
		if (!SvROK((SV*) ST(2)))
		{
			result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,
			           "[GMSECAPI::Connection] [Subscribe] third argument is not a"
			           " blessed Callback object");
			storeStatus(ST(0), result);
			XSRETURN_UNDEF;
		}

		if (items == 5)
		{
			char *package = SvPV_nolen(ST(3));
			char *method = SvPV_nolen(ST(4));
			PerlCallback *cb;
			cb = new PerlCallback(ST(2), package, method);
			result = conn->Subscribe(subject, cb);

		}
		else if (items == 4)
		{
			char *package = SvPV_nolen(ST(3));
			PerlCallback *cb;
			cb = new PerlCallback(ST(2), package, "OnMessage");
			result = conn->Subscribe(subject, cb);

		}
		else
		{
			PerlCallback *cb;
			cb = new PerlCallback(ST(2));
			result = conn->Subscribe(subject, cb);
		}
	}

	storeStatus(self, result);

	if (result.GetCode() == 0)
	{
		XSRETURN_YES;
	}
	else
	{
		XSRETURN_NO;
	}
}


GMSEC_XS_STATIC XS(_PGA_Connection_UnSubscribe)
{
	Connection *conn = (Connection *) 0;
	const char *subject;
	Status result;
	dXSARGS;

	if (items != 2)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::Connection] Usage: $conn->UnSubscribe($subject);");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	if (!sv_isa((SV*) ST(0), "GMSECAPI::Connection"))
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::Connection] [UnSubscribe] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	getObject(ST(0), conn);

	subject = SvPV_nolen(ST(1));

	result = conn->UnSubscribe(subject);

	storeStatus(ST(0), result);

	if (result.GetCode() == 0)
	{
		XSRETURN_YES;
	}
	else
	{
		XSRETURN_NO;
	}
}


GMSEC_XS_STATIC XS(_PGA_Connection_CreateMessage)
{
	Connection *conn = (Connection *) 0;
	Message *msg = (Message *) 0;
	Config *cfg = (Config *) 0;
	Status result;
	const char *subject;
	GMSEC_MSG_KIND msgKind;
	dXSARGS;

	if (items != 3 && items != 4)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::Connection] Usage: $msg = $conn->CreateMessage($subject, $msgKind);"
		           " OR $msg = $conn->CreateMessage($subject, $msgKind, $cfg);");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	if (!sv_isa((SV*) ST(0), "GMSECAPI::Connection"))
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::Connection] [CreateMessage] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	getObject(ST(0), conn);

	subject = SvPV_nolen(ST(1));
	msgKind = SvUV(ST(2));

	if (items == 3)
	{
		result = conn->CreateMessage(subject, msgKind, msg);
	}

	if (items == 4)
	{
		if (!sv_isa((SV*) ST(3), "GMSECAPI::Config"))
		{
			result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,
			           "[GMSECAPI::Connection] [CreateMessage] third argument"
			           " is not a blessed Config object");
			storeStatus(ST(0), result);
			XSRETURN_UNDEF;
		}

		getObject(ST(3), cfg);

		result = conn->CreateMessage(subject, msgKind, msg, cfg);
	}

	storeStatus(ST(0), result);

	if (result.GetCode() == 0)
	{
		ST(0) = getPerlMessageObject(msg);
		XSRETURN(1);

	}
	else
	{
		XSRETURN_UNDEF;
	}
}


GMSEC_XS_STATIC XS(_PGA_Connection_ConvertMessage)
{
	Connection *conn = (Connection *) 0;
	Message *inMsg = (Message *) 0;
	Message *outMsg = (Message *) 0;
	Status result;
	dXSARGS;

	if (items != 2)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::Connection] Usage: $outMsg = $conn->ConvertMessage($inMsg);");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	if (!sv_isa((SV*) ST(0), "GMSECAPI::Connection"))
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::Connection] [ConvertMessage] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	if (!sv_isa((SV*) ST(1), "GMSECAPI::Message"))
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::Connection] [ConvertMessage] second "
		           "argument is not a blessed Message object");

		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	getObject(ST(0), conn);
	getObject(ST(1), inMsg);

	result = conn->ConvertMessage(inMsg, outMsg);

	storeStatus(ST(0), result);

	if (result.GetCode() == 0)
	{
		ST(0) = getPerlMessageObject(outMsg);
		XSRETURN(1);

	}
	else
	{
		XSRETURN_UNDEF;
	}
}


GMSEC_XS_STATIC XS(_PGA_Connection_ConvertCleanup)
{
	Connection *conn = (Connection *) 0;
	Message *inMsg = (Message *) 0;
	Message *outMsg = (Message *) 0;
	Status result;
	dXSARGS;

	if (items != 3)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Connection] Usage: $conn->ConvertCleanup($inMsg, $outMsg);");
		XSRETURN_UNDEF;
	}

	if (!sv_isa((SV*) ST(0), "GMSECAPI::Connection"))
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Connection] [ConvertCleanup] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	if (!sv_isa((SV*) ST(1), "GMSECAPI::Message"))
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Connection] [ConvertCleanup] second argument is not a blessed Message object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	if (!sv_isa((SV*) ST(2), "GMSECAPI::Message"))
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Connection] [ConvertCleanup] third argument is not a blessed Message object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	getObject(ST(0), conn);
	getObject(ST(1), inMsg);
	getObject(ST(2), outMsg);

	result = conn->ConvertCleanup(inMsg, outMsg);

	storeStatus(ST(0), result);

	if (result.GetCode() == 0)
	{
		XSRETURN_YES;
	}
	else
	{
		XSRETURN_NO;
	}
}


GMSEC_XS_STATIC XS(_PGA_Connection_CloneMessage)
{
	Connection *conn = (Connection *) 0;
	Message *inMsg = (Message *) 0;
	Message *outMsg = (Message *) 0;
	Status result;
	dXSARGS;

	if (items != 2)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Connection] Usage: $outMsg = $conn->CloneMessage($inMsg);");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	if (!sv_isa((SV*) ST(0), "GMSECAPI::Connection"))
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Connection] [CloneMessage] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	if (!sv_isa((SV*) ST(1), "GMSECAPI::Message"))
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Connection] [CloneMessage] second argument is not a blessed Message object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	getObject(ST(0), conn);
	getObject(ST(1), inMsg);

	result = conn->CloneMessage(inMsg, outMsg);

	storeStatus(ST(0), result);

	if (result.GetCode() == 0)
	{
		ST(0) = getPerlMessageObject(outMsg);
		XSRETURN(1);

	}
	else
	{
		XSRETURN_UNDEF;
	}
}


GMSEC_XS_STATIC XS(_PGA_Connection_DestroyMessage)
{
	Connection *conn = (Connection *) 0;
	Message *msg = (Message *) 0;
	Status result;
	dXSARGS;

	if (items != 2)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Connection] Usage: $conn->DestroyMessage($msg);");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	if (!sv_isa((SV*) ST(0), "GMSECAPI::Connection"))
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Connection] [DestroyMessage] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	if (!sv_isa((SV*) ST(1), "GMSECAPI::Message"))
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Connection] [DestroyMessage] second argument is not a blessed Message object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	getObject(ST(0), conn);
	getObject(ST(1), msg);

	Status * msgStatus = NULL;
	getStatus(ST(1), msgStatus);
	if (msgStatus) delete msgStatus;

	result = conn->DestroyMessage(msg);

	storeStatus(ST(0), result);

	if (result.GetCode() == 0)
	{
		XSRETURN_YES;
	}
	else
	{
		XSRETURN_NO;
	}
}


GMSEC_XS_STATIC XS(_PGA_Connection_Publish)
{
	Connection *conn = (Connection *) 0;
	Message *msg = (Message *) 0;
	Status result;
	dXSARGS;

	if (items != 2)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Connection] Usage: $conn->Publish($msg);");
		XSRETURN_UNDEF;
	}

	if (!sv_isa((SV*) ST(0), "GMSECAPI::Connection"))
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Connection] [Publish] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	if (!sv_isa((SV*) ST(1), "GMSECAPI::Message"))
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Connection] [Publish] second argument is not a blessed Message object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	getObject(ST(0), conn);
	getObject(ST(1), msg);

	result = conn->Publish(msg);

	storeStatus(ST(0), result);

	if (result.GetCode() == 0)
	{
		XSRETURN_YES;
	}
	else
	{
		XSRETURN_NO;
	}
}


GMSEC_XS_STATIC XS(_PGA_Connection_Request)
{
	SV *self;
	Connection *conn = (Connection *) 0;
	Message *request = (Message *) 0;
	long timeout;
	Message *reply = (Message *) 0;
	Status result;
	dXSARGS;

	if (items < 3 || items > 6)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Connection] Usage: $reply = $conn->Request($request, $timeout); OR $conn->Request($request, $timeout, $callbackObject); OR $conn->Request($request, $timeout, $callbackObject, $objectPackage): OR $conn->Request($request, $timeout, $callbackObject, $objectPackage, $objectMethod);");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	self = (SV *) ST(0);

	if (!sv_isa(self, "GMSECAPI::Connection"))
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Connection] [Request] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	if (!sv_isa((SV*) ST(1), "GMSECAPI::Message"))
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Connection] [Request] second argument is not a blessed Message object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	getObject(self, conn);
	getObject(ST(1), request);
	timeout = SvIV(ST(2));

	if (items == 3)
	{
		// reply instantly
		result = conn->Request(request, timeout, reply);

		storeStatus(self, result);

		if (result.GetCode() == 0)
		{
			ST(0) = getPerlMessageObject(reply);
			XSRETURN(1);

		}
		else
		{
			XSRETURN_UNDEF;
		}

	}
	else
	{
		// register callback
		if (!SvROK((SV*) ST(3)))
		{
			result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Connection] [Request] fourth argument is not a blessed object");
			storeStatus(ST(0), result);
			XSRETURN_UNDEF;
		}

		if (items == 6)
		{
			char *package = SvPV_nolen(ST(4));
			char *method = SvPV_nolen(ST(5));
			PerlCallback *cb;
			cb = new PerlCallback(ST(3), package, method);
			result = conn->Request(request, timeout, cb);

		}
		else if (items == 5)
		{
			char *package = SvPV_nolen(ST(4));
			PerlCallback *cb;
			cb = new PerlCallback(ST(3), package, "OnMessage");
			result = conn->Request(request, timeout, cb);

		}
		else
		{
			PerlCallback *cb;
			cb = new PerlCallback(ST(3));
			result = conn->Request(request, timeout, cb);
		}

		storeStatus(self, result);

		if (result.GetCode() == 0)
		{
			XSRETURN_YES;
		}
		else
		{
			XSRETURN_NO;
		}
	}
}


GMSEC_XS_STATIC XS(_PGA_Connection_Reply)
{
	Connection *conn = (Connection *) 0;
	Message *request = (Message *) 0;
	Message *reply = (Message *) 0;
	Status result;
	dXSARGS;

	if (items != 3)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Connection] Usage: $conn->Reply($request, $reply);");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	if (!sv_isa((SV*) ST(0), "GMSECAPI::Connection"))
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Connection] [Reply] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	if (!sv_isa((SV*) ST(1), "GMSECAPI::Message"))
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Connection] [Reply] second argument is not a blessed Message object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	if (!sv_isa((SV*) ST(2), "GMSECAPI::Message"))
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Connection] [Reply] third argument is not a blessed Message object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	getObject(ST(0), conn);
	getObject(ST(1), request);
	getObject(ST(2), reply);

	result = conn->Reply(request, reply);

	storeStatus(ST(0), result);

	if (result.GetCode() == 0)
	{
		XSRETURN_YES;
	}
	else
	{
		XSRETURN_NO;
	}
}


GMSEC_XS_STATIC XS(_PGA_Connection_StartAutoDispatch)
{
	Connection *conn = (Connection *) 0;
	Status result;
	dXSARGS;

	if (items != 1)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Connection] Usage: $conn->StartAutoDispatch();");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	if (!sv_isa((SV*) ST(0), "GMSECAPI::Connection"))
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Connection] [StartAutoDispatch] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	getObject(ST(0), conn);

	result = conn->StartAutoDispatch();
	storeStatus(ST(0), result);

	if (result.GetCode() == 0)
	{
		XSRETURN_YES;
	}
	else
	{
		XSRETURN_NO;
	}
}


GMSEC_XS_STATIC XS(_PGA_Connection_SetDispatchContext)
{
	Status result;
	dXSARGS;

	if (items != 1)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Connection] Usage: $conn->SetDispatchContext();");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	if (!sv_isa((SV*) ST(0), "GMSECAPI::Connection"))
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Connection] [StartAutoDispatch] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	setDispatchInterpreter(my_perl);

	XSRETURN_YES;
}


GMSEC_XS_STATIC XS(_PGA_Connection_SetThreadedDispatchContext)
{
	Status result;
	dXSARGS;

	if (items != 1)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Connection] Usage: $conn->SetThreadedDispatchContext();");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	if (!sv_isa((SV*) ST(0), "GMSECAPI::Connection"))
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Connection] [SetThreadedDispatchContext] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	m_threadedDispatchInterpreter = my_perl;

	XSRETURN_YES;
}


GMSEC_XS_STATIC XS(_PGA_Connection_ChooseThreadedDispatchContext)
{
	Status result;
	dXSARGS;

	if (items != 1)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Connection] Usage: $conn->SetThreadedDispatchContext();");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	if (!sv_isa((SV*) ST(0), "GMSECAPI::Connection"))
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Connection] [SetThreadedDispatchContext] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	setDispatchInterpreter(m_threadedDispatchInterpreter);

	XSRETURN_YES;
}


GMSEC_XS_STATIC XS(_PGA_Connection_StopAutoDispatch)
{
	Connection *conn = (Connection *) 0;
	Status result;
	dXSARGS;

	if (items != 1)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Connection] Usage: $conn->StopAutoDispatch();");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	if (!sv_isa((SV*) ST(0), "GMSECAPI::Connection"))
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Connection] [StopAutoDispatch] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	getObject(ST(0), conn);

	result = conn->StopAutoDispatch();

	storeStatus(ST(0), result);

	if (result.GetCode() == 0)
	{
		XSRETURN_YES;
	}
	else
	{
		XSRETURN_NO;
	}
}


GMSEC_XS_STATIC XS(_PGA_Connection_GetNextMsg)
{
	Connection *conn = (Connection *) 0;
	Message *msg = (Message *) 0;
	Status result;
	long timeout;
	dXSARGS;

	if (items != 2)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Connection] Usage: $msg = $conn->GetNextMsg($timeout);");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	if (!sv_isa((SV*) ST(0), "GMSECAPI::Connection"))
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Connection] [GetNextMsg] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	getObject(ST(0), conn);
	timeout = SvIV(ST(1));

	result = conn->GetNextMsg(msg, timeout);

	storeStatus(ST(0), result);

	if (result.GetCode() == 0)
	{
		ST(0) = getPerlMessageObject(msg);
		XSRETURN(1);

	}
	else
	{
		XSRETURN_UNDEF;
	}
}


GMSEC_XS_STATIC XS(_PGA_Connection_DispatchMsg)
{
	SV *self;
	Connection *conn = (Connection *) 0;
	Message *msg = (Message *) 0;
	Status result;
	dXSARGS;


	if (items != 2)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Connection] Usage: $msg = $conn->DispatchMsg($msg);");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	self = (SV*) ST(0);

	if (!sv_isa(self, "GMSECAPI::Connection"))
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Connection] [DispatchMsg] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	if (!sv_isa((SV*) ST(1), "GMSECAPI::Message"))
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Connection] [DispatchMsg] second argument is not a blessed Message object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}
	getObject(self, conn);
	getObject(ST(1), msg);

	result = conn->DispatchMsg(msg);

	storeStatus(self, result);
	if (result.GetCode() == 0)
	{
		XSRETURN_YES;
	}
	else
	{
		XSRETURN_NO;
	}
}

GMSEC_XS_STATIC XS(_PGA_Connection_IsConnected)
{
	Connection *conn = (Connection *) 0;
	int retVal = 0;
	Status result;
	dXSARGS;


	if (items != 1)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,
		           "{GMSECAPI::Connection} Usage: [boolean] = $conn->IsConnected();");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	if (!sv_isa((SV*) ST(0), "GMSECAPI::Connection"))
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::Connection] [GetLastDispatcherStatus] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	getObject(ST(0), conn);

	retVal = conn->IsConnected();

	XSRETURN_IV(retVal);
}


GMSEC_XS_STATIC XS(_PGA_Connection_GetLastDispatcherStatus)
{
	Connection *conn = (Connection *) 0;
	Status result;
	dXSARGS;

	if (items != 1)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Connection] Usage: $status = $conn->GetLastDispatcherStatus();");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}

	if (!sv_isa((SV*) ST(0), "GMSECAPI::Connection"))
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_OTHER_ERROR,"[GMSECAPI::Connection] [GetLastDispatcherStatus] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	}


	getObject(ST(0), conn);

	result = conn->GetLastDispatcherStatus();

	HV *stHash;
	SV *stPointer;
	HV *stStash;
	SV *stPerlObject;
	IV pointer;

	// create the Perl Status object to return
	stHash = newHV();

	pointer = (IV) new Status(result);
	stPointer = newSVuv(pointer);
	hv_store(stHash, "pointer", 7, stPointer, 0);

	stPerlObject = sv_2mortal(newRV_inc(sv_2mortal((SV *) stHash)));
	stStash = gv_stashpv("GMSECAPI::Status", 1);
	sv_bless(stPerlObject, stStash);

	ST(0) = stPerlObject;
	XSRETURN(1);
}


LINKAGE XS(boot_GMSECAPI__Connection)
{
	dXSARGS;

	USE_ITEMS;

	newXS("GMSECAPI::Connection::ConnectXS", _PGA_Connection_Connect, __FILE__);
	newXS("GMSECAPI::Connection::Disconnect", _PGA_Connection_Disconnect, __FILE__);

	newXS("GMSECAPI::Connection::GetLibraryRootName", _PGA_Connection_GetLibraryRootName, __FILE__);
	newXS("GMSECAPI::Connection::GetLibraryVersion", _PGA_Connection_GetLibraryVersion, __FILE__);

	newXS("GMSECAPI::Connection::Subscribe", _PGA_Connection_Subscribe, __FILE__);
	newXS("GMSECAPI::Connection::UnSubscribe", _PGA_Connection_UnSubscribe, __FILE__);

	newXS("GMSECAPI::Connection::CreateMessage", _PGA_Connection_CreateMessage, __FILE__);

	newXS("GMSECAPI::Connection::ConvertMessage", _PGA_Connection_ConvertMessage, __FILE__);
	newXS("GMSECAPI::Connection::ConvertCleanup", _PGA_Connection_ConvertCleanup, __FILE__);

	newXS("GMSECAPI::Connection::CloneMessage", _PGA_Connection_CloneMessage, __FILE__);
	newXS("GMSECAPI::Connection::DestroyMessage", _PGA_Connection_DestroyMessage, __FILE__);

	newXS("GMSECAPI::Connection::Publish", _PGA_Connection_Publish, __FILE__);
	newXS("GMSECAPI::Connection::Request", _PGA_Connection_Request, __FILE__);
	newXS("GMSECAPI::Connection::Reply", _PGA_Connection_Reply, __FILE__);

	newXS("GMSECAPI::Connection::StartAutoDispatchXS", _PGA_Connection_StartAutoDispatch, __FILE__);
	newXS("GMSECAPI::Connection::SetDispatchContext", _PGA_Connection_SetDispatchContext, __FILE__);
	newXS("GMSECAPI::Connection::SetThreadedDispatchContext", _PGA_Connection_SetThreadedDispatchContext, __FILE__);
	newXS("GMSECAPI::Connection::ChooseThreadedDispatchContext", _PGA_Connection_ChooseThreadedDispatchContext, __FILE__);
	newXS("GMSECAPI::Connection::StopAutoDispatchXS", _PGA_Connection_StopAutoDispatch, __FILE__);

	newXS("GMSECAPI::Connection::GetNextMsg", _PGA_Connection_GetNextMsg, __FILE__);

	newXS("GMSECAPI::Connection::DispatchMsg", _PGA_Connection_DispatchMsg, __FILE__);
	newXS("GMSECAPI::Connection::IsConnected", _PGA_Connection_IsConnected, __FILE__);

	newXS("GMSECAPI::Connection::GetLastDispatcherStatus", _PGA_Connection_GetLastDispatcherStatus, __FILE__);

	XSRETURN_YES;
}

