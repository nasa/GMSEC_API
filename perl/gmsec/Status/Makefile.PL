# Copyright 2007-2016 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.



use ExtUtils::MakeMaker;

use Util;


$opts{CCFLAGS} = "-P $Config{ccflags}" if $Config{cc} eq 'gcc';  
$opts{CCFLAGS} =~ s/-Wdeclaration-after-statement//ig;


WriteMakefile(
	Util::args('GMSECAPI::Status'),
	CCFLAGS => $opts{CCFLAGS},
	OBJECT => 'Status$(OBJ_EXT)',
);
