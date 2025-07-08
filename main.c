#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define SV_IMPLEMENTATION
#include "./sv.h"

typedef struct {
	int tokenId;
	int tokenFreq;
	size_t tokenCount;
	char* tokenData;
} Token;

char* loadFileContent(const char* filePath) {

	FILE* fptr;
	//Open file
	if ((fptr = fopen(filePath, "r")) == NULL) {
		printf("ERROR: %d %s - %s\n", errno, filePath, strerror(errno));
		exit(EXIT_FAILURE);
	}
	//Set cursor to file end to get file size in bytes
	if(!(fseek(fptr, 0, SEEK_END) == 0)){
		printf("ERROR: %d - %s\n", errno, strerror(errno));
		exit(EXIT_FAILURE);
	}
	//Read cursor position in bytes	
	long int fileSize = 0;
	if((fileSize = ftell(fptr)) == -1) {
		printf("ERROR: %d - %s\n", errno, strerror(errno));
		exit(EXIT_FAILURE);
	}
	//Reset cursor position to read file from the beginning
	fseek(fptr, 0, SEEK_SET);

	//Allocate the file content buffer on the heap
	static char* fileData;
	if((fileData = malloc(fileSize)) == NULL) {
		printf("ERROR: %d - %s\n", errno, strerror(errno));
		exit(EXIT_FAILURE);
	}

	//Read content file into the buffer pointed to by fileData
	size_t bytesRead = 0;
	if((bytesRead = fread(fileData, sizeof(char), fileSize, fptr)) != fileSize){
		printf("ERROR: %d - %s\n", errno, strerror(errno));
		exit(EXIT_FAILURE);
	}

	//Log results
	printf("Input file: %s\nFile size: %ld\n", filePath, fileSize);
	printf("Number of bytes read into buffer: %zu\n", bytesRead);

	//Return pointer to buffer
	return fileData;
}

int main(int argc, char* argv[]) {

	if (argc == 1) {
		printf("%s\n", "Plese provide path to input file");
		return 1;
	}	
	
	const char* filePath = argv[1];

	char* fileData = loadFileContent(filePath);
	
	const char* outFilePath = "output.txt";
	FILE* outfptr;
	if((outfptr = fopen(outFilePath, "w")) == NULL) {
		printf("ERROR: %d %s - %s\n", errno, outFilePath, strerror(errno));
		exit(EXIT_FAILURE);
	}

	String_View fileContent = { .count = strlen(fileData), .data = fileData };
	int idCount = 0;
	while(fileContent.count > 0) {
		String_View svToken = sv_chop_by_delim(&fileContent, ' ');
		Token token = {
			.tokenId = idCount,
			.tokenFreq = 0,
			.tokenCount = svToken.count,
			.tokenData = malloc(svToken.count)
		};
		memcpy(token.tokenData, svToken.data, svToken.count);
		if((fprintf(outfptr, "TOKEN: %s\n\tTOKEN ID: %d - TOKEN FREQ: %d - TOKEN COUNT: %zu\n",
				token.tokenData, token.tokenId, token.tokenFreq, token.tokenCount)) == -1) {
			printf("ERROR: %d %s - %s\n", errno, filePath, strerror(errno));
			exit(EXIT_FAILURE);
		}
		idCount++;
	}
	fclose(outfptr);
	free(fileData);
	return 0;
}
