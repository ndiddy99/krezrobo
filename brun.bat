@set NGPC_DEV=c:\ngpc

path=%path%;%NGPC_DEV%\T900\bin
SET THOME=%NGPC_DEV%\T900
cd C:\ngpc\nathanstuff

@REM Clean
del *.rel
del *.abs
del *.map 
del *.s24
del *.ngp

cc900 -c -O3 main.c -o main.rel
cc900 -c -O3 library.c -o library.rel
tulink -la -o out.abs ngpc.lcf system.lib main.rel library.rel 
tuconv -Fs24 out.abs
s242ngp out.s24

@REM Clean
del *.rel
del *.abs
del *.map 
del *.s24

%NGPC_DEV%\NeoPop\NeoPop-Win32.exe out.ngp