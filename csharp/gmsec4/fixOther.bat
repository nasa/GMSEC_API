@ECHO OFF


rem Copyright 2007-2021 United States Government as represented by the
rem Administrator of The National Aeronautics and Space Administration.
rem No copyright is claimed in the United States under Title 17, U.S. Code.
rem All Rights Reserved.


set GENERATED_SRC=interfaces/BinaryField.cs
set OLD_TEXT=public SWIGTYPE_p_GMSEC_BinaryData GetValue
set NEW_TEXT=public byte\[\] GetValue
perl -pi.bak -e "s/%OLD_TEXT%/%NEW_TEXT%/" %GENERATED_SRC%

set OLD_CTOR=public BinaryField\(string name, byte\[\] blob, int length\) : this\(GmsecPINVOKE.new_BinaryField__SWIG_0\(name, blob, length\), true\) \{
set NEW_CTOR=public BinaryField\(string name, byte\[\] blob\) : this\(GmsecPINVOKE.new_BinaryField__SWIG_0\(name, blob, blob.Length\), true\) \{
perl -pi.bak -e "s/%OLD_CTOR%/%NEW_CTOR%/" %GENERATED_SRC%

set GENERATED_SRC=interfaces/ConfigFile.cs
set OLD_TEXT=public SWIGTYPE_p_gmsec__api__ConfigFile__SubscriptionEntry LookupSubscriptionEntry\(string name\) \{
set NEW_TEXT=public SubscriptionEntry LookupSubscriptionEntry\(string name\) \{
perl -pi.bak -e "s/%OLD_TEXT%/%NEW_TEXT%/" %GENERATED_SRC%

set OLD_TEXT=SWIGTYPE_p_gmsec__api__ConfigFile__SubscriptionEntry ret = new SWIGTYPE_p_gmsec__api__ConfigFile__SubscriptionEntry\(GmsecPINVOKE.ConfigFile_LookupSubscriptionEntry\(swigCPtr, name\), false\);
set NEW_TEXT=SubscriptionEntry ret = new SubscriptionEntry\(GmsecPINVOKE.ConfigFile_LookupSubscriptionEntry\(swigCPtr, name\), false\);
perl -pi.bak -e "s/%OLD_TEXT%/%NEW_TEXT%/" %GENERATED_SRC%


set GENERATED_SRC=interfaces/Message.cs
set OLD_TEXT=public bool AddField\(string name, byte\[\] bin, int len\) \{
set NEW_TEXT=public bool AddField\(string name, byte\[\] bin\) \{
perl -pi.bak -e "s/%OLD_TEXT%/%NEW_TEXT%/" %GENERATED_SRC%

set OLD_TEXT=bool ret = GmsecPINVOKE.Message_AddField__SWIG_1\(swigCPtr, name, bin, len\);
set NEW_TEXT=bool ret = GmsecPINVOKE.Message_AddField__SWIG_1\(swigCPtr, name, bin, bin.Length\);
perl -pi.bak -e "s/%OLD_TEXT%/%NEW_TEXT%/" %GENERATED_SRC%


set GENERATED_SRC=interfaces/ProductFile.cs
set OLD_TEXT=public SWIGTYPE_p_GMSEC_BinaryData GetContents
set NEW_TEXT=public byte\[\] GetContents
perl -pi.bak -e "s/%OLD_TEXT%/%NEW_TEXT%/" %GENERATED_SRC%

set OLD_CTOR=public ProductFile\(string pName, string pDescription, string pVersion, string pFormat, byte\[\] pData, int pFileSize\) : this\(GmsecPINVOKE.new_ProductFile__SWIG_1\(pName, pDescription, pVersion, pFormat, pData, pFileSize\), true\) \{
set NEW_CTOR=public ProductFile\(string pName, string pDescription, string pVersion, string pFormat, byte\[\] pData\) : this\(GmsecPINVOKE.new_ProductFile__SWIG_1\(pName, pDescription, pVersion, pFormat, pData, pData.Length\), true\) \{
perl -pi.bak -e "s/%OLD_CTOR%/%NEW_CTOR%/" %GENERATED_SRC%

set GENERATED_SRC=interfaces/GmsecPINVOKE.cs
set OLD_TEXT=\[global::System.Runtime.InteropServices.DllImport\(\"Gmsec\"
set NEW_TEXT=\[global::System.Runtime.InteropServices.DllImport\(\"libgmsec_csharp\"
perl -pi.bak -e "s/%OLD_TEXT%/%NEW_TEXT%/" %GENERATED_SRC%

