#!/usr/bin/env bash

# Copyright 2007-2022 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

systype=$(uname -s)

if [[ "$systype" == "Darwin" || "$systype" == "Mac" ]]; then

    echo
    echo
    echo "##############################################################"
    echo "#"
    echo "#  Patching GMSEC API libraries and executables for macOS"
    echo "#"
    echo "##############################################################"

    pushd bin > /dev/null

    products=( _libgmsec_python3.so \
               libgmsec_activemq395.dylib libgmsec_amqp.dylib libgmsec_artemis.dylib \
               libgmsec_bolt.dylib libgmsec_c2.dylib libgmsec_generic_jms.dylib libgmsec_jni.jnilib libgmsec_mb.dylib \
               MBServer gm_msg_config gmconfig_edit gmhelp gmpub gmreq gmrpl gmsub throughput_pub throughput_sub \
               libgmsec_perl.dylib libgmsec_ruby.bundle )
    for product in ${products[@]}; do
        if [ -f ${product} ]; then
            install_name_tool -change "../bin/libgmsec_api.dylib" "@loader_path/libgmsec_api.dylib" ${product}
        fi
    done


    # _libgmsec_python3.so
    if [ -f _libgmsec_python3.so ]; then
        install_name_tool -change "/Users/gsfc-580-nightrun/nightrun/API-5.x/GMSEC_SUPPORT/python-3.10.7/lib/libpython3.10.dylib" "@loader_path/libpython3.10.dylib" _libgmsec_python3.so
    fi


    old_support1="/Users/nightrun/dave"
    old_support2="/Users/gsfc-580-nightrun/build"
    new_support="/Users/gsfc-580-nightrun/nightrun/API-5.x/GMSEC_SUPPORT"


    # libgmsec_activemq395.dylib
    if [ -f libgmsec_activemq395.dylib ]; then
        install_name_tool -change "${old_support2}/activemq/lib/libactivemq-cpp.19.dylib" "${new_support}/activemq395/lib/libactivemq-cpp.19.dylib" libgmsec_activemq395.dylib
        install_name_tool -change "${old_support2}/activemq/lib/libapr-1.0.dylib" "${new_support}/activemq395/lib/libapr-1.0.dylib" libgmsec_activemq395.dylib
    fi


    # libgmsec_amqp.dylib
    if [ -f libgmsec_amqp.dylib ]; then
        install_name_tool -change "libqpid-proton.2.dylib" "${new_support}/amqp-1.0/lib64/libqpid-proton.2.dylib" libgmsec_amqp.dylib
    fi


    # libgmsec_artemis.dylib
    if [ -f libgmsec_artemis.dylib ]; then
        install_name_tool -change "${old_support2}/activemq/lib/libactivemq-cpp.19.dylib" "${new_support}/activemq395/lib/libactivemq-cpp.19.dylib" libgmsec_artemis.dylib
        install_name_tool -change "${old_support2}/activemq/lib/libapr-1.0.dylib" "${new_support}/activemq395/lib/libapr-1.0.dylib" libgmsec_artemis.dylib
    fi


    popd > /dev/null
fi

exit 0
