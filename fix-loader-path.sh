#!/usr/bin/env bash

# Copyright 2007-2019 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.

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

    products=( _libgmsec_python.so _libgmsec_python3.so libgmsec_activemq38.dylib libgmsec_activemq384.dylib \
               libgmsec_activemq39.dylib libgmsec_activemq394.dylib libgmsec_amqp.dylib \
               libgmsec_bolt.dylib libgmsec_c2.dylib libgmsec_generic_jms.dylib libgmsec_jni.jnilib libgmsec_mb.dylib libgmsec_perl.dylib \
               MBServer gm_msg_config gmconfig_edit gmhelp gmpub gmreq gmrpl gmsub throughput_pub throughput_sub )
    for product in ${products[@]}; do
        if [ -f ${product} ]; then
            install_name_tool -change "../bin/libGMSECAPI.dylib" "@loader_path/libGMSECAPI.dylib" ${product}
        fi
    done


    products2=( libGMSECAPI.dylib libexslt.0.dylib libxslt.1.dylib )
    for product in ${products2[@]}; do
        if [ -f ${product} ]; then
            install_name_tool -change "/Users/gsfc-580-nightrun/SUPPORT/PARTS/MacOSX.x86_64/libxml2-2.9.8/lib/libxml2.2.dylib" "@loader_path/libxml2.2.dylib" ${product}
        fi
    done


    products3=( libGMSECAPI.dylib libexslt.0.dylib )
    for product in ${products3[@]}; do
        if [ -f ${product} ]; then
            install_name_tool -change "/Users/gsfc-580-nightrun/SUPPORT/PARTS/MacOSX.x86_64/libxslt-1.1.33/lib/libxslt.1.dylib" "@loader_path/libxslt.1.dylib" ${product}
        fi
    done

    products4=( _libgmsec_python3.so )
    for product in ${products4[@]}; do
        if [ -f ${product} ]; then
            install_name_tool -change "/Users/nightrun/dave/SUPPORT/PARTS/MacOSX.x86_64/Python-3.6.5/lib/libpython3.6m.dylib" "/usr/local/opt/python/Frameworks/Python.framework/Versions/3.7/Python" ${product}
        fi
    done


    old_support="/Users/nightrun/dave"
    new_support="/Users/gsfc-580-nightrun/gmsec/SUPPORT"


    # libgmsec_activemq38.dylib
    if [ -f libgmsec_activemq38.dylib ]; then
        install_name_tool -change "${old_support}/activemq/lib/libactivemq-cpp.18.dylib" "${new_support}/activemq384/lib/libactivemq-cpp.18.dylib" libgmsec_activemq38.dylib
        install_name_tool -change "${old_support}/activemq/lib/libapr-1.0.dylib" "${new_support}/activemq384/lib/libapr-1.0.dylib" libgmsec_activemq38.dylib
    fi


    # libgmsec_activemq39.dylib
    if [ -f libgmsec_activemq39.dylib ]; then
        install_name_tool -change "${old_support}/activemq/lib/libactivemq-cpp.19.dylib" "${new_support}/activemq394/lib/libactivemq-cpp.19.dylib" libgmsec_activemq39.dylib
        install_name_tool -change "${old_support}/activemq/lib/libapr-1.0.dylib" "${new_support}/activemq394/lib/libapr-1.0.dylib" libgmsec_activemq39.dylib
    fi


    # libgmsec_amqp.dylib
    if [ -f libgmsec_amqp.dylib ]; then
        install_name_tool -change "libqpid-proton.2.dylib" "${new_support}/amqp-1.0/lib64/libqpid-proton.2.dylib" libgmsec_amqp.dylib
    fi


    popd > /dev/null
fi

exit 0
