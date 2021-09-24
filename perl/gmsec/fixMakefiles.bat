@ECHO OFF

rem Copyright 2007-2021 United States Government as represented by the
rem Administrator of The National Aeronautics and Space Administration.
rem No copyright is claimed in the United States under Title 17, U.S. Code.
rem All Rights Reserved.


setlocal EnableDelayedExpansion

for %%d in (GMSECAPI Config ConfigFile Connection ConnectionFactory Field Message Status) do (

	perl -pi.old -e "s/DLEXT = xs.dll/DLEXT = dll/g" %%d\Makefile

	perl -pi.old -e "s/OBJ_EXT = .o/OBJ_EXT = .obj/g" %%d\Makefile
	
	perl -pi.old -e "s/-s -O2/-O2/g" %%d\Makefile
	
	perl -pi.old -e "s/DUSE_PERLIO/DUSE_PERLIO \/EHsc/g" %%d\Makefile

	perl -pi.old -e "s/-O2/\/O2/g" %%d\Makefile
	
	perl -pi.old -e "s/\$\(CC\) -c/\$\(CC\) \/c/g" %%d\Makefile
	
	perl -pi.old -e "s/-I/\/I/g" %%d\Makefile
	
	perl -pi.old -e "s/-D/\/D/g" %%d\Makefile
	
	perl -pi.old -e "s/-w -P //g" %%d\Makefile
	
	perl -pi.old -e "s/-mdll -s//g" %%d\Makefile
	
	perl -pi.old -e "s/-s -L/-L/g" %%d\Makefile
	
	perl -pi.old -e "s/-L/\/libpath:/g" %%d\Makefile

	perl -pi.old -e "s/\$\(LD\) \$\(EXPORT_LIST\) -o /\$\(LD\) \/DEF:\$\(EXPORT_LIST\) \/OUT:/g" %%d\Makefile

	perl -pi.old -e "s/-Wl,--enable-auto-image-base/\/DLL/g" %%d\Makefile
	
	perl -pi.old -e "s/-fwrapv//g" %%d\Makefile
	
	perl -pi.old -e "s/-fno-strict-aliasing//g" %%d\Makefile
	
	perl -pi.old -e "s/-mms-bitfields//g" %%d\Makefile
	
	perl -pi.old -e "s/LDLOADLIBS =  -lmoldname -lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -lnetapi32 -luuid -lws2_32 -lmpr -lwinmm -lversion -lodbc32 -lodbccp32 -lcomctl32/LDLOADLIBS =/g" %%d\Makefile

)
