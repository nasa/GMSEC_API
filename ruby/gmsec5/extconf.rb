# Copyright 2007-2025 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

require 'mkmf'

LIBDIR     = RbConfig::CONFIG['libdir']
INCLUDEDIR = RbConfig::CONFIG['includedir']

HEADER_DIRS = [
  '../../framework/include',
  INCLUDEDIR
]

LIBDIRS = []

if RUBY_PLATFORM.downcase.include?("mswin")
	# Windows

	LIB_DIRS = [
		'../../objects/Release',
		LIBDIR
	]

	dir_config('', HEADER_DIRS, LIB_DIRS)

	unless have_library('gmsec_api') && append_library($libs, 'gmsec_api')
		abort "Unable to locate gmsec_api library"
	end

	$CXXFLAGS << " /GS /analyze- /Gy /Gm- /O2 /Ob1 /GF /WX- /Zc:forScope /Gd /Oy- /MD /EHsc"
	$LDFLAGS  << " /DYNAMICBASE:NO /SAFESEH:NO"
else
	# Linux/macOS

	LIB_DIRS = [
		'../../bin',
		LIBDIR
	]

	dir_config('', HEADER_DIRS, LIB_DIRS)

	unless have_library('gmsec_api') && append_library($libs, 'gmsec_api')
		abort "Unable to locate gmsec_api library"
	end

    if RUBY_PLATFORM.downcase.include?("darwin")
	    $CXXFLAGS << " -w"    # TODO: Not a good idea
    else
	    $CXXFLAGS << " -std=c++11 -Wno-unused-but-set-variable"
    end
end

create_makefile('interfaces/libgmsec_ruby', 'interfaces')
