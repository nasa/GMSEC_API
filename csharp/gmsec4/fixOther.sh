#!/usr/bin/env bash

# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


GENERATED_SRC="interfaces/BinaryField.cs"
OLD_TEXT="public SWIGTYPE_p_GMSEC_BinaryData GetValue"
NEW_TEXT="public byte[] GetValue"
sed -i "s/$OLD_TEXT/$NEW_TEXT/g" $GENERATED_SRC
OLD_CTOR="public BinaryField(string name, byte\[\] blob, int length) : this(GmsecPINVOKE.new_BinaryField__SWIG_0(name, blob, length), true) {"
NEW_CTOR="public BinaryField(string name, byte\[\] blob) : this(GmsecPINVOKE.new_BinaryField__SWIG_0(name, blob, blob.Length), true) {"
sed -i "s/$OLD_CTOR/$NEW_CTOR/g" $GENERATED_SRC


GENERATED_SRC="interfaces/ConfigFile.cs"
OLD_TEXT="public SWIGTYPE_p_gmsec__api__ConfigFile__SubscriptionEntry LookupSubscriptionEntry(string name) {"
NEW_TEXT="public SubscriptionEntry LookupSubscriptionEntry(string name) {"
sed -i "s/$OLD_TEXT/$NEW_TEXT/g" $GENERATED_SRC
OLD_TEXT="SWIGTYPE_p_gmsec__api__ConfigFile__SubscriptionEntry ret = new SWIGTYPE_p_gmsec__api__ConfigFile__SubscriptionEntry(GmsecPINVOKE.ConfigFile_LookupSubscriptionEntry(swigCPtr, name), false);"
NEW_TEXT="SubscriptionEntry ret = new SubscriptionEntry(GmsecPINVOKE.ConfigFile_LookupSubscriptionEntry(swigCPtr, name), false);"
sed -i "s/$OLD_TEXT/$NEW_TEXT/g" $GENERATED_SRC


GENERATED_SRC="interfaces/Message.cs"
OLD_TEXT="public bool AddField(string name, byte\[\] bin, int len) {"
NEW_TEXT="public bool AddField(string name, byte\[\] bin) {"
sed -i "s/$OLD_TEXT/$NEW_TEXT/g" $GENERATED_SRC
OLD_TEXT="bool ret = GmsecPINVOKE.Message_AddField__SWIG_1(swigCPtr, name, bin, len);"
NEW_TEXT="bool ret = GmsecPINVOKE.Message_AddField__SWIG_1(swigCPtr, name, bin, bin.Length);"
sed -i "s/$OLD_TEXT/$NEW_TEXT/g" $GENERATED_SRC


GENERATED_SRC="interfaces/ProductFile.cs"
OLD_TEXT="public SWIGTYPE_p_GMSEC_BinaryData GetContents"
NEW_TEXT="public byte[] GetContents"
sed -i "s/$OLD_TEXT/$NEW_TEXT/g" $GENERATED_SRC
OLD_CTOR="public ProductFile(string pName, string pDescription, string pVersion, string pFormat, byte\[\] pData, int pFileSize) : this(GmsecPINVOKE.new_ProductFile__SWIG_1(pName, pDescription, pVersion, pFormat, pData, pFileSize), true) {"
NEW_CTOR="public ProductFile(string pName, string pDescription, string pVersion, string pFormat, byte\[\] pData) : this(GmsecPINVOKE.new_ProductFile__SWIG_1(pName, pDescription, pVersion, pFormat, pData, pData.Length), true) {"
sed -i "s/$OLD_CTOR/$NEW_CTOR/g" $GENERATED_SRC

GENERATED_SRC="interfaces/GmsecPINVOKE.cs"
OLD_TEXT="\[global::System.Runtime.InteropServices.DllImport(\"Gmsec\""
NEW_TEXT="\[global::System.Runtime.InteropServices.DllImport(\"libgmsec_csharp\""
sed -i "s/$OLD_TEXT/$NEW_TEXT/g" $GENERATED_SRC

exit 0
