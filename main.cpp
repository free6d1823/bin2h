#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>

using namespace std;
void usage(char* name)
{
    printf("Usage: %s <input_file> [-f <output_h>] [-v <var_name>]\n", name);
    printf("Convert a binary file to header file.\n\n");
    printf("\tinput_file       input binary filename.\n");
    printf("\t-f <output_h>    output header filename.\n");
    printf("\t-v <var_name>    declared variable name.\n");
}
void printHeader(FILE* fp, char* name, char* sourceName)
{
	time_t T= time(NULL);
	struct  tm tm = *localtime(&T);

	fprintf(fp, "/*********************************************************************\n");
	fprintf(fp, "\tCopyright (C) 2020  Siengine Inc.,\n");

	fprintf(fp, "\tImage data converted from %s\n", sourceName);
	fprintf(fp, "\tDate: %04d/%02d/%02d %02d:%02d:%02d  \n", 
			tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	fprintf(fp, " **********************************************************************/\n\n");

	fprintf(fp, "#ifndef _%s_\n", name);
	fprintf(fp, "#define _%s_\n", name);
	fprintf(fp, "static unsigned char %s[] = {\n", name); 
}
/* return length of elements */
int printData(FILE* fpIn, FILE* fpOut)
{
	unsigned char data;
	int length = 0;
	while( 1 == fread(&data, 1,1, fpIn)) {
		if (length%10 == 0)
			fprintf(fpOut, "\t");
		fprintf(fpOut, "0x%02X,", data);
		length++;
		if (length%10 == 0)
			fprintf(fpOut, "\n");

	}
	fprintf(fpOut, "\n");
	return length;
}
void printEnd(FILE* fp, char* name)
{
	fprintf(fp, "};\n\n");
	fprintf(fp, "#endif //_%s_\n", name);
}
int main(int argc, char *argv[])
{
	FILE* fpIn = NULL;
	FILE* fpOut = NULL;

	char* szInFile = NULL;
	char* szOutFile = NULL;
	char szName[64] = "bitstream";
	char ch;

	while ((ch = getopt(argc, argv, "f:v:h?")) != -1)
	{
		switch (ch)
		{
		case 'f':
		    szOutFile = optarg;
		    break;
		case 'v':
		    strncpy((char*)szName, optarg, 63);
		    break;
		default:
		    usage(argv[0]);
		    exit(-1);
		}            
	}

    szInFile = argv[optind];

    fpIn = fopen(szInFile, "rb");
    if (!fpIn) {
        fprintf(stderr, "Cannot open input file %s!\n", szInFile);
        exit(-1);
    }

    fpOut = fopen(szOutFile, "wt");
    if (fpOut == NULL)
        fpOut = stdout;

	/* print header */
	printHeader(fpOut, szName, szInFile);
	int len = printData(fpIn, fpOut);

	fprintf(fpOut, "\t/* Total %d bytes */\n", len);
	printEnd(fpOut, szName);
	if(fpIn)
		fclose(fpIn);
	if(fpOut)
		fclose(fpOut);
    return 0;
}

