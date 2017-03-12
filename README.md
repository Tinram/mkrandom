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
chmod +x ./mkrandom && mv ./mkrandom /usr/local/bin/mkrandom

# Linux
curl -o mkrandom -L https://github.com/cohadar/mkrandom/blob/master/bin/linux_x86_64/mkrandom?raw=true
chmod +x ./mkrandom && mv ./mkrandom /usr/local/bin/mkrandom
```

## Download windows executable
[click here](https://github.com/cohadar/mkrandom/blob/master/bin/windows7_x86_64/mkrandom.exe?raw=true)
