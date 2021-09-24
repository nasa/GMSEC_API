#!/usr/bin/env bash

# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


function ShowUsage
{
	echo "Usage: $0 <mw> [check_JMS]"
	echo
	echo "where <mw> is one of the following:"
	echo
	echo "    activemq39"
	echo "    amqp"
	echo "    artemis"
	echo "    bolt"
	echo "    ibmmq90"
	echo "    mb"
	echo "    zeromq413"
	echo
	echo "Note: It is also acceptable to preface any of the above middleware types with"
	echo "      'gmsec_'.  For example, 'gmsec_activemq39'."
	echo
	echo
	echo "The [check_JMS] argument is optional and can be used to check if the"
	echo "installed Java Runtime Environment is compatible with the GMSEC JMS wrapper"
	echo "and whether the appropriate JRE libraries have been found."
	echo
}

function Success
{
	echo "${txtgrn}    +++ SUCCESS: $1 ${txtrst}"
}

function Failure
{
	echo "${txtred}    --- FAILURE: $1 ${txtrst}"
}

function Warning
{
	echo "${txtyel}    ~~~ WARNING: $1 ${txtrst}"
}

function myreadlink()
{
	if [ ! -h "$1" ]; then
		echo "$1"
	else
		local link="$(expr "$(command ls -ld -- "$1")" : '.*-> \(.*\)$')"
		local link_dir=$(dirname $link)
		if [[ "$link_dir" == "." ]]; then
			link="$(dirname $1)/$link"
		fi
		myreadlink "$link";
	fi
}

function FindInPath
{
	path_found=""

	IFS=":"
	for dir in $PATH; do
		if [ -e $dir/$1 ]; then
			path_found=$dir
			break
		fi
	done
	unset IFS

	if [ "$path_found" == "" ]; then
		Warning "$1 not found in PATH"
	else
		Success "$1 found in $path_found using PATH"
	fi
}

function FindDirInPath
{
	path_found=""

	IFS=":"
	for dir in $PATH; do
		if [ "$dir" == "$1" ]; then
			path_found=$dir
			break
		fi
	done
	unset IFS

	if [ "$path_found" == "" ]; then
		Warning "$1 not found in PATH"
	else
		Success "$1 found in PATH"
	fi
}

function CheckDependencies
{
	lib=$1
	critical=$2
	baselib=$(basename $lib)

	ldd_out=$($ldd_cmd $ldd_opt $lib 2>&1)

	if [ $? -eq 0 ]; then
		Success "Found $baselib"

		echo
		echo "${txtcyn}Checking dependencies for $baselib...${txtrst}"

		case "$sys_type" in
			Linux)
				IFS=$'\n'
				for entry in ${ldd_out[@]}; do
					echo $entry | grep "not found" > /dev/null

					if [ $? -eq 0 ]; then
						unset IFS

						fields=($entry)
						if [[ $critical == true ]]; then
							Failure "Unable to reference ${fields[0]} using LD_LIBRARY_PATH"
							result=1
						else
							Warning "Unable to reference ${fields[0]} using LD_LIBRARY_PATH"
						fi

						IFS=$'\n'
					else
						IFS=" "
						tmp=( $(echo $entry) )
						IFS=$'\n'
						deplib=${tmp[2]}
						if [[ ( $deplib != "" ) && ( -f $deplib ) ]]; then
							if [[ ( ${deplib:0:4} != "/usr" ) && ( ${deplib:0:4} != "/lib" ) && ( ${deplib:0:4} != "/opt" ) ]]; then
								if [ -L $deplib ]; then
									CheckArchitecture $(myreadlink $deplib)
								else
									CheckArchitecture $deplib
								fi
							fi
						fi
					fi
				done
				unset IFS
				;;

			Darwin | Mac)
				IFS=$'\n'
				for entry in ${ldd_out[@]}; do
					dep_file=$(echo $entry | awk -F " " '{print $1}')

					# check for existence of dependency file
					if [ ! -e $dep_file ]; then
						# if not found, search through DYLD_LIBRARY_PATH
						found=false
						dep_file=$(basename $dep_file)
						IFS=":"
						for dir in $DYLD_LIBRARY_PATH; do
							if [ -e $dir/$dep_file ]; then
								found=true
								dep_file=$dir/$dep_file
								break
							fi
						done
						unset IFS

						if $found; then
							if [[ ( ${dep_file:0:4} != "/usr" ) && ( ${dep_file:0:4} != "/lib" && ( ${dep_file:0:7} != "/System" ) ) ]]; then
								CheckArchitecture $dep_file
							fi
						else
							if [[ $critical == true ]]; then
								Failure "Unable to locate $dep_file using DYLD_LIBRARY_PATH"
								result=1
							else
								Warning "Unable to locate $dep_file using DYLD_LIBRARY_PATH"
							fi
						fi
						IFS=$'\n'
					fi
				done
				unset IFS
				;;

			*)
				echo "Unknown system $sys_type; unable to check for dependencies."
				result=1
		esac
	else
		if [[ $critical == true ]]; then
			Failure "Unable to locate $baselib"
			result=1
		else
			Warning "Unable to locate $baselib"
		fi
	fi

	if [ $result -eq 0 ]; then
		Success "All dependencies for $baselib have been met."
	fi
}

function CheckArchitecture
{
	sys_arch=$(uname -p 2> /dev/null)
	file_info=$(file $1)

	case "$sys_type" in
		Linux)
			if [ "$sys_arch" == "x86_64" ]; then
				pattern="64-bit"
			else
				pattern="32-bit"
			fi
			;;

		Darwin | Mac)
			ioreg -l -p IODeviceTree | grep firmware-abi | grep "EFI64" > /dev/null
			if [ $? -eq 0 ]; then
				sys_arch=x86_64
			else
				sys_arch=i386
			fi
			pattern=$sys_arch
			;;

		*)
			Warning "Unable to determine if $(basename $1) is compatible with a system with type of $sys_type"
			result=1
	esac

	echo $file_info | grep $pattern > /dev/null
	if [ $? -eq 0 ]; then
		Success "Library $(basename $1) is compatible with a $sys_type $sys_arch system"
	else
		Failure "Library $(basename $1) is not compatible with a $sys_type $sys_arch system"
		result=1
	fi
}

function CheckGMSEC
{
	echo
	echo "${txtcyn}Checking GMSEC API settings...${txtrst}"

	# Search for the GMSEC API bin directory using the appropriate Library Path
	# for this system.  We accomplish this by searching for GMSEC API library.
	#
	IFS=":"
	for dir in $ld_lib_path; do
		if [[ -e $dir/libGMSECAPI.${lib_ext} ]]; then
			gmsec_api_bin=$dir
			break
		fi
	done
	unset IFS

	# Core API library file not found using Library Path; assume that it
	# resides one level above the location where this script resides.
	if [ -z $gmsec_api_bin ]; then
		tmp="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
		gmsec_api_bin=$(dirname $tmp)
	fi

	if [[ -e $gmsec_api_bin/libGMSECAPI.${lib_ext} ]]; then
		Success "Found Core GMSEC library libGMSECAPI.${lib_ext} in $gmsec_api_bin"

		# Check to ensure we have the appropriate version of the GMSEC API for the
		# current architecture.
		#
		CheckArchitecture $gmsec_api_bin/libGMSECAPI.${lib_ext}

		if [ $result -eq 0 ]; then
			# Check PATH setting to see if GMSEC API bin is specified there (note, having
			# GMSEC API bin directory in PATH is not mandatory).
			#
			FindDirInPath $gmsec_api_bin

			# Check for existence of JAR files
			#
			if [ ! -z $gmsec_api_bin ]; then
				jars=( gmsecapi.jar )

				for jar in "${jars[@]}"; do
					if [[ ! -e $gmsec_api_bin/$jar ]]; then
						Failure "$jar not found within $gmsec_api_bin"
						result=1
					fi
				done

				# Check for JAR files in CLASSPATH
				#
				for jar in "${jars[@]}"; do
					echo $CLASSPATH | grep "$gmsec_api_bin/$jar" > /dev/null
					if [ $? -eq 0 ]; then
						Success "Reference to $jar JAR file found in CLASSPATH"
					else
						Warning "Reference to $jar JAR file not found in CLASSPATH"
					fi
				done
			fi

			# Check for the existence of the C2 and C2_NSS libraries
			#
			echo
			echo "${txtcyn}Checking GMSEC C2 and C2_NSS Extension settings...${txtrst}"
			if [ -e $gmsec_api_bin/libgmsec_c2.${lib_ext} ]; then
				Success "GMSEC C2 Extensions are available"
				CheckDependencies ${gmsec_api_bin}/libgmsec_c2.${lib_ext} false
			else
				Warning "GMSEC C2 library not found"
			fi
			if [ -e $gmsec_api_bin/libgmsec_c2_nss.${lib_ext} ]; then
				echo
				Success "GMSEC C2_NSS Extensions are available"
				CheckDependencies ${gmsec_api_bin}/libgmsec_c2_nss.${lib_ext} false
			else
				Warning "GMSEC C2_NSS library not found"
			fi
		fi
	else
		Failure "Core GMSEC library libGMSECAPI.${lib_ext} not found"
		result=1
	fi
}

function CheckJava
{
	echo
	echo "${txtcyn}Checking Java settings...${txtrst}"

    if [ -z $JAVA_HOME ]; then
        Warning "JAVA_HOME not defined"
        if [ -z $JRE_HOME ]; then
            Warning "JRE_HOME not defined"
            if [ -z $JDK_HOME ]; then
                Warning "JDK_HOME not defined"

				FindInPath java
				if [ "$path_found" != "" ]; then
					java_home=$(dirname $path_found)
				fi
            else
                Success "JDK_HOME is defined"
                java_home=$JDK_HOME
            fi
        else
            Success "JRE_HOME is defined"
            java_home=$JRE_HOME
        fi
    else
        Success "JAVA_HOME is defined"
		java_home=$JAVA_HOME
    fi

	if [ ! -z $java_home ]; then
		java_ver=$($java_home/bin/java -version 2>&1 | grep "version" | cut -d' ' -f3 | cut -d'"' -f2)

		if [ ! -z $java_ver ]; then
			Success "Detected Java $java_ver"

			# Attempt to execute validator application in GMSEC API to verify if the version of Java is compatible.
			#
			java=$java_home/bin/java
			cp=$gmsec_api_bin/gmsecapi.jar
			validator=gov.nasa.gsfc.gmsecapi.util.SystemValidator

			$java -cp $cp $validator 2>&1 | grep "java.lang.UnsupportedClassVersionError" > /dev/null

			if [ $? -eq 0 ]; then
				Failure "GMSEC is not compatible with Java $java_ver"
				result=1
			else
				Success "GMSEC is compatible with Java $java_ver"
			fi

			if [ $check_jms == true ]; then
				# Check if the GMSEC JMS wrapper is compatible with the Java version.
				#
		    	min_java_ver="1.6.0"

				echo $java_ver | grep "1.5" > /dev/null
				if [ $? -eq 0 ]; then
					Failure "GMSEC JMS wrapper requires Java $min_java_ver or later"
					result=1
				else
					Success "GMSEC JMS wrapper compatible with Java $java_ver"
				fi
			fi
		else
			Failure "Check installation of Java"
			result=1
		fi
	fi

	# Check if all dependencies are satisfied for GMSEC JNI library
	#
	echo
	echo "${txtcyn}Checking dependencies for GMSEC Java Native Interface (JNI) library...${txtrst}"
	CheckDependencies ${gmsec_api_bin}/libgmsec_jni.${jnilib_ext} false
}

function CheckPerl
{
	echo
	echo "${txtcyn}Checking Perl setup...${txtrst}"

	/usr/bin/which perl >& /dev/null

	if [ $? -eq 0 ]; then
		Success "Perl is installed"

		perl_ver="5.8.0"	# minimum version of perl supported by GMSEC

		$working_dir/perl_ver.pl >& /dev/null

		if [ $? -eq 0 ]; then
			Success "Perl with version $perl_ver (or later) found"
		else
			Failure "Perl with version $perl_ver (or later) not found"
			result=1
		fi
	else
		Warning "Perl was not found"
	fi
}

function CheckActiveMQ
{
	echo
	echo "${txtcyn}Checking middleware dependencies for ActiveMQ...${txtrst}"

	# Check if all dependencies are satisfied for mw_type
	#
	CheckDependencies ${gmsec_api_bin}/lib${mw}.${lib_ext} true

	# Check if using JMS wrappers
	#
	if $check_jms; then
		echo
		echo "${txtcyn}Checking JMS support for ActiveMQ...${txtrst}"

		CheckJMS

		# Check CLASSPATH settings
		#
		echo
		echo "${txtcyn}Checking for JMS ActiveMQ JAR file(s)...${txtrst}"

		jar_prefixes=( activemq-all camel-spring slf4j-simple )

		for prefix in "${jar_prefixes[@]}"; do
			IFS=":"
			tmp_result=1
			for dir in $CLASSPATH; do
				echo $dir | grep $prefix > /dev/null
				if [ $? -eq 0 ]; then
					tmp_result=0
					break
				fi
			done
			unset IFS

			if [ $tmp_result -eq 0 ]; then
				Success "Reference to $prefix JAR file found in CLASSPATH"
			else
				Failure "Reference to $prefix JAR file not found in CLASSPATH"
				result=1
			fi
		done
	fi

	# Check for environment variables
	#
	CheckEnvironmentVariables ActiveMQ
}

function CheckAMQP
{
	echo
	echo "${txtcyn}Checking middleware dependencies for AMQP...${txtrst}"

	# Check if all dependencies are satisfied for mw_type
	#
	CheckDependencies ${gmsec_api_bin}/lib${mw}.${lib_ext} true

	# Check if using JMS wrappers
	#
	if $check_jms; then
		echo
		echo "${txtcyn}JMS support is not offered for AMQP...${txtrst}"
	fi

	# Check for environment variables
	#
	CheckEnvironmentVariables AMQP

	# Check configuration file
	#
	echo
	echo "${txtcyn}Checking for existence of AMQP XML configuration file...${txtrst}"

	amqp_cfg_dir=${gmsec_api_bin}/amqp
	amqp_cfg_xml=${amqp_cfg_dir}/amqp.xml

	if [ -f ${amqp_cfg_xml} ]; then
		Success "Found amqp.xml configuration file in ${amqp_cfg_dir}"
	else
		Failure "Unable to find amqp.xml configuration within ${amqp_cfg_dir}"
	fi
}

function CheckArtemis
{
	echo
	echo "${txtcyn}Checking middleware dependencies for Artemis...${txtrst}"

	# Check if all dependencies are satisfied for mw_type
	#
	CheckDependencies ${gmsec_api_bin}/lib${mw}.${lib_ext} true

	# Check for environment variables
	#
	CheckEnvironmentVariables ActiveMQ
}

function CheckApollo
{
	echo
	echo "${txtcyn}Checking middleware dependencies for Apollo (which uses the ActiveMQ libraries)...${txtrst}"

	# Check if all dependencies are satisfied for mw_type
	#
	alt_mw=$( echo $mw | sed s/apollo/activemq/g )

	CheckDependencies ${gmsec_api_bin}/lib${alt_mw}.${lib_ext} true

	# Check if using JMS wrappers
	#
	if $check_jms; then
		echo
		echo "${txtcyn}JMS support is not offered for Apollo...${txtrst}"
	fi

	# Check for environment variables
	#
	CheckEnvironmentVariables Apollo
}

function CheckBolt
{
	echo
	echo "${txtcyn}Checking middleware dependencies for GMSEC Bolt...${txtrst}"

	# Check if all dependencies are satisfied for mw_type
	#
	CheckDependencies ${gmsec_api_bin}/lib${mw}.${lib_ext} true

	# Check if using JMS wrappers
	#
	if [ $check_jms == true ]; then
		echo
		echo "${txtcyn}Checking JMS support for GMSEC Bolt...${txtrst}"
		Warning "Not supported"
	fi

	# Check for environment variables
	#
	CheckEnvironmentVariables Bolt
}

function CheckMessageBus
{
	echo
	echo "${txtcyn}Checking middleware dependencies for GMSEC Message Bus (MB)...${txtrst}"

	# Check if all dependencies are satisfied for mw_type
	#
	CheckDependencies ${gmsec_api_bin}/lib${mw}.${lib_ext} true

	# Check if using JMS wrappers
	#
	if [ $check_jms == true ]; then
		echo
		echo "${txtcyn}Checking JMS support for GMSEC Message Bus...${txtrst}"
		Warning "Not supported"
	fi

	# Check for environment variables
	#
	CheckEnvironmentVariables MB
}

function CheckOpenDDS
{
	echo
	echo "${txtcyn}Checking middleware dependencies for GMSEC OpenDDS...${txtrst}"

	# Check if all dependencies are satisfied for mw_type
	#
	CheckDependencies ${gmsec_api_bin}/lib${mw}.${lib_ext} true

	# Check if using JMS wrappers
	#
	if [ $check_jms == true ]; then
		echo
		echo "${txtcyn}Checking JMS support for GMSEC OpenDDS...${txtrst}"
		Warning "Not supported"
	fi

	# Check for environment variables
	#
	CheckEnvironmentVariables OpenDDS
}

function CheckWebSphere
{
	echo
	echo "${txtcyn}Checking middleware dependencies for WebSphere...${txtrst}"

	# Check if all dependencies are satisfied for mw_type
	#
	CheckDependencies ${gmsec_api_bin}/lib${mw}.${lib_ext} true

	# Check if using JMS wrappers
	#
	if [ $check_jms == true ]; then
		echo
		echo "${txtcyn}Checking JMS support for WebSphere...${txtrst}"

		CheckJMS

		# Check CLASSPATH settings
		#
		echo
		echo "${txtcyn}Checking for JMS WebSphere JAR file(s)...${txtrst}"

		jars=( com.ibm.mqjms.jar )

		for jar in "${jars[@]}"; do
			IFS=":"
			tmp_result=1
			for dir in $CLASSPATH; do
				echo $dir | grep $jar > /dev/null
				if [ $? -eq 0 ]; then
					tmp_result=0
					break
				fi
			done
			unset IFS

			if [ $tmp_result -eq 0 ]; then
				Success "Reference to $jar JAR file found in CLASSPATH"
			else
				Failure "Reference to $jar JAR file not found in CLASSPATH"
				result=1
			fi
		done
	fi

	# Check for environment variables
	#
	CheckEnvironmentVariables WebSphere
}

function CheckZeroMQ
{
	echo
	echo "${txtcyn}Checking middleware dependencies for ZeroMQ...${txtrst}"

	# Check if all dependencies are satisfied for mw_type
	#
	CheckDependencies ${gmsec_api_bin}/lib${mw}.${lib_ext} true

	# Check if using JMS wrappers
	#
	if $check_jms; then
		echo
		echo "${txtcyn}JMS support is not offered for ZeroMQ ${txtrst}"
	fi

	# Check for environment variables
	#
	CheckEnvironmentVariables ZeroMQ
}

function CheckJMS
{
	# Check if all dependencies are satisfied for the Generic JMS library.
	#
	CheckDependencies ${gmsec_api_bin}/libgmsec_generic_jms.${lib_ext} true
}

function CheckEnvironmentVariables
{
	mw=$1
	lower_mw="$(echo $mw | tr '[A-Z]' '[a-z]')"
	env_file=$working_dir/$lower_mw.env

	if [ -f $env_file ]; then
		echo
		echo "${txtcyn}Checking for optional $mw environment variables...${txtrst}"

		while read line; do
			if [ "${line:0:1}" == "#" ]; then
				continue
			fi
			if [ -z "${!line}" ]; then
				Warning "Environment variable $line is undefined"
			else
				Success "Environment variable $line is set to '${!line}'"
			fi
		done < $env_file
	fi
}


###
###  Entry to script
###
if [ $# -lt 1 ]; then
	echo "Error: Middleware type missing."
	echo
	ShowUsage
	exit 1
fi


working_dir=$(dirname $0)
mw_type=$1
gmsec_api_bin=
path_found=
java_home=
java_ver=
result=0


if [[ -f $working_dir/../../include/gmsec_version.h ]]; then
	version=$(cat $working_dir/../../include/gmsec_version.h | grep GMSEC_VERSION_NUMBER | awk -F\" '{print $2}')
elif [[ -f ../version.inf ]]; then
	version=$(cat ../version.inf)
else
	version=__API_VERSION__
fi

# Only define colors (for text) when stdout is associated with a terminal.
if [ -t 1 ]; then
	txtrst=$(tput sgr0)    # Text reset
	txtred=$(tput setaf 1) # Red
	txtgrn=$(tput setaf 2) # Green
	txtyel=$(tput setaf 3) # Yellow
	txtblu=$(tput setaf 4) # Blue
	txtpur=$(tput setaf 5) # Purple
	txtcyn=$(tput setaf 6) # Cyan
	txtwht=$(tput setaf 7) # White
fi


# Check validity of input for middleware type
#
case $mw_type in
	*"activemq"*)   check_mwtype=amq     ;;
	*"amqp")        check_mwtype=amqp    ;;
	*"artemis")     check_mwtype=art     ;;
	*"bolt")        check_mwtype=bolt    ;;
	*"ibmmq"*)      check_mwtype=ibmmq   ;;
	*"mb")          check_mwtype=mb      ;;
	*"opendds"*)    check_mwtype=opendds ;;
	*"zeromq"*)     check_mwtype=zeromq  ;;
	*)
		echo
		echo "ERROR: Unrecognized middleware -- $mw_type"
		echo
		ShowUsage
		exit 1;;
esac

# If necessary, prepend 'gmsec_' to middleware type.
#
echo $mw_type | grep "gmsec_" > /dev/null
if [ $? -eq 0 ]; then
	mw=$mw_type
else
	mw=gmsec_$mw_type
fi

# Check if we have a second argument, and whether it is valid for requesting
# a JMS compatibility check.
#
check_jms=false
if [ $# -eq 2 ]; then
	if [ "$2" == "check_jms" -o "$2" == "check_JMS" -o "$2" == "CHECK_JMS" ]; then
		check_jms=true
	else
		echo "Error: Invalid string for check_JMS parameter; found '$2'"
		ShowUsage
		exit 1
	fi
fi

# Perform System Analysis (note the order is important)
#
# Determine our system type
sys_type=$(uname -s)
case "$sys_type" in
	Linux)
		ld_lib_path=$LD_LIBRARY_PATH
		ld_lib_path_type="LD_LIBRARY_PATH"
		ldd_cmd=/usr/bin/ldd
		ldd_opt=
		lib_ext="so"
		jnilib_ext=$lib_ext
		;;

	Darwin | Mac)
		sys_type=Mac	# Use Mac; the name Darwin is not as recognizable.
		ld_lib_path=$DYLD_LIBRARY_PATH
		ld_lib_path_type="DYLD_LIBRARY_PATH"
		ldd_cmd=/usr/bin/otool
		ldd_opt="-L -X"
		lib_ext="dylib"
		jnilib_ext="jnilib"
		;;

	*)
		echo "${txtred}ERROR: Unable to determine system type; found '$sys_type'.  Supported systems are:"
		echo "       Linux and MacOS${txtrst}"
		exit 1
esac


echo
echo "${txtcyn}Starting GMSEC System Environment Validator Tool (version $version)...${txtrst}"

CheckGMSEC
CheckJava
CheckPerl

case $check_mwtype in
	"amq")		CheckActiveMQ;;
	"amqp")		CheckAMQP;;
	"art")		CheckArtemis;;
	"bolt")		CheckBolt;;
	"ibmmq")	CheckWebSphere;;
	"mb")		CheckMessageBus;;
	"opendds")	CheckOpenDDS;;
	"ws")		CheckWebSphere;;
	"zeromq")	CheckZeroMQ;;
esac


# Report test summary
#
echo
echo "${txtcyn}         -----------------------------------------------------${txtrst}"
echo
if [ $result -eq 0 ]; then
	echo "${txtgrn}Your system is properly configured!${txtrst}"
	echo
	echo "${txtgrn}If there are any WARNING(s) listed above, carefully consider${txtrst}"
	echo "${txtgrn}whether they need to be corrected for your system needs.${txtrst}"
else
	echo "${txtred}Please correct FAILURE(s) and optionally any WARNING(s) shown above${txtrst}"
	echo "${txtred}before proceeding to use the GMSEC API.${txtrst}"
	echo
	echo "${txtred}Refer to the GMSEC API Installation Guide and/or the Installation and${txtrst}"
	echo "${txtred}Configuration Guide (for GMSEC Middleware Installation) for assistance${txtrst}"
	echo "${txtred}in resolving the issues(s).${txtrst}"
fi
echo

exit $result
