#!/usr/bin/env bash

# Copyright 2007-2024 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


GENERATED_SRC="interfaces/BinaryField.cs"

OLD_TEXT_1="public SWIGTYPE_p_GmsecBinaryData GetValue"
NEW_TEXT_1="public byte\[\] GetValue"

OLD_TEXT_2="public BinaryField(string name, byte\[\] blob, int length, bool IsHeader) : this(GmsecPINVOKE.new_BinaryField__SWIG_0(name, blob, length, IsHeader), true) {"
NEW_TEXT_2="public BinaryField(string name, byte\[\] blob, bool IsHeader) : this(GmsecPINVOKE.new_BinaryField__SWIG_0(name, blob, blob.Length, IsHeader), true) {"

OLD_TEXT_3="public BinaryField(string name, byte\[\] blob, int length) : this(GmsecPINVOKE.new_BinaryField__SWIG_1(name, blob, length), true) {"
NEW_TEXT_3="public BinaryField(string name, byte\[\] blob) : this(GmsecPINVOKE.new_BinaryField__SWIG_1(name, blob, blob.Length), true) {"

if [[ "$GMSEC_PLATFORM" == "linux"* ]] || [[ "$GMSEC_PLATFORM" == "aarch64" ]]; then

    echo "Patching $GENERATED_SRC for $GMSEC_PLATFORM..."

    sed -i "s/$OLD_TEXT_1/$NEW_TEXT_1/g" $GENERATED_SRC
    sed -i "s/$OLD_TEXT_2/$NEW_TEXT_2/g" $GENERATED_SRC
    sed -i "s/$OLD_TEXT_3/$NEW_TEXT_3/g" $GENERATED_SRC

elif [[ "$GMSEC_PLATFORM" == "macosx"* ]]; then

    echo "Patching $GENERATED_SRC for MacOS..."

    sed -i '' "s/$OLD_TEXT_1/$NEW_TEXT_1/g" $GENERATED_SRC
    sed -i '' "s/$OLD_TEXT_2/$NEW_TEXT_2/g" $GENERATED_SRC
    sed -i '' "s/$OLD_TEXT_3/$NEW_TEXT_3/g" $GENERATED_SRC

else

    echo "Warning: $GENERATED_SRC is not being patched!"

fi


GENERATED_SRC="interfaces/ConfigFile.cs"

OLD_TEXT_1="public void AddSubscriptionEntry(SWIGTYPE_p_gmsec__api5__ConfigFile__SubscriptionEntry entry) {"
NEW_TEXT_1="public void AddSubscriptionEntry(SubscriptionEntry entry) {"

OLD_TEXT_2="GmsecPINVOKE.ConfigFile_AddSubscriptionEntry(swigCPtr, SWIGTYPE_p_gmsec__api5__ConfigFile__SubscriptionEntry.getCPtr(entry));"
NEW_TEXT_2="GmsecPINVOKE.ConfigFile_AddSubscriptionEntry(swigCPtr, SubscriptionEntry.getCPtr(entry));"

OLD_TEXT_3="public SWIGTYPE_p_gmsec__api5__ConfigFile__SubscriptionEntry LookupSubscriptionEntry(string name) {"
NEW_TEXT_3="public SubscriptionEntry LookupSubscriptionEntry(string name) {"

OLD_TEXT_4="SWIGTYPE_p_gmsec__api5__ConfigFile__SubscriptionEntry ret = new SWIGTYPE_p_gmsec__api5__ConfigFile__SubscriptionEntry(GmsecPINVOKE.ConfigFile_LookupSubscriptionEntry(swigCPtr, name), false);"
NEW_TEXT_4="SubscriptionEntry ret = new SubscriptionEntry(GmsecPINVOKE.ConfigFile_LookupSubscriptionEntry(swigCPtr, name), false);"


if [[ "$GMSEC_PLATFORM" == "linux"* ]] || [[ "$GMSEC_PLATFORM" == "aarch64" ]]; then

    sed -i "s/$OLD_TEXT_1/$NEW_TEXT_1/g" $GENERATED_SRC
    sed -i "s/$OLD_TEXT_2/$NEW_TEXT_2/g" $GENERATED_SRC
    sed -i "s/$OLD_TEXT_3/$NEW_TEXT_3/g" $GENERATED_SRC
    sed -i "s/$OLD_TEXT_4/$NEW_TEXT_4/g" $GENERATED_SRC

elif [[ "$GMSEC_PLATFORM" == "macosx"* ]]; then

    sed -i '' "s/$OLD_TEXT_1/$NEW_TEXT_1/g" $GENERATED_SRC
    sed -i '' "s/$OLD_TEXT_2/$NEW_TEXT_2/g" $GENERATED_SRC
    sed -i '' "s/$OLD_TEXT_3/$NEW_TEXT_3/g" $GENERATED_SRC
    sed -i '' "s/$OLD_TEXT_4/$NEW_TEXT_4/g" $GENERATED_SRC

else

    echo "$WRAPPED_SRC is not being patched!"

fi


GENERATED_SRC="interfaces/Message.cs"

OLD_TEXT_1="public bool AddField(string name, byte\[\] blob, int len) {"
NEW_TEXT_1="public bool AddField(string name, byte\[\] blob) {"

OLD_TEXT_2="bool ret = GmsecPINVOKE.Message_AddField__SWIG_1(swigCPtr, name, blob, len);"
NEW_TEXT_2="bool ret = GmsecPINVOKE.Message_AddField__SWIG_1(swigCPtr, name, blob, blob.Length);"

if [[ "$GMSEC_PLATFORM" == "linux"* ]] || [[ "$GMSEC_PLATFORM" == "aarch64" ]]; then

    sed -i "s/$OLD_TEXT_1/$NEW_TEXT_1/g" $GENERATED_SRC
    sed -i "s/$OLD_TEXT_2/$NEW_TEXT_2/g" $GENERATED_SRC

elif [[ "$GMSEC_PLATFORM" == "macosx"* ]]; then

    sed -i '' "s/$OLD_TEXT_1/$NEW_TEXT_1/g" $GENERATED_SRC
    sed -i '' "s/$OLD_TEXT_2/$NEW_TEXT_2/g" $GENERATED_SRC

else

    echo "$WRAPPED_SRC is not being patched!"

fi


GENERATED_SRC="interfaces/GmsecPINVOKE.cs"

OLD_TEXT_1="\[global::System.Runtime.InteropServices.DllImport(\"Gmsec\""
NEW_TEXT_1="\[global::System.Runtime.InteropServices.DllImport(\"libgmsec_csharp\""

if [[ "$GMSEC_PLATFORM" == "linux"* ]] || [[ "$GMSEC_PLATFORM" == "aarch64" ]]; then

    sed -i "s/$OLD_TEXT_1/$NEW_TEXT_1/g" $GENERATED_SRC

elif [[ "$GMSEC_PLATFORM" == "macosx"* ]]; then

    sed -i '' "s/$OLD_TEXT_1/$NEW_TEXT_1/g" $GENERATED_SRC

else

    echo "$WRAPPED_SRC is not being patched!"

fi

exit 0
