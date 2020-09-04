#!/usr/bin/env bash


WRAPPED_SRC=./interfaces/libgmsec_python_wrap.cxx


if [[ "$GMSEC_PLATFORM" == "linux"* ]]; then

	echo "Patching $WRAPPED_SRC for Linux..."

	# Additional handling for ensuring that the globally-defined XEntry classes (from ConfigFile) are used
	# Need to include the comma at the end of the replacement string in order to not overwrite the SWIG-generated definition for the (unused) ConfigFile::XEntry classes.
	sed -i 's/SWIGTYPE_p_gmsec__api__ConfigFile__SubscriptionEntry,/SWIGTYPE_p_SubscriptionEntry,/g' $WRAPPED_SRC
	sed -i 's/SWIGTYPE_p_gmsec__api__ConfigFile__MessageEntry,/SWIGTYPE_p_MessageEntry,/g' $WRAPPED_SRC
	sed -i 's/SWIGTYPE_p_gmsec__api__ConfigFile__ConfigEntry,/SWIGTYPE_p_ConfigEntry,/g' $WRAPPED_SRC

	sed -i 's/<gmsec4\/Exception.h>/<gmsec4\/Exception.h>\n\n\#include <gmsec4\/util\/Deprecated.h>\nGMSEC_DISABLE_DEPRECATED_WARNINGS\n/g' $WRAPPED_SRC
    echo GMSEC_ENABLE_DEPRECATED_WARNINGS >> $WRAPPED_SRC

elif [[ "$GMSEC_PLATFORM" == "macosx"* ]]; then

	echo "Patching $WRAPPED_SRC for OS X..."

	# Additional handling for ensuring that the globally-defined XEntry classes (from ConfigFile) are used
	# Need to include the comma at the end of the replacement string in order to not overwrite the SWIG-generated definition for the (unused) ConfigFile::XEntry classes.
	sed -i '' 's/SWIGTYPE_p_gmsec__api__ConfigFile__SubscriptionEntry,/SWIGTYPE_p_SubscriptionEntry,/g' $WRAPPED_SRC
	sed -i '' 's/SWIGTYPE_p_gmsec__api__ConfigFile__MessageEntry,/SWIGTYPE_p_MessageEntry,/g' $WRAPPED_SRC
	sed -i '' 's/SWIGTYPE_p_gmsec__api__ConfigFile__ConfigEntry,/SWIGTYPE_p_ConfigEntry,/g' $WRAPPED_SRC

	sed -i '' 's/<gmsec4\/Exception.h>/<gmsec4\/Exception.h> \
	#include <gmsec4\/util\/Deprecated.h> \
	GMSEC_DISABLE_DEPRECATED_WARNINGS \
	/g' $WRAPPED_SRC

    echo GMSEC_ENABLE_DEPRECATED_WARNINGS >> $WRAPPED_SRC

else

	echo "$WRAPPED_SRC is not being patched!"

fi

exit 0
