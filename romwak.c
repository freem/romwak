/* ROMWak (ANSI C port by freem, original by Jeff Kurtz)
 *
 * ROMWak was originally programmed in Delphi. This port is in (mostly) ANSI C.
 * The sources given to me were of version 0.3, though I have a version 0.4
 * binary on my computer. I have not analyzed the differences between the two
 * versions, as I do not have access to Delphi build tools.
 */
#include <stdio.h>
#include <stdlib.h>

#include "romwak.h"

/* Usage() - Print program usage. */
void Usage(){
	printf("usage: romwak <option> <infile> <outfile> [outfile2] [psize] [pbyte]\n");
	printf("You must use one of these options:\n");
	printf(" /b - Split file into two files, alternating bytes into separate files.\n");
	printf(" /f - Flip low/high bytes of a file. (<outfile1> optional.)\n");
	printf(" /h - Split file in half (two files).\n");
	printf(" /m - Byte merge two files. (stores results in <outfile2>).\n");
	printf(" /s - Swap top and bottom halves of a file. (<outfile1> optional.)\n");
	printf(" /w - Split file into two files, alternating words into output files.\n");
	printf(" /p - Pad file to [psize] in K with [pbyte] value (0-255).\n");
	printf("\n");
	printf("NOTE: Omission of [outfile2] will result in the second file not being saved.\n");
}
/*----------------------------------------------------------------------------*/

/* FileExists(char *fileIn) - Helper function to determine if a file exists. */
bool FileExists(char *fileIn){
	/* try opening file */
	FILE *f = fopen(fileIn,"r");
	if(f != NULL){
		/* file open was successful, we don't need this handle. */
		fclose(f);
		return true;
	}
	/* file open unsuccessful, print error */
	perror("Error attempting to open file");
	return false;
}
/*----------------------------------------------------------------------------*/

/* EqualSplit(char *fileIn, char *fileOutA, char *fileOutB) - /h
 * Splits a file in half equally.
 * (Get filesize, divide it by 2, and split the data. Easy enough.)
 *
 * (Params)
 * char *fileIn			Input filename
 * char *fileOutA		Output filename 1
 * char *fileOutB		Output filename 2
 */
int EqualSplit(char *fileIn, char *fileOutA, char *fileOutB){
	if(!FileExists(fileIn)){
		return EXIT_FAILURE;
	}

	/* FILE *pInFile, *pOutFile1, *pOutFile2; */

	return EXIT_SUCCESS;
}
/*----------------------------------------------------------------------------*/

/* ByteSplit(char *fileIn, char *fileOutA, char *fileOutB) - /b
 *
 * (Params)
 * char *fileIn			Input filename
 * char *fileOutA		Output filename 1
 * char *fileOutB		Output filename 2
 */
int ByteSplit(char *fileIn, char *fileOutA, char *fileOutB){
	if(!FileExists(fileIn)){
		return EXIT_FAILURE;
	}

	/* FILE *pInFile, *pOutFile1, *pOutFile2; */

	return EXIT_SUCCESS;
}
/*----------------------------------------------------------------------------*/

/* WordSplit(char *fileIn, char *fileOutA, char *fileOutB) - /w
 *
 * (Params)
 * char *fileIn			Input filename
 * char *fileOutA		Output filename 1
 * char *fileOutB		Output filename 2
 */
int WordSplit(char *fileIn, char *fileOutA, char *fileOutB){
	if(!FileExists(fileIn)){
		return EXIT_FAILURE;
	}

	/* FILE *pInFile, *pOutFile1, *pOutFile2; */

	return EXIT_SUCCESS;
}
/*----------------------------------------------------------------------------*/

/* FlipByte(char *fileIn, char *fileOut) - /f
 * Flip low/high bytes of a file.
 *
 * (Params)
 * char *fileIn			Input filename
 * char *fileOut		Output filename
 */
int FlipByte(char *fileIn, char *fileOut){
	if(!FileExists(fileIn)){
		return EXIT_FAILURE;
	}

	/* FILE *pInFile, *pOutFile; */

	return EXIT_SUCCESS;
}
/*----------------------------------------------------------------------------*/

/* MergeBytes(char *fileIn1, char *fileIn2, char *fileOut) - /m
 * Byte merges two files; stores result in fileOut.
 *
 * (Params)
 * char *fileIn1		Input filename 1
 * char *fileIn2		Input filename 2
 * char *fileOut		Output filename
 */
int MergeBytes(char *fileIn1, char *fileIn2, char *fileOut){
	if(!FileExists(fileIn1)){
		return EXIT_FAILURE;
	}
	if(!FileExists(fileIn2)){
		return EXIT_FAILURE;
	}

	/* FILE *pInFile1, *pInFile2, *pOutFile; */

	return EXIT_SUCCESS;
}
/*----------------------------------------------------------------------------*/

/* SwapHalf(char *fileIn, char *fileOut) - /s
 * Swaps the top and bottom halves of fileIn; writes to fileOut.
 *
 * (Params)
 * char *fileIn			Input filename
 * char *fileOut		Output filename
 */
int SwapHalf(char *fileIn, char *fileOut){
	if(!FileExists(fileIn)){
		return EXIT_FAILURE;
	}

	/*FILE *pInFile, *pOutFile;*/

	return EXIT_SUCCESS;
}
/*----------------------------------------------------------------------------*/

/* PadFile(char *fileIn, char *fileOut, char *padSize, char *padByte) - /p
 * Pads fileIn to padSize kilobytes with specified padByte; writes to fileOut.
 *
 * (Params)
 * char *fileIn			Input filename
 * char *fileOut		Output filename
 * char *padSize		Size to pad file to (in Kilobytes)
 * char *padByte		Value to pad with
 */
int PadFile(char *fileIn, char *fileOut, char *padSize, char *padByte){
	if(!FileExists(fileIn)){
		return EXIT_FAILURE;
	}

	FILE *pInFile, *pOutFile;
	pInFile = fopen(fileIn,"rb");
	if(pInFile == NULL){
		perror("Error attempting to open input file");
		exit(EXIT_FAILURE);
	}

	/* find current file size */
	long length = 0;
	fseek(pInFile,0,SEEK_END); /* SEEK_END is non-portable, sorry */
	length = ftell(pInFile);
	rewind(pInFile);

	/* copy data into buffer */
	long fullPadSize = (atoi(padSize))*1024;
	long bufLength = fullPadSize;
	unsigned char *buffer = (unsigned char*)malloc(bufLength);
	if(buffer == NULL){
		printf("Error allocating memory for input file buffer.");
		exit(EXIT_FAILURE);
	}

	size_t result = fread(buffer,sizeof(unsigned char),length,pInFile);
	if(result != length){
		perror("Error reading input file");
		exit(EXIT_FAILURE);
	}

	fclose(pInFile);

	/* add padding to buffer */
	long remain = fullPadSize-(sizeof(unsigned char)*length);
	long bufPos = result;
	long i = 0;
	while(i<remain){
		buffer[bufPos] = (unsigned char)atoi(padByte);
		bufPos++;
		i++;
	}

	/* create new file with padding */
	pOutFile = fopen(fileOut,"wb");
	if(pOutFile == NULL){
		perror("Error attempting to create output file");
		exit(EXIT_FAILURE);
	}

	/* write output file */
	result = fwrite(buffer,sizeof(unsigned char),bufLength,pOutFile);
	if(result != bufLength){
		perror("Error writing output file");
		exit(EXIT_FAILURE);
	}
	fclose(pOutFile);
	printf("'%s' saved successfully!\n",fileOut);

	free(buffer);
	return EXIT_SUCCESS;
}
/*----------------------------------------------------------------------------*/

/* ye olde main */
int main(int argc, char* argv[]){
	printf("ROMWak 0.x - freem port; original by Jeff Kurtz\n");
	if(argc < 2){
		Usage();
		return EXIT_FAILURE; /* failure to run due to no options */
	}

	/* command line argument parsing (originally in ROMWAK.DPR) */
	/* The original program used /switches, but I might want to support
	 * --switches as well, for people who aren't on Windows... */
	if(argv[1][0] != '/'){
		printf("ERROR: Invalid command %s\n\n",argv[1]);
		Usage();
		return EXIT_FAILURE; /* command syntax is wrong, broheim */
	}
	else{
		switch(argv[1][1]){
			case 'b': /* split file in two, alternating bytes */
				return ByteSplit(argv[2],argv[3],argv[4]);

			case 'f': /* flip low/high bytes */
				return FlipByte(argv[2],argv[3]);

			case 'h': /* split file in half (two files) */
				return EqualSplit(argv[2],argv[3],argv[4]);

			case 'm': /* byte merge two files */
				return MergeBytes(argv[2],argv[3],argv[4]);

			case 's': /* swap top and bottom halves of a file */
				return SwapHalf(argv[2],argv[3]);

			case 'w': /* split file in two, alternating words */
				return WordSplit(argv[2],argv[3],argv[4]);

			case 'p': /* pad file */
				return PadFile(argv[2],argv[3],argv[4],argv[5]);

			default:
				/* option does not exist */
				printf("ERROR: Option '/%c' doesn't exist.\n",argv[1][1]);
				return EXIT_FAILURE;
		}
	}

	/* should not get here, but if it does, let's not make a fuss. */
	return EXIT_SUCCESS;
}
