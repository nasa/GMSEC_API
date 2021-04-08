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

	unless have_library('gmsecapi') && append_library($libs, 'gmsecapi')
		abort "Unable to locate gmsecapi library"
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

	unless have_library('GMSECAPI') && append_library($libs, 'GMSECAPI')
		abort "Unable to locate GMSECAPI library"
	end

	$CXXFLAGS << " -Wno-unused-but-set-variable"
end

create_makefile('interfaces/libgmsec_ruby', 'interfaces')
