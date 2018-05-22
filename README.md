
# mkrandom

#### Fast random data stream and file generator.


## Background

Damir Cohadarevic ([Codahar](https://github.com/cohadar)) has created a deceptively simple program in *mkrandom*.

It's an example of something greater than its separate parts.

*mkrandom* combines the highly promising [PCG](http://www.pcg-random.org/) random number generator with chunked output to create a blazingly fast junk data generator.

[PCG](http://www.pcg-random.org/), created by Melissa E. O'Neill (Professor of Computer Science at a US College), is a fast random number generator with statistically good output.


## Purpose

Quickly generate large files and large streams of random data (GB, to perhaps TB in size).


## Uses

File hashing, integrity tests, benchmarking, network speed tests ... and probably, if the intent is abuse, HDD and SSD destruction.


## OS Support

+ Linux x64
+ Windows x64


## [C Version](https://github.com/Tinram/mkrandom/tree/c_legacy)

I've added a slightly revised version of Codahar's original *mkrandom* C program into a new [branch](https://github.com/Tinram/mkrandom/tree/c_legacy) of this forked repo.

The main revision is stream output in addition to file output.

### Why C?

#### Size

An 11kB C ELF executable is great; I'm not so keen on ferrying around the 1.9MB Go executable equivalent.

#### Language

I'm more familiar with C than Go.  My Go education is on hold.


## Usage

        ./mkrandom <size><k|m|g|t> [filename]

        mkrandom.exe <size><k|m|g|t> [filename]

Output data will be to file if the optional filename is specified, else to *stdout*, meaning a stream which can be piped to another program (or without a pipe specified, to the terminal ...).


## Executables

### Linux

+ [mkrandom](https://github.com/Tinram/mkrandom/raw/c_legacy/bin/linux/amd64/c/mkrandom)

### Windows

+ [mkrandom.exe](https://github.com/Tinram/mkrandom/raw/c_legacy/bin/windows/amd64/c/mkrandom.exe)


## References

+ [PCG](http://www.pcg-random.org/)
+ [mkrandom](https://github.com/cohadar/mkrandom)


## License

*mkrandom* is released under [Codahar's MIT License](https://github.com/cohadar/mkrandom/blob/master/LICENSE).
