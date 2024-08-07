#!/usr/bin/env bash

# Copyright 2007-2024 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


WRAPPED_SRC=./interfaces/gmsec_nodejs_wrap.cxx


if [[ "$GMSEC_PLATFORM" == "linux"* ]] || [[ "$GMSEC_PLATFORM" == "aarch64" ]]; then

	echo "Patching $WRAPPED_SRC for $GMSEC_PLATFORM..."

	# Additional handling for ensuring that the globally-defined XEntry classes (from ConfigFile) are used
	# Need to include the comma at the end of the replacement string in order to not overwrite the SWIG-generated definition for the (unused) ConfigFile::XEntry classes.
	sed -i 's/SWIGTYPE_p_gmsec__api5__ConfigFile__SubscriptionEntry,/SWIGTYPE_p_SubscriptionEntry,/g' $WRAPPED_SRC
	sed -i 's/SWIGTYPE_p_gmsec__api5__ConfigFile__MessageEntry,/SWIGTYPE_p_MessageEntry,/g' $WRAPPED_SRC
	sed -i 's/SWIGTYPE_p_gmsec__api5__ConfigFile__ConfigEntry,/SWIGTYPE_p_ConfigEntry,/g' $WRAPPED_SRC

	sed -i 's/long longVal = (long) SWIGV8_NUMBER_VALUE(obj);/long long longVal = (long long) SWIGV8_NUMBER_VALUE(obj);/g' $WRAPPED_SRC


elif [[ "$GMSEC_PLATFORM" == "macosx"* ]]; then

	echo "Patching $WRAPPED_SRC for MacOS..."

	# Additional handling for ensuring that the globally-defined XEntry classes (from ConfigFile) are used
	# Need to include the comma at the end of the replacement string in order to not overwrite the SWIG-generated definition for the (unused) ConfigFile::XEntry classes.
	sed -i '' 's/SWIGTYPE_p_gmsec__api5__ConfigFile__SubscriptionEntry,/SWIGTYPE_p_SubscriptionEntry,/g' $WRAPPED_SRC
	sed -i '' 's/SWIGTYPE_p_gmsec__api5__ConfigFile__MessageEntry,/SWIGTYPE_p_MessageEntry,/g' $WRAPPED_SRC
	sed -i '' 's/SWIGTYPE_p_gmsec__api5__ConfigFile__ConfigEntry,/SWIGTYPE_p_ConfigEntry,/g' $WRAPPED_SRC

	sed -i '' 's/long longVal = (long) SWIGV8_NUMBER_VALUE(obj);/long long longVal = (long long) SWIGV8_NUMBER_VALUE(obj);/g' $WRAPPED_SRC

else

	echo "$WRAPPED_SRC is not being patched!"

fi

exit 0
