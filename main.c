#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char* argv[]) {

	if (argc == 1) {
		printf("%s\n", "Plese provide path to input file");
		return 1;
	}	
	
	const char* filePath = argv[1];
	FILE* fptr;

	if ((fptr = fopen(filePath, "r")) == NULL) {
		printf("ERROR: %d %s - %s\n", errno, filePath, strerror(errno));
		return 1;
	}

	if(!(fseek(fptr, 0, SEEK_END) == 0)){
		printf("ERROR: %d - %s\n", errno, strerror(errno));
		return 1;
	}

	long int fileSize = 0;
	if((fileSize = ftell(fptr)) == -1) {
		printf("ERROR: %d - %s\n", errno, strerror(errno));
		return 1;
	}

	fseek(fptr, 0, SEEK_SET);


	char* fileData;
	if((fileData = malloc(fileSize)) == NULL) {
		printf("ERROR: %d - %s\n", errno, strerror(errno));
		return 1;
	}
	size_t bytesRead = 0;
	if((bytesRead = fread(fileData, sizeof(char), fileSize, fptr)) != fileSize){
		printf("ERROR: %d - %s\n", errno, strerror(errno));
		return 1;
	}

	printf("Input file: %s\nFile size: %ld\n", filePath, fileSize);
	printf("Number of bytes read into buffer: %zu\n", bytesRead);
	return 0;
}
