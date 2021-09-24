@ECHO OFF


rem Copyright 2007-2021 United States Government as represented by the
rem Administrator of The National Aeronautics and Space Administration.
rem No copyright is claimed in the United States under Title 17, U.S. Code.
rem All Rights Reserved.


REM Replace instances where a new Field object is created with a
REM call to our factory method that constructs the desired Field
REM subsclass.
REM 
set OLD_TEXT="new Field("
set NEW_TEXT="FieldFactory.BuildField("

for %%a in (interfaces\*.cs) do (
	perl -pi.bak -e "s/new Field\(/FieldFactory.BuildField\(/g" %%a
)


REM Finally, copy custom Field Factory to interfaces directory
REM
copy FieldFactory.cs interfaces > NUL


@ECHO ON
