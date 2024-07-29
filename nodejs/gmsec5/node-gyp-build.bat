rem Copyright 2007-2024 United States Government as represented by the
rem Administrator of The National Aeronautics and Space Administration.
rem No copyright is claimed in the United States under Title 17, U.S. Code.
rem All Rights Reserved.


if "%NODE_HOME%" == "" (
	echo Error: NODE_HOME has not been defined
	goto:eof
)

set GMSEC_SRC=..\..

echo NODE_HOME: %NODE_HOME%

REM Clean up
if exist .\build (
	rmdir .\build /s /q
)

echo %NODE_HOME%\node_modules\npm\bin\node-gyp-bin\node-gyp configure --python=%NODE_HOME%\..\python-3.10.7\python.exe
call %NODE_HOME%\node_modules\npm\bin\node-gyp-bin\node-gyp configure --python=%NODE_HOME%\..\python-3.10.7\python.exe

echo Creating build directories for header and source files within %CD%
mkdir .\build\Release\obj.target\include\gmsec5
mkdir .\build\Release\obj.target\include\gmsec5\c\field
mkdir .\build\Release\obj.target\include\gmsec5\field
mkdir .\build\Release\obj.target\include\gmsec5\internal\field
mkdir .\build\Release\obj.target\include\gmsec5\internal\util
mkdir .\build\Release\obj.target\include\gmsec5\secure
mkdir .\build\Release\obj.target\include\gmsec5\util
mkdir .\build\Release\obj.target\src\gmsec5
mkdir .\build\Release\obj.target\src\gmsec5\field
mkdir .\build\Release\obj.target\src\gmsec5\internal\field
mkdir .\build\Release\obj.target\src\gmsec5\internal\util
mkdir .\build\Release\obj.target\src\gmsec5\secure
mkdir .\build\Release\obj.target\src\gmsec5\util

echo Copying header and source files
copy %GMSEC_SRC%\framework\include\*.h .\build\Release\obj.target\include\
copy %GMSEC_SRC%\framework\include\gmsec5\*.h .\build\Release\obj.target\include\gmsec5\
copy %GMSEC_SRC%\framework\include\gmsec5\c\*.h .\build\Release\obj.target\include\gmsec5\c
copy %GMSEC_SRC%\framework\include\gmsec5\c\field\*.h .\build\Release\obj.target\include\gmsec5\c\field
copy %GMSEC_SRC%\framework\include\gmsec5\field\*.h .\build\Release\obj.target\include\gmsec5\field
copy %GMSEC_SRC%\framework\include\gmsec5\secure\*.h .\build\Release\obj.target\include\gmsec5\secure
copy %GMSEC_SRC%\framework\include\gmsec5\util\*.h .\build\Release\obj.target\include\gmsec5\util
copy %GMSEC_SRC%\framework\include\gmsec5\internal\*.h .\build\Release\obj.target\include\gmsec5\internal
copy %GMSEC_SRC%\framework\include\gmsec5\internal\field\*.h .\build\Release\obj.target\include\gmsec5\internal\field
copy %GMSEC_SRC%\framework\include\gmsec5\internal\util\*.h .\build\Release\obj.target\include\gmsec5\internal\util
copy %GMSEC_SRC%\framework\src\gmsec5\*.cpp .\build\Release\obj.target\src\gmsec5
copy %GMSEC_SRC%\framework\src\gmsec5\field\*.cpp .\build\Release\obj.target\src\gmsec5\field
copy %GMSEC_SRC%\framework\src\gmsec5\util\*.cpp .\build\Release\obj.target\src\gmsec5\util

echo %NODE_HOME%\node_modules\npm\bin\node-gyp-bin\node-gyp build
call %NODE_HOME%\node_modules\npm\bin\node-gyp-bin\node-gyp build

echo Copying gmsec_nodejs.node to bin dir
copy .\build\Release\gmsec_nodejs.node ..\..\bin

REM Clean up
REM if exist .\build (
REM	rmdir .\build /s /q
REM )

:eof
