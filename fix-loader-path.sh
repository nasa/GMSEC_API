#!/usr/bin/env bash

# Copyright 2007-2020 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

systype=$(uname -s)

if [[ "$systype" == "Darwin" || "$systype" == "Mac" ]]; then

    echo
    echo
    echo "###########################################################"
    echo "#"
    echo "#  Patching GMSEC API libraries and executables for macOS"
    echo "#"
    echo "###########################################################"

    pushd bin > /dev/null

    products=( _libgmsec_python.so _libgmsec_python3.so \
               libgmsec_activemq384.dylib libgmsec_activemq394.dylib libgmsec_activemq395.dylib libgmsec_amqp.dylib \
               libgmsec_bolt.dylib libgmsec_c2.dylib libgmsec_generic_jms.dylib libgmsec_jni.jnilib libgmsec_mb.dylib libgmsec_perl.dylib \
               MBServer gm_msg_config gmconfig_edit gmhelp gmpub gmreq gmrpl gmsub throughput_pub throughput_sub )
    for product in ${products[@]}; do
        if [ -f ${product} ]; then
            install_name_tool -change "../bin/libGMSECAPI.dylib" "@loader_path/libGMSECAPI.dylib" ${product}
        fi
    done


    products4=( _libgmsec_python3.so )
    for product in ${products4[@]}; do
        if [ -f ${product} ]; then
            install_name_tool -change "/Users/nightrun/dave/SUPPORT/PARTS/MacOSX.x86_64/Python-3.6.5/lib/libpython3.6m.dylib" "/usr/local/opt/python/Frameworks/Python.framework/Versions/3.7/Python" ${product}
        fi
    done


    old_support1="/Users/nightrun/dave"
    old_support2="/Users/gsfc-580-nightrun/build"
    new_support="/Users/gsfc-580-nightrun/nightrun/API/GMSEC_SUPPORT"


    # libgmsec_activemq384.dylib
    if [ -f libgmsec_activemq384.dylib ]; then
        install_name_tool -change "${old_support1}/activemq/lib/libactivemq-cpp.18.dylib" "${new_support}/activemq384/lib/libactivemq-cpp.18.dylib" libgmsec_activemq384.dylib
        install_name_tool -change "${old_support1}/activemq/lib/libapr-1.0.dylib" "${new_support}/activemq384/lib/libapr-1.0.dylib" libgmsec_activemq384.dylib
    fi


    # libgmsec_activemq394.dylib
    if [ -f libgmsec_activemq394.dylib ]; then
        install_name_tool -change "${old_support1}/activemq/lib/libactivemq-cpp.19.dylib" "${new_support}/activemq394/lib/libactivemq-cpp.19.dylib" libgmsec_activemq394.dylib
        install_name_tool -change "${old_support1}/activemq/lib/libapr-1.0.dylib" "${new_support}/activemq394/lib/libapr-1.0.dylib" libgmsec_activemq394.dylib
    fi


    # libgmsec_activemq395.dylib
    if [ -f libgmsec_activemq395.dylib ]; then
        install_name_tool -change "${old_support2}/activemq/lib/libactivemq-cpp.19.dylib" "${new_support}/activemq395/lib/libactivemq-cpp.19.dylib" libgmsec_activemq395.dylib
        install_name_tool -change "${old_support2}/activemq/lib/libapr-1.0.dylib" "${new_support}/activemq395/lib/libapr-1.0.dylib" libgmsec_activemq395.dylib
    fi


    # libgmsec_amqp.dylib
    if [ -f libgmsec_amqp.dylib ]; then
        install_name_tool -change "libqpid-proton.2.dylib" "${new_support}/amqp-1.0/lib64/libqpid-proton.2.dylib" libgmsec_amqp.dylib
    fi


    popd > /dev/null
fi

exit 0
