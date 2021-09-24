#!/usr/bin/perl

# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.



# This perl script is intended for use on Window OS to determine the 
# architecture that was used to build the executable or DLL file that
# is given.
#
# usage: get_arch <exefile>
#
$exe = $ARGV[0];

open(EXE, $exe) or die "can't open $exe: $!";

binmode(EXE);

if (read(EXE, $doshdr, 68)) {

   ($magic,$skip,$offset)=unpack('a2a58l', $doshdr);

   die("Not an executable") if ($magic ne 'MZ');

   seek(EXE,$offset,SEEK_SET);

   if (read(EXE, $pehdr, 6)) {
       ($sig,$skip,$machine)=unpack('a2a2v', $pehdr);

       die("No a PE Executable") if ($sig ne 'PE');

       if ($machine == 0x014c) {
            print "x86";
       }
       elsif ($machine == 0x0200) {
            print "IA64";
       }
       elsif ($machine == 0x8664) {
            print "AMD64";
       }
       else {
            printf("Unknown machine type 0x%lx\n", $machine);
       }
   }
}

close(EXE);
