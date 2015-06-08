cppbg
=====

*Author: metamaker*

This library enables parsing of WeiDU .tra files and manipulation of .TLK v1 format. It is useful for modding of Baldur's Gate, Icewind Dale, etc.

How to build
------------

You can build this library with `CMake` (<http://www.cmake.org/>). The steps are next:

1. Open command line and change current directory with `$ cd` command to the directory where you unpacked the project's source code.
2. Run `$ cmake -H. -Bbuild -G "Visual Studio 12"` to create Makefile for Visual Studio 2013 (or use other generator if you use other IDE). Don't forget to specify path to dependency libraries and headers with -D parameter.
3. Open `build/cppbg.sln` and build solution in Visual Studio.
4. Optionally, you can install library by building project which name is `INSTALL` in Visual Studio. Remember that you must have enough rights on your system to let VS copy files to installation folder.

If you are using Visual Studio 12 you can build and install library by just running `scripts/build-vs2013.bat`.

How to use
----------

To work with WeiDU .tra files please check `cppbg/tra/WeiDUModTranslation.h`. To work with .TLK v1 format check `cppbg/tlk_v1/TalkTableFile.h`.
