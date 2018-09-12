@echo off

rem call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"
call D:\VisualStudio\2017\Community\VC\Auxiliary\Build\vcvars64.bat

mkdir .build_msc_x64
cd .build_msc_x64
rem cmake .. -DCMAKE_BUILD_TYPE=Release -DARCH=x64 -G "NMake Makefiles"
cmake .. -DCMAKE_BUILD_TYPE=Debug -G "NMake Makefiles"
nmake
pause
