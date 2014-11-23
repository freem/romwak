ROMWak
======
Original program by Jeff Kurtz, ported to ANSI C by freem

Tested and confirmed working on these platforms and compilers:
* Windows x32 (Microsoft 32-bit C/C++ Optimizing Compiler Version 15.00.30729.01 for 80x86)
* Windows x64 (tcc 0.9.26)
* Windows x64 (gcc (rev5, Built by MinGW-W64 project) 4.8.1 on Windows 7 x64)
* Linux x64 (gcc (Ubuntu 4.8.2-19ubuntu1) 4.8.2 on Linux 3.13.0-24 x86_64)
* Linux PowerPC (gcc (Debian 4.6.3-14) 4.6.3 on Linux 3.2.0-4 powerpc)
* OpenPandora/armv7l (arm-angstrom-linux-gnueabi-gcc (GCC) 4.3.3 on openpandora 3.2.53)

Usage
-----
ROMWak is a program whose parameters change depending on what option you pass in.

The base command is `romwak <option>`, where the options are:
* /b - Split file into two files, alternating bytes into separate files.
* /f - Flip low/high bytes of a file. (`<outfile1>` optional.)
* /h - Split file in half (two files).
* /m - Byte merge two files. (stores results in `<outfile2>`).
* /s - Swap top and bottom halves of a file. (`<outfile1>` optional.)
* /w - Split file into two files, alternating words into output files.
* /p - Pad file to [psize] in K with [pbyte] value (0-255).

The program also supports shorthand -params (e.g. '-b', '-p', and so on).

### Split File in Two, Alternating Bytes (/b) ###
`romwak /b <infile> <outfile1> <outfile2>`  
Splits the specified input file into two files by words (two bytes).

### Flip High/Low Bytes (/f) ###
`romwak /f <infile> [<outfile>]`  
Flips the high and low bytes of the specified file.

`<outfile>` is optional; if omitted, the file will be swapped in place.

### Split File in Half (/h) ###
`romwak /h <infile> <outfile1> <outfile2>`  
Splits the input file in half into two files (outfile1 and outfile2).

### Byte Merge Two Files (/m) ###
`romwak /m <infile1> <infile2> <outfile>`  
Merges the bytes of infile1 and infile2 to create outfile.

The byte of infile1 is written, then the byte of infile2 is written;
repeat for the entire length of the file.

### Swap Top and Bottom Halves of File (/s) ###
`romwak /s <infile> [<outfile>]`  
Swaps the top and bottom halves of the file.

This works pretty awkwardly with odd length files (one byte will be missing).

`<outfile>` is optional; if omitted, the file will be swapped in place.

### Split File Two, Alternating Words (/w) ###
`romwak /w <infile> <outfile1> <outfile2>`  
Splits the input file into two files by words (two bytes).

### Pad file (/p) ###
`romwak /p <infile> <outfile> <padsize> <padbyte>`  
Pads the input file to <padsize> Kilobytes with the specified byte.

* `<outfile>` is currently not optional. This may change in a future release.
* `<padsize>` is multiplied by 1024, so for 64KB, enter 64 here, not 65535.
* `<padbyte>` values are currently only accepted as decimal (0-255).

TODO
----
* More error checking.
 * empty filenames, especially for output files.
* Alternate command syntax (--pad, --merge, --split)
* Other functionality not available in original ROMWak?
