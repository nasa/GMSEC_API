# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.








# Check to see if Perl supports threading
use Config;
$Config{useithreads} or die("#----------------------------------------------------------------------------------#\n#----- Recompile Perl with threads to run this program. (Version 5.8 or later)-----#\n#----------------------------------------------------------------------------------#");

use ExtUtils::MakeMaker;

use Util;

WriteMakefile(
	Util::args('GMSECAPI'),
	DIR => [ qw(GMSECAPI Status Config ConfigFile
		Connection ConnectionFactory Field Message) ],
);

