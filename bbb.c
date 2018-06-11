
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>


#include "aaa.h"

int main(int argc, char * argv[])
{

	int i, fdsrc, fdtag, nl;
	char * filename;
	char *srcname, *tagname;
	FILE * fpsrc;
	FILE * fptag;

	
	filename = (char*)argv[1];
	if(filename == NULL){
		printf("No txt file.");
		return 1;
	}

	srcname = (char *)malloc(32);
	tagname = (char *)malloc(32);
	
	nl = strlen(filename);
	strcpy(srcname, filename);
	strcpy(tagname, filename);

	if(((strncmp(filename + (nl-4), ".txt", 4))==0) || ((strncmp(filename + (nl-4), ".TXT", 4))==0)){
		strncpy(tagname + (nl-3), "bin", 3);
	}else{
		srcname = strcat(srcname, ".txt");
		tagname = strcat(tagname, ".bin");
	}


	fpsrc = fopen(srcname, "r+");
	if(!fpsrc){
		printf("open source file fail.");
		return 1;
	}

	fptag = fopen(tagname, "wb+");
	if(!fptag){
		printf("open taget file fail.");
		return 1;
	}

	printf("\r\n");
	printf("Input file is : %s.\r\n", srcname);
	printf("Output file is : %s.\r\n", tagname);

//	printf("Processing......\r\n");

	nl = find_sectors(fpsrc, fptag);

	printf("\r\nDown. %d sectors Total.\r\n", nl);
////////////////////////
// release mem
////////////////////////
	if(srcname)
		free(srcname);
	if(tagname)
		free(tagname);
	if(fpsrc)
		fclose(fpsrc);
	if(fptag)
		fclose(fptag);

	return 0;

}
