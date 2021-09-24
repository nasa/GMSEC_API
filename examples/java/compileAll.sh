#!/usr/bin/env bash

# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


if [ -z $JDK_HOME ]; then
	echo "JDK_HOME is undefined."
	exit 1
fi

echo "Building java examples..."
$JDK_HOME/bin/javac -classpath ../../bin/gmsecapi.jar:. -Xlint:deprecation *.java
