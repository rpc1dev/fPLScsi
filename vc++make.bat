@echo off
Rem *** This batch file can be used to compile with the **free** version of VC++ toolkit
Rem *** See: http://msdn.microsoft.com/visualc/vctoolkit2003/

if "%1"=="" goto :default
goto :%1

:default
cl /o plscsi.exe /nologo *.c*
goto :exit


:clean

del plscsi.exe

del cl.obj
del dosaspi.obj
del dos32aspi.obj
del plscsi.obj
del scl.obj
del scsi.obj
del scsistub.obj
del sgio.obj
del sptx.obj
del stuc.obj
del wideint.obj
del winaspi.obj


:exit
