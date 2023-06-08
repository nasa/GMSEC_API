#!/usr/bin/env bash

# Copyright 2007-2023 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

JAVA_11_CLASS_DIR=classes11

JAVA_OPTS="-Xlint:unchecked -Xlint:deprecation --release 8"

SOURCE="src/*.java"

error=0

if [[ "$1" == "clean" ]]; then

	rm -rf $JAVA_11_CLASS_DIR

else

	if [ -z $JDK_HOME ]; then

		echo "JDK_HOME is not defined!"

		error=1

	elif [ -z $GMSEC_API_HOME ]; then

		echo "GMSEC_API_HOME is not defined!"

		error=1

	else

		mkdir -p $JAVA_11_CLASS_DIR

		$JDK_HOME/bin/javac -cp $GMSEC_API_HOME/bin/gmsecapi.jar -d $JAVA_11_CLASS_DIR $JAVA_OPTS $SOURCE

		if [ $? -ne 0 ]; then
			error=1
		fi
	fi

fi

exit $error
