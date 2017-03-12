# mkrandom
Fast random binary file generator 

Files are incompressible and have good statistical properties:
http://www.pcg-random.org/

# Usage 
`mkrandom size[k|m|g|t] filename`

## Download for OS X 
```
curl -o mkrandom -L https://github.com/cohadar/mkrandom/blob/master/bin/darwin/amd64/mkrandom?raw=true
chmod +x ./mkrandom && sudo mv ./mkrandom /usr/local/bin/mkrandom
```

## Download for Linux
```
curl -o mkrandom -L https://github.com/cohadar/mkrandom/blob/master/bin/linux/amd64/mkrandom?raw=true
chmod +x ./mkrandom && sudo mv ./mkrandom /usr/local/bin/mkrandom
```

## Download for Windows
* [mkrandom.exe (64-bit)](https://github.com/cohadar/mkrandom/blob/master/bin/windows/386/mkrandom.exe?raw=true)
* [mkrandom.exe (32-bit)](https://github.com/cohadar/mkrandom/blob/master/bin/windows/amd64/mkrandom.exe?raw=true)
