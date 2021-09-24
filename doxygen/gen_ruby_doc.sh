#!/usr/bin/env bash


# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


RDOC=/usr/bin/rdoc

if [ ! -f ${RDOC} ]; then
	echo "Error: ${RDOC} not found."
	exit -1
fi

RUBY_DIR="../ruby/gmsec4/dox"
RUBY_MAN="manual/ruby_4x"

API_VERSION=$(cat ../version.inf)


# Generate Ruby documentation using rdoc.
#
if [ -d ${RUBY_MAN} ]; then
	echo "Removing existing ${RUBY_MAN} directory."
	rm -rf ${RUBY_MAN}
fi

${RDOC} -t "GMSEC API ${API_VERSION} Ruby Binding" -o ${RUBY_MAN} ${RUBY_DIR}/*.rb > /dev/null 2>&1

exit 0
