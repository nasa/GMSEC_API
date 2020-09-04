#!/usr/bin/env bash

# This script is used to export generated Doxygen files to the GMSEC_API_DOCS/docs/manual folder
#
# It is assumed that the Doxygen documentation for the current API release has already been generated.
#

src_dir=./manual
dest_dir=../../GMSEC_API_DOCS/docs/manual


if [ ! -d $src_dir ]; then
	echo "Error: Folder $src_dir could not be found."
	echo
	echo "Did you forget to generate the Doxygen documentation?"
	echo
	exit 1
fi

if [ ! -d $dest_dir ]; then
	echo "Error: Folder $dest_dir could not be found."
	echo
	echo "Did you forget to checkout GMSEC_API_DOCS?"
	echo
	exit 1
fi


doc_dirs=( c_3x c_4x cpp_3x cpp_4x java_3x java_4x perl_3x perl_4x python_4x )

for doc_dir in ${doc_dirs[@]}; do

	svn rm $dest_dir/$doc_dir

	cp -rp $src_dir/$doc_dir $dest_dir

	svn add $dest_dir/$doc_dir

	pushd $dest_dir > /dev/null

		svn commit -m "Updating Doxygen documentation directory $doc_dir for API release."

	popd > /dev/null

done
