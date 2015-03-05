@echo off
set mydate=%date:~6,4%%date:~3,2%%date:~0,2%
echo %mydate%
echo Copying ImagePlay Release %mydate%

set destfolder=..\Releases\%mydate%\
set sourcefolder=.\ImagePlay\release\

echo %sourcefolder%
echo %destfolder%

mkdir %destfolder%

REM copy contents
xcopy /Y %sourcefolder%ImagePlay.exe 	%destfolder%

xcopy /Y %sourcefolder%IPL.dll 		%destfolder%
xcopy /Y %sourcefolder%FreeImage.dll 	%destfolder%
xcopy /Y %sourcefolder%msvcr120.dll 	%destfolder%
xcopy /Y %sourcefolder%msvcp120.dll 	%destfolder%
xcopy /Y %sourcefolder%vcomp120.dll 	%destfolder%
xcopy /Y %sourcefolder%icudt52.dll 	%destfolder%
xcopy /Y %sourcefolder%icuin52.dll 	%destfolder%
xcopy /Y %sourcefolder%icuuc52.dll 	%destfolder%
xcopy /Y %sourcefolder%libEGL.dll 		%destfolder%
xcopy /Y %sourcefolder%libGLESv2.dll 	%destfolder%
xcopy /Y %sourcefolder%Qt5Core.dll 	%destfolder%
xcopy /Y %sourcefolder%Qt5Gui.dll 		%destfolder%
xcopy /Y %sourcefolder%Qt5Help.dll 	%destfolder%
xcopy /Y %sourcefolder%Qt5Widgets.dll 	%destfolder%

xcopy /Y /S %sourcefolder%platforms\* 		%destfolder%platforms\*
xcopy /Y /S %sourcefolder%images\* 		%destfolder%images\*
xcopy /Y /S %sourcefolder%plugins\* 		%destfolder%plugins\*
xcopy /Y /S %sourcefolder%plugin_development\* 		%destfolder%plugin_development\*
xcopy /Y /S %sourcefolder%process_icons\* 		%destfolder%process_icons\*

REM pause
%SystemRoot%\explorer.exe %destfolder%
