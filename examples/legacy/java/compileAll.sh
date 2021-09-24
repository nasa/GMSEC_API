#!/bin/sh

# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.








echo Building java examples...
javac -classpath ../../bin/gmsecapi.jar:. *.java

