# mkrandom
Fast random binary file generator 

Files are incompressible and have good statistical properties:
http://www.pcg-random.org/

# Usage 
`mkrandom size[k|m|g|t] filename`

## Download mkrandom binary via curl
```
# OS X
curl -o mkrandom -L https://github.com/cohadar/mkrandom/blob/master/bin/darwin_amd64/mkrandom?raw=true

# Linux
curl -o mkrandom -L https://github.com/cohadar/mkrandom/blob/master/bin/linux_x86_64/mkrandom?raw=true

# Windows
curl -o mkrandom.exe -UseBasicParsing https://github.com/cohadar/mkrandom/blob/master/bin/windows7_x86_64/mkrandom.exe?raw=true
```

## Make the mkrandom binary executable and move it to your PATH (e.g. /usr/local/bin):
```
chmod +x ./mkrandom
sudo mv ./mkrandom /usr/local/bin/mkrandom
```

## Build from source on Linux and MacOSX
simple ```make``` will do the trick.

## Build from source on Windows
* If you have Visual Studio run `nmake -f Makefile.MSVC` from Developer Command Prompt.
* If you have MinGW run `make -f Makefile.MingGW` from msys console (C:\MinGW\msys\1.0\msys.bat).
