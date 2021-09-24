
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/*
* Classification
*  
* UNCLASSIFIED
*
*/
#pragma warning( disable : 4635 )  // Disable warning messages from native C++ about badly formatted XML in comments
#pragma warning( disable : 4638 )  // Disable warning messages from native C++ about badly formatted XML in comments
#pragma warning( disable : 4641 )  // Disable warning messages from native C++ about badly formatted XML in comments

#ifndef GMSEC_Net
#define GMSEC_Net

// managed
#include "Status_Net.h"

// native
#include <exception>

using namespace System;

// Define macros used by most files to handle very often and simple repeated lines of code

namespace Nasa
{
namespace Gmsec
{
namespace Net
{

// try, just for consistency, use when using manage ends
#define STARTMANAGEDCATCH try

	// try, just for consistency, use when using manage finally
#define FINALLYMANAGEDCATCH finally

// empty catch for .net and native exceptions
#define ENDMANAGEDCATCHNOOP \
catch (Exception^)\
{\
}\
catch (...)\
{\
}

// empty catch for .net and native exceptions and return class/code/error in status
#define ENDMANAGEDCATCH(errorClass, errorCode)\
catch (const std::exception& e)\
{\
String^ error = gcnew System::String(e.what());\
Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status(errorClass, errorCode, error);\
return status;\
}\
catch (Exception^ e)\
{\
Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status(errorClass, errorCode, e->ToString());\
return status;\
}\
catch (...)\
{\
Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status(errorClass, errorCode, "Managed Wrapper:: Unhandled Exception");\
return status;\
}

// empty catch for .net and native exceptions and return value
#define ENDMANAGEDCATCHRETURNVALUE(returnValue)\
catch (const std::exception&)\
{\
return returnValue;\
}\
catch (Exception^)\
{\
return returnValue;\
}\
catch (...)\
{\
return returnValue;\
}

// empty catch for .net and native exceptions and return class/code/error if input value is null
#define RETURN_STATUS_IF_NULLPTR_CHECK(errorClass, errorCode, variable, message)\
if (variable == nullptr)\
{\
Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status(errorClass, errorCode,  message);\
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
System::Runtime::InteropServices::Marshal::FreeHGlobal(static_cast<IntPtr>(variable));\
}

}
}
}

#endif //GMSEC_Net
