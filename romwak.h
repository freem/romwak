/* the extremely awkward things one has to do in ANSI C ;) */
typedef int bool;
#define false	0
#define true	1

/* romwak function prototypes */

/* Print usage */
void Usage();

/* [Program Functionality] */
int EqualSplit(char *fileIn, char *fileOutA, char *fileOutB);
int ByteSplit(char *fileIn, char *fileOutA, char *fileOutB);
int WordSplit(char *fileIn, char *fileOutA, char *fileOutB);
int FlipByte(char *fileIn, char *fileOut);
int MergeBytes(char *fileIn1, char *fileIn2, char *fileOut);
int SwapHalf(char *fileIn, char *fileOut);
int PadFile(char *fileIn, char *fileOut, char *padSize, char *padByte);

/* [Helper Functions] */
bool FileExists(char *fileIn);
long FileSize(FILE *pFile);
