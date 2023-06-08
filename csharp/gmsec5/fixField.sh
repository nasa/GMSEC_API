#!/usr/bin/env bash

# Copyright 2007-2023 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


# Replace instances where a new Field object is created with a
# call to our factory method that constructs the desired Field
# subsclass.
#
GENERATED_SRCS=$( ls interfaces/*.cs )
OLD_TEXT="new Field("
NEW_TEXT="FieldFactory.BuildField("

if [[ "$GMSEC_PLATFORM" == "linux"* ]] || [[ "$GMSEC_PLATFORM" == "aarch64" ]]; then

    echo "Patching generated C# source code for $GMSEC_PLATFORM..."

	for src in ${GENERATED_SRCS[@]}; do
		sed -i "s/$OLD_TEXT/$NEW_TEXT/g" $src
	done

elif [[ "$GMSEC_PLATFORM" == "macosx"* ]]; then

    echo "Patching generated C# source code for $GMSEC_PLATFORM..."

	for src in ${GENERATED_SRCS[@]}; do
		sed -i '' "s/$OLD_TEXT/$NEW_TEXT/g" $src
	done

else

    echo "Warning: Generated C# source code is not being patched!"

fi


# Finally, copy custom Field Factory to interfaces directory
#
cp FieldFactory.cs interfaces/

exit 0
