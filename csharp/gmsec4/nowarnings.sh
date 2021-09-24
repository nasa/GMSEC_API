#!/usr/bin/env bash

# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

WRAPPED_SRC="interfaces/Gmsec_wrap.cxx"

if [ ! -f $WRAPPED_SRC ]; then
	echo "Error; cannot find $WRAPPED_SRC"
	exit 1
fi

if [[ "$GMSEC_PLATFORM" == "linux"* ]]; then

	echo "Patching $WRAPPED_SRC for Linux..."

	sed -i 's/<gmsec_version.h>/<gmsec_version.h>\n\n\#include <gmsec4\/util\/Deprecated.h>\nGMSEC_DISABLE_DEPRECATED_WARNINGS\n/g' $WRAPPED_SRC

elif [[ "$GMSEC_PLATFORM" == "macosx"* ]]; then

	echo "Patching $WRAPPED_SRC for macOS"

    sed -i '' 's/<gmsec_version.h>/<gmsec_version.h> \
    #include <gmsec4\/util\/Deprecated.h> \
    GMSEC_DISABLE_DEPRECATED_WARNINGS \
    /g' $WRAPPED_SRC

else

	echo "$WRAPPED_SRC is not being patched!"

fi

exit 0
