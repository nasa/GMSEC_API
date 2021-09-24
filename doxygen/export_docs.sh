#!/usr/bin/env bash

# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

# This script is used to export generated Doxygen files to the
# GMSEC_API_DOCS/docs/manual folder.
#
# It is assumed that the Doxygen documentation for the current
# API release has already been generated.
#

usage()
{
	echo "Usage: $0 <branch>"
	echo
	echo "Example: $0 API-1234"
	echo
}


if [ $# -ne 1 ]; then
	echo
	echo "Error: missing task/branch"
	echo
	usage
	exit 1
fi

git_branch=$1

if [[ "$git_branch" == "master" ]]; then
	echo
	echo "Cannot use 'master' for the branch!"
	echo
	usage
	exit 1
fi


src_dir=./manual
dest_dir=../../GMSEC_API_DOCS
man_dir=$dest_dir/docs/manual

doc_dirs=( c_3x c_4x cpp_3x cpp_4x csharp_4x java_3x java_4x perl_3x perl_4x python3_4x ruby_4x )


if [ ! -d $src_dir ]; then
	echo "Error: Folder $src_dir could not be found."
	echo
	echo "Did you forget to generate the Doxygen documentation?"
	echo
	exit 1
fi

if [ ! -d $man_dir ]; then
	echo "Error: Folder $man_dir could not be found."
	echo
	echo "Did you forget to checkout GMSEC_API_DOCS?"
	echo
	exit 1
fi


error=0

pushd $dest_dir > /dev/null
	git checkout master
	git fetch -p origin
	git merge origin/master

	git checkout -b $git_branch

	error=$?
popd > /dev/null


if [ $error -ne 0 ]; then
	echo
	echo "Exiting after fatal error."
	echo
	exit $error
fi


for doc_dir in ${doc_dirs[@]}; do

	if [ -d $man_dir/$doc_dir ]; then
		rm -rf $man_dir/$doc_dir
	fi

	cp -rp $src_dir/$doc_dir $man_dir

	pushd $man_dir/$doc_dir > /dev/null
		git add *
		git commit -am "Updating Doxygen documentation directory $doc_dir for API release."
	popd > /dev/null

done

pushd $dest_dir > /dev/null
	git push origin $git_branch
popd > /dev/null

exit 0
