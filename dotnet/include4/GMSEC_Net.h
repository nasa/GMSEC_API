/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#pragma warning( disable : 4635 )  // Disable warning messages from native C++ about badly formatted XML in comments
#pragma warning( disable : 4638 )  // Disable warning messages from native C++ about badly formatted XML in comments
#pragma warning( disable : 4641 )  // Disable warning messages from native C++ about badly formatted XML in comments

#ifndef GMSEC_API_GMSEC_NET_H
#define GMSEC_API_GMSEC_NET_H


// Managed
#include <Status_Net.h>
#include <GMSEC_Exception_Net.h>

// C++ STL native
#include <exception>


// Define macros used by most files to handle very often and simple repeated lines of code

namespace GMSEC
{
namespace API
{

// try, just for consistency, use when using manage ends
#define START_MANAGED_CATCH try

	// try, just for consistency, use when using manage finally
#define FINALLY_MANAGED_CATCH finally

// empty catch for .net and native exceptions
#define END_MANAGED_CATCH_NO_OP \
catch (System::Exception^)\
{\
}\
catch (...)\
{\
}

// empty catch for .net and native exceptions and return class/code/reason in status
#define END_MANAGED_CATCH(statusClass, statusCode)\
catch (const std::exception& e)\
{\
	String^ reason = gcnew System::String(e.What());\
	GMSEC::API::Status^ status = gcnew GMSEC::API::Status(statusClass, statusCode, reason);\
	return status;\
}\
catch (System::Exception^ e)\
{\
	GMSEC::API::Status^ status = gcnew GMSEC::API::Status(statusClass, statusCode, e->ToString());\
	return status;\
}\
catch (...)\
{\
	GMSEC::API::Status^ status = gcnew GMSEC::API::Status(statusClass, statusCode, "Managed Wrapper:: Unhandled Exception");\
	return status;\
}

// empty catch for .net and native exceptions and return value
#define END_MANAGED_CATCH_RETURN_VALUE(returnValue)\
catch (const std::exception&)\
{\
	return returnValue;\
}\
catch (System::Exception^)\
{\
	return returnValue;\
}\
catch (...)\
{\
	return returnValue;\
}

// empty catch for .net and native exceptions and return class/code/reason if input value is null
#define RETURN_STATUS_IF_NULLPTR_CHECK(statusClass, statusCode, variable, reason)\
if (variable == nullptr)\
{\
	GMSEC::API::Status^ status = gcnew GMSEC::API::Status(statusClass, statusCode,  reason);\
	return status;\
}

// empty catch for .net and native exceptions and return if input value is null
#define RETURN_IF_NULLPTR_CHECK(variable)\
if (variable == nullptr)\
{\
	return;\
}

	// empty catch for .net and native exceptions and return value if input value is null
#define RETURN_VALUE_IF_NULLPTR_CHECK(variable, value)\
if (variable == nullptr)\
{\
	return value;\
}

// Free char* if not null
#define FREE_HGLOBAL_IF_NOT_NULLPTR(variable)\
if (variable != nullptr)\
{\
	System::Runtime::InteropServices::Marshal::FreeHGlobal(static_cast<System::IntPtr>(variable));\
}


#define THROW_EXCEPTION_IF_NULLPTR(ptr, clazz, code, msg)\
if (ptr== nullptr)\
{\
	throw gcnew GMSEC::API::GMSEC_Exception(clazz, code, msg);\
}

}  // end namespace API
}  // end namespace GMSEC

#endif
