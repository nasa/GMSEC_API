#!/usr/bin/env bash


# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


PY_DOC3=../../GMSEC_SUPPORT/python-3.8.5/bin/pydoc3

if [ ! -f ${PY_DOC3} ]; then
	echo "Error: ${PY_DOC3} not found."
	exit -1
fi

PY3_DIR="../bin/lib/GMSECAPI4"
PY3_MOD="libgmsec_python3.py"
PY3_HTM="libgmsec_python3.html"
PY3_MAN="manual/python3_4x"


# Generate Python3 documentation using pydoc3, and then
# clean up the resulting HTML file to use better colors,
# and to remove obvious C++ keywords/namespaces/etc.
#
${PY_DOC3} -w ${PY3_DIR}/${PY3_MOD} > /dev/null 2>&1


sed -i 's/ee77aa/44467A/g' ${PY3_HTM}		# Background color
sed -i 's/aa55cc/44467A/g' ${PY3_HTM}		# Header color
sed -i 's/ffc8d8/ffdead/g' ${PY3_HTM}		# Frame color for classes

sed -i 's/gmsec::api::Field:://g' ${PY3_HTM}
sed -i 's/gmsec::api::Message:://g' ${PY3_HTM}
sed -i 's/gmsec::api::util:://g' ${PY3_HTM}
sed -i 's/gmsec::api::mist::message:://g' ${PY3_HTM}
sed -i 's/gmsec::api::mist:://g' ${PY3_HTM}
sed -i 's/gmsec::api:://g' ${PY3_HTM}
sed -i 's/PyObject \*/PyObject/g' ${PY3_HTM}
sed -i 's/char&nbsp;const&nbsp;\*/str/g' ${PY3_HTM}
sed -i 's/char const \*/str/g' ${PY3_HTM}
sed -i 's/ &//g' ${PY3_HTM}
sed -i 's/amp;//g' ${PY3_HTM}
sed -i 's/ const//g' ${PY3_HTM}
sed -i 's/ConfigFile:://g' ${PY3_HTM}
sed -i 's/Message \*/Message/g' ${PY3_HTM}
sed -i 's/Field \*/Field/g' ${PY3_HTM}
sed -i 's/ConnectionManagerCallback \*/ConnectionManagerCallback/g' ${PY3_HTM}
sed -i 's/Callback \*/Callback/g' ${PY3_HTM}
sed -i 's/std::list&lt; FieldSpecification \*,std::allocator&lt; FieldSpecification \*gt;gt;/FieldList/g' ${PY3_HTM}
sed -i 's/std::list&lt; MessageSpecification \*,std::allocator&lt; MessageSpecification \*gt;gt;/MessageSpecificationList/g' ${PY3_HTM}
sed -i 's/SubscriptionInfo \*/SubscriptionInfo/g' ${PY3_HTM}
sed -i 's/Connection \*/Connection/g' ${PY3_HTM}
sed -i 's/size_t/int/g' ${PY3_HTM}
sed -i 's/std::string/str/g' ${PY3_HTM}
sed -i '6,16d' ${PY3_HTM}

mkdir -p ${PY3_MAN}

mv ${PY3_HTM} ${PY3_MAN}

exit 0
