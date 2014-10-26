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

#define ROMWAK_VERSION	"0.3c" /* derived from 0.3 source code; see above note */

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
	printf("\n");
	printf("See the included README.md for more details. If README.md was not included,\n");
	printf("please visit https://github.com/freem/romwak/\n");
}
/*----------------------------------------------------------------------------*/

/* FileExists(char *fileIn) - Helper function to determine if a file exists.
 *
 * (Params)
 * char *fileIn			Target filename
 */
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

/* FileSize(FILE *pFile) - Helper function to determine a file's size.
 * Does not rewind the file, you must do it yourself.
 *
 * (Params)
 * FILE *pFile			Handle to a loaded file.
 */
long FileSize(FILE *pFile){
	fseek(pFile,0,SEEK_END); /* SEEK_END is non-portable, sorry */
	return ftell(pFile);
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
	printf("Splitting file '%s' equally, saving to '%s' and '%s'\n",fileIn,fileOutA,fileOutB);

	FILE *pInFile, *pOutFile1, *pOutFile2;

	pInFile = fopen(fileIn,"rb");
	if(pInFile == NULL){
		perror("Error attempting to open input file");
		exit(EXIT_FAILURE);
	}

	/* find file size */
	long length = FileSize(pInFile);
	rewind(pInFile);

	/* copy data into buffer */
	unsigned char *inBuffer = (unsigned char*)malloc(length);
	if(inBuffer == NULL){
		printf("Error allocating memory for input file buffer.");
		exit(EXIT_FAILURE);
	}

	size_t result = fread(inBuffer,sizeof(unsigned char),length,pInFile);
	if(result != length){
		perror("Error reading input file");
		exit(EXIT_FAILURE);
	}
	fclose(pInFile);

	/* prepare output buffers */
	long halfLength = length/2;
	unsigned char *outBuf1 = (unsigned char*)malloc(halfLength);
	if(outBuf1 == NULL){
		printf("Error allocating memory for first output file buffer.");
		exit(EXIT_FAILURE);
	}
	unsigned char *outBuf2 = (unsigned char*)malloc(halfLength);
	if(outBuf2 == NULL){
		printf("Error allocating memory for second output file buffer.");
		exit(EXIT_FAILURE);
	}

	/* fill buffers */
	long i = 0;
	long pos = 0;
	unsigned char b1, b2;
	while(i<halfLength){
		b1 = inBuffer[i];
		b2 = inBuffer[i+halfLength];
		outBuf1[pos] = b1;
		outBuf2[pos] = b2;
		i++;
		pos++;
	}
	free(inBuffer);

	/* write output files */
	pOutFile1 = fopen(fileOutA,"wb");
	if(pOutFile1 == NULL){
		perror("Error attempting to create first output file");
		exit(EXIT_FAILURE);
	}
	result = fwrite(outBuf1,sizeof(unsigned char),halfLength,pOutFile1);
	if(result != halfLength){
		perror("Error writing first output file");
		exit(EXIT_FAILURE);
	}
	fclose(pOutFile1);
	printf("'%s' saved successfully!\n",fileOutA);
	free(outBuf1);

	pOutFile2 = fopen(fileOutB,"wb");
	if(pOutFile2 == NULL){
		perror("Error attempting to create second output file");
		exit(EXIT_FAILURE);
	}
	result = fwrite(outBuf2,sizeof(unsigned char),halfLength,pOutFile2);
	if(result != halfLength){
		perror("Error writing second output file");
		exit(EXIT_FAILURE);
	}
	fclose(pOutFile2);
	printf("'%s' saved successfully!\n",fileOutB);
	free(outBuf2);

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
	printf("Splitting file '%s' into bytes, saving to '%s' and '%s'\n",fileIn,fileOutA,fileOutB);

	FILE *pInFile, *pOutFile1, *pOutFile2;

	pInFile = fopen(fileIn,"rb");
	if(pInFile == NULL){
		perror("Error attempting to open input file");
		exit(EXIT_FAILURE);
	}

	/* find file size */
	long length = FileSize(pInFile);
	rewind(pInFile);

	/* copy data into buffer */
	unsigned char *inBuffer = (unsigned char*)malloc(length);
	if(inBuffer == NULL){
		printf("Error allocating memory for input file buffer.");
		exit(EXIT_FAILURE);
	}

	size_t result = fread(inBuffer,sizeof(unsigned char),length,pInFile);
	if(result != length){
		perror("Error reading input file");
		exit(EXIT_FAILURE);
	}
	fclose(pInFile);

	/* prepare output buffers */
	long halfLength = length/2;
	unsigned char *outBuf1 = (unsigned char*)malloc(halfLength);
	if(outBuf1 == NULL){
		printf("Error allocating memory for first output file buffer.");
		exit(EXIT_FAILURE);
	}
	unsigned char *outBuf2 = (unsigned char*)malloc(halfLength);
	if(outBuf2 == NULL){
		printf("Error allocating memory for second output file buffer.");
		exit(EXIT_FAILURE);
	}

	/* fill buffers */
	long i = 0;
	long pos1 = 0;
	long pos2 = 1;
	while(i<halfLength){
		outBuf1[i] = inBuffer[pos1];
		outBuf2[i] = inBuffer[pos2];
		i++;
		pos1+=2;
		pos2+=2;
	}
	free(inBuffer);

	/* write output files */
	pOutFile1 = fopen(fileOutA,"wb");
	if(pOutFile1 == NULL){
		perror("Error attempting to create first output file");
		exit(EXIT_FAILURE);
	}
	result = fwrite(outBuf1,sizeof(unsigned char),halfLength,pOutFile1);
	if(result != halfLength){
		perror("Error writing first output file");
		exit(EXIT_FAILURE);
	}
	fclose(pOutFile1);
	printf("'%s' saved successfully!\n",fileOutA);
	free(outBuf1);

	pOutFile2 = fopen(fileOutB,"wb");
	if(pOutFile2 == NULL){
		perror("Error attempting to create second output file");
		exit(EXIT_FAILURE);
	}
	result = fwrite(outBuf2,sizeof(unsigned char),halfLength,pOutFile2);
	if(result != halfLength){
		perror("Error writing second output file");
		exit(EXIT_FAILURE);
	}
	fclose(pOutFile2);
	printf("'%s' saved successfully!\n",fileOutB);
	free(outBuf2);

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
	printf("Splitting file '%s' into words, saving to '%s' and '%s'\n",fileIn,fileOutA,fileOutB);

	FILE *pInFile, *pOutFile1, *pOutFile2;

	pInFile = fopen(fileIn,"rb");
	if(pInFile == NULL){
		perror("Error attempting to open input file");
		exit(EXIT_FAILURE);
	}

	/* find file size */
	long length = FileSize(pInFile);
	rewind(pInFile);

	/* copy data into buffer */
	unsigned char *inBuffer = (unsigned char*)malloc(length);
	if(inBuffer == NULL){
		printf("Error allocating memory for input file buffer.");
		exit(EXIT_FAILURE);
	}

	size_t result = fread(inBuffer,sizeof(unsigned char),length,pInFile);
	if(result != length){
		perror("Error reading input file");
		exit(EXIT_FAILURE);
	}
	fclose(pInFile);

	/* prepare output buffers */
	long halfLength = length/2;
	unsigned char *outBuf1 = (unsigned char*)malloc(halfLength);
	if(outBuf1 == NULL){
		printf("Error allocating memory for first output file buffer.");
		exit(EXIT_FAILURE);
	}
	unsigned char *outBuf2 = (unsigned char*)malloc(halfLength);
	if(outBuf2 == NULL){
		printf("Error allocating memory for second output file buffer.");
		exit(EXIT_FAILURE);
	}

	/* fill buffers */
	long i = 0;
	long pos1 = 0;
	long pos2 = 2;
	while(i<halfLength){
		outBuf1[i] = inBuffer[pos1];
		outBuf1[i+1] = inBuffer[pos1+1];
		outBuf2[i] = inBuffer[pos2];
		outBuf2[i+1] = inBuffer[pos2+1];
		i+=2;
		pos1+=4;
		pos2+=4;
	}
	free(inBuffer);

	/* write output files */
	pOutFile1 = fopen(fileOutA,"wb");
	if(pOutFile1 == NULL){
		perror("Error attempting to create first output file");
		exit(EXIT_FAILURE);
	}
	result = fwrite(outBuf1,sizeof(unsigned char),halfLength,pOutFile1);
	if(result != halfLength){
		perror("Error writing first output file");
		exit(EXIT_FAILURE);
	}
	fclose(pOutFile1);
	printf("'%s' saved successfully!\n",fileOutA);
	free(outBuf1);

	pOutFile2 = fopen(fileOutB,"wb");
	if(pOutFile2 == NULL){
		perror("Error attempting to create second output file");
		exit(EXIT_FAILURE);
	}
	result = fwrite(outBuf2,sizeof(unsigned char),halfLength,pOutFile2);
	if(result != halfLength){
		perror("Error writing second output file");
		exit(EXIT_FAILURE);
	}
	fclose(pOutFile2);
	printf("'%s' saved successfully!\n",fileOutB);
	free(outBuf2);

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
	/* If the second file is not passed in, flip the file in place. */
	if(fileOut == NULL){
		fileOut = fileIn;
	}

	printf("Flipping bytes of '%s', saving to '%s'\n",fileIn,fileOut);

	FILE *pInFile, *pOutFile;
	pInFile = fopen(fileIn,"rb");
	if(pInFile == NULL){
		perror("Error attempting to open input file");
		exit(EXIT_FAILURE);
	}

	/* find file size */
	long length = FileSize(pInFile);
	rewind(pInFile);

	/* copy data into buffer */
	unsigned char *buffer = (unsigned char*)malloc(length);
	if(buffer == NULL){
		printf("Error allocating memory for buffer.");
		exit(EXIT_FAILURE);
	}

	size_t result = fread(buffer,sizeof(unsigned char),length,pInFile);
	if(result != length){
		perror("Error reading input file");
		exit(EXIT_FAILURE);
	}
	fclose(pInFile);

	/* Create new file */
	pOutFile = fopen(fileOut,"wb");
	if(pOutFile == NULL){
		perror("Error attempting to create output file");
		exit(EXIT_FAILURE);
	}

	/* flip bytes in buffer */
	long i = 0;
	unsigned char b1, b2;
	while(i<length){
		b1 = buffer[i];
		b2 = buffer[i+1];
		buffer[i+1] = b1;
		buffer[i] = b2;
		i+=2;
	}

	/* write output file */
	result = fwrite(buffer,sizeof(unsigned char),length,pOutFile);
	if(result != length){
		perror("Error writing output file");
		exit(EXIT_FAILURE);
	}
	fclose(pOutFile);
	printf("'%s' saved successfully!\n",fileOut);

	free(buffer);
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

	printf("Merging bytes of '%s' and '%s', saving to '%s'\n",fileIn1,fileIn2,fileOut);

	FILE *pInFile1, *pInFile2, *pOutFile;

	/* Read file 1 */
	pInFile1 = fopen(fileIn1,"rb");
	if(pInFile1 == NULL){
		perror("Error attempting to open first input file");
		exit(EXIT_FAILURE);
	}

	/* find first file size */
	long length1 = FileSize(pInFile1);
	rewind(pInFile1);

	/* put file 1's contents into buffer */
	unsigned char *inBuf1 = (unsigned char*)malloc(length1);
	if(inBuf1 == NULL){
		printf("Error allocating memory for input file buffer 1.");
		exit(EXIT_FAILURE);
	}

	size_t result = fread(inBuf1,sizeof(unsigned char),length1,pInFile1);
	if(result != length1){
		perror("Error reading first input file");
		exit(EXIT_FAILURE);
	}
	fclose(pInFile1);

	/* Read file 2 */
	pInFile2 = fopen(fileIn2,"rb");
	if(pInFile2 == NULL){
		perror("Error attempting to open second input file");
		exit(EXIT_FAILURE);
	}

	/* find second file size */
	long length2 = FileSize(pInFile2);
	rewind(pInFile2);

	/* put file 2's contents into buffer */
	unsigned char *inBuf2 = (unsigned char*)malloc(length2);
	if(inBuf2 == NULL){
		printf("Error allocating memory for input file buffer 2.");
		exit(EXIT_FAILURE);
	}

	result = fread(inBuf2,sizeof(unsigned char),length2,pInFile2);
	if(result != length2){
		perror("Error reading second input file");
		exit(EXIT_FAILURE);
	}
	fclose(pInFile2);

	/* Create new file */
	pOutFile = fopen(fileOut,"wb");
	if(pOutFile == NULL){
		perror("Error attempting to create output file");
		exit(EXIT_FAILURE);
	}

	/* merge bytes into a new buffer */
	long outBufLen = length1+length2;
	unsigned char *outBuf = (unsigned char*)malloc(outBufLen);
	if(outBuf == NULL){
		printf("Error allocating memory for output file buffer.");
		exit(EXIT_FAILURE);
	}

	long i = 0;
	long curPos = 0;
	while(i<length1){
		outBuf[curPos] = inBuf1[i];
		outBuf[curPos+1] = inBuf2[i];
		curPos+=2;
		i++;
	}
	free(inBuf1);
	free(inBuf2);

	/* write output file */
	result = fwrite(outBuf,sizeof(unsigned char),outBufLen,pOutFile);
	if(result != outBufLen){
		perror("Error writing output file");
		exit(EXIT_FAILURE);
	}
	fclose(pOutFile);
	printf("'%s' saved successfully!\n",fileOut);

	free(outBuf);
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
	/* If the second file is not passed in, swap the file in place. */
	if(fileOut == NULL){
		fileOut = fileIn;
	}

	printf("Swapping halves of '%s', saving to '%s'\n",fileIn,fileOut);

	FILE *pInFile, *pOutFile;
	pInFile = fopen(fileIn,"rb");
	if(pInFile == NULL){
		perror("Error attempting to open input file");
		exit(EXIT_FAILURE);
	}

	/* find current file size */
	long length = FileSize(pInFile);
	rewind(pInFile);
	long halfLength = length/2;

	/* read halves of file into two buffers */
	unsigned char *inBufHalf1 = (unsigned char*)malloc(halfLength);
	if(inBufHalf1 == NULL){
		printf("Error allocating memory for input file buffer 1.");
		exit(EXIT_FAILURE);
	}
	size_t result = fread(inBufHalf1,sizeof(unsigned char),halfLength,pInFile);
	if(result != halfLength){
		perror("Error reading input file");
		exit(EXIT_FAILURE);
	}

	unsigned char *inBufHalf2 = (unsigned char*)malloc(halfLength);
	if(inBufHalf2 == NULL){
		printf("Error allocating memory for input file buffer 1.");
		exit(EXIT_FAILURE);
	}
	result = fread(inBufHalf2,sizeof(unsigned char),halfLength,pInFile);
	if(result != halfLength){
		perror("Error reading input file");
		exit(EXIT_FAILURE);
	}

	close(pInFile);

	/* create new file from buffers written in reverse order */
	pOutFile = fopen(fileOut,"wb");
	if(pOutFile == NULL){
		perror("Error attempting to create output file");
		exit(EXIT_FAILURE);
	}

	/* write output file */
	result = fwrite(inBufHalf2,sizeof(unsigned char),halfLength,pOutFile);
	if(result != halfLength){
		perror("Error writing output file");
		exit(EXIT_FAILURE);
	}
	result = fwrite(inBufHalf1,sizeof(unsigned char),halfLength,pOutFile);
	if(result != halfLength){
		perror("Error writing output file");
		exit(EXIT_FAILURE);
	}
	fclose(pOutFile);
	printf("'%s' saved successfully!\n",fileOut);

	free(inBufHalf1);
	free(inBufHalf2);
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

	unsigned int shortPadSize = (atoi(padSize));
	unsigned char padChar = (unsigned char)atoi(padByte);
	printf("Padding '%s' to %d kilobytes with byte 0x%02X, saving to '%s'\n",
		fileIn,shortPadSize,padChar,fileOut);

	FILE *pInFile, *pOutFile;
	pInFile = fopen(fileIn,"rb");
	if(pInFile == NULL){
		perror("Error attempting to open input file");
		exit(EXIT_FAILURE);
	}

	/* find current file size */
	long length = FileSize(pInFile);
	rewind(pInFile);

	/* copy data into buffer */
	long fullPadSize = shortPadSize*1024;
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
		buffer[bufPos] = padChar;
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
	printf("ROMWak %s - original version by Jeff Kurtz / ANSI C port by freem\n",ROMWAK_VERSION);
	if(argc < 2){
		Usage();
		return EXIT_FAILURE; /* failure to run due to no options */
	}

	/* command line argument parsing (originally in ROMWAK.DPR) */
	/* The original program used /switches, but this port allows shorthand
	 * switches with a '-' as well, for people who aren't on Windows. */
	if(argv[1][0] != '/' && argv[1][0] != '-'){
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
