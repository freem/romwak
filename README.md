ROMWak
======
Original program by Jeff Kurtz, ported to C by freem

Usage
-----
ROMWak is a program whose parameters change depending on what option you pass in.

The base command is romwak <option>, where the options are:
/b - Split file into two files, alternating bytes into separate files.
/f - Flip low/high bytes of a file. (<outfile1> optional.)
/h - Split file in half (two files).
/m - Byte merge two files. (stores results in <outfile2>).
/s - Swap top and bottom halves of a file. (<outfile1> optional.)
/w - Split file into two files, alternating words into output files.
/p - Pad file to [psize] in K with [pbyte] value (0-255).

The program also supports shorthand -params (e.g. '-b', '-p', and so on).

### Split File in Two, Alternating Bytes (/b) ###
romwak /b <infile> <outfile1> <outfile2>
Splits the specified input file into two files by words (two bytes).

### Flip High/Low Bytes (/f) ###
romwak /f <infile> [<outfile>]
Flips the high and low bytes of the specified file.

<outfile> is optional; if omitted, the file will be swapped in place.

### Split File in Half (/h) ###
romwak /h 

### Byte Merge Two Files (/m) ###
romwak /m <infile1> <infile2> <outfile>
Merges the bytes of infile1 and infile2 to create outfile.

The byte of infile1 is written, then the byte of infile2 is written;
repeat for the entire length of the file.

### Swap Top and Bottom Halves of File (/s) ###
romwak /s <infile> [<outfile>]
Swaps the top and bottom halves of the file.

This works pretty awkwardly with odd length files (one byte will be missing).

<outfile> is optional; if omitted, the file will be swapped in place.

### Split File Two, Alternating Words (/w) ###
romwak /w <infile> <outfile1> <outfile2>
Splits the specified input file into two files by words (two bytes).

### Pad file (/p) ###
romwak /p <infile> <outfile> <padsize> <padbyte>
Pads the specified input file to <padsize> Kilobytes with the specified byte.

<outfile> is currently not optional. This may change in a future release.
<padsize> is multiplied by 1024, so for 64KB, enter 64 here, not 65535.
<padbyte> values are currently only accepted as decimal (0-255).

TODO
----
* More error checking.
 * empty filenames, especially for output files.
* Some functions have the ability for output file to be blank; this isn't handled yet.
* Alternate command syntax (-p, -m, -s or --pad, --merge, --split)
* Other functionality not available in original ROMWak?
