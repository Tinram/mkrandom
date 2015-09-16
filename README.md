# mkrandom
Fast random binary file generator 

Files are incompressible and have good statistical properties:
http://www.pcg-random.org/

Usage: ```mkrandom size[k|m|g|t] filename```

## Linux and MacOSX
simple ```make``` will do the trick.

## Windows
If you have Visual Studio run ```nmake -f Makefile.MSVC``` from Developer Command Prompt.

If you have MinGW run ```make -f Makefile.MingGW``` from msys console (C:\MinGW\msys\1.0\msys.bat).
