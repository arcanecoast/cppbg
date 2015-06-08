@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x86

cd ..
cmake -H. -Bbuild -G "Visual Studio 12" -DCMAKE_INSTALL_PREFIX:PATH=../bin

cd build
msbuild cppbg.sln /p:Configuration=Debug
msbuild INSTALL.vcxproj /p:Configuration=Debug

msbuild cppbg.sln /p:Configuration=Release
msbuild INSTALL.vcxproj /p:Configuration=Release
cd ..

cd scripts

