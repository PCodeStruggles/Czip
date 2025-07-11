#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#define SV_IMPLEMENTATION
#include "./sv.h"
#define TOKEN_CAP 1024

typedef struct {
	int tokenId;
	int tokenFreq;
	size_t tokenCount;
	char* tokenData;
} Token;

typedef struct {
	size_t count;
	Token array[TOKEN_CAP];
} Tokens;

char* loadFileContent(const char* filePath);
int isTokenPresent(Tokens tokens, String_View svToken);

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

int isTokenPresent(Tokens tokens, String_View svToken) {
	for(size_t i = 0; i < tokens.count; i++) {
		if(tokens.array[i].tokenData && ((strncmp(tokens.array[i].tokenData, svToken.data, svToken.count)) == 0))
			return i;
	}
	return -1;
}

int main(int argc, char* argv[]) {

	if (argc == 1) {
		printf("%s\n", "Plese provide path to input file");
		return 1;
	}	
	const char* filePath = argv[1];

	char* fileData = loadFileContent(filePath);
	
	Tokens tokens = { .count = 0, .array = {0} };
	
	//Open file STREAM to output file
	const char* outFilePath = "output.txt";
	FILE* outfptr;
	if((outfptr = fopen(outFilePath, "w")) == NULL) {
		printf("ERROR: %d %s - %s\n", errno, outFilePath, strerror(errno));
		return 1;
	}

	String_View fileContent = { .count = strlen(fileData), .data = fileData };
	int tokenID = 0;
	while(fileContent.count > 0) {

		String_View svToken = sv_chop_by_delim(&fileContent, ' ');

		//Either increment freq if token already in token array
		//or insert token at the end of the token array
		int i = isTokenPresent(tokens, svToken);
		if(i != -1){
			tokens.array[i].tokenFreq++;
			} else {
			assert(tokens.count < TOKEN_CAP && "Tokens array overflow");
			Token token = {
				.tokenId = tokenID,
				.tokenFreq = 1,
				.tokenCount = svToken.count,
				.tokenData = malloc(svToken.count)
				};
			memcpy(token.tokenData, svToken.data, svToken.count);
			tokens.array[tokens.count++] = token;
			tokenID++;
			}
		}
		free(fileData);

		//Tokens sorting in Freq decreasing order
		//Bubble Sort for now
		for(size_t i = 0; i < tokens.count; i++) {
			for(size_t j = 0; j < tokens.count -1 -i; j++) {
				if(tokens.array[j].tokenFreq < tokens.array[j + 1].tokenFreq) {
					const Token tmp = tokens.array[j];
					tokens.array[j] = tokens.array[j + 1];
					tokens.array[j + 1] = tmp;
				}
			}
		}
	
		//Generate output file
		for(size_t i = 0; i < tokens.count; i++) {
			fprintf(outfptr, "TOKEN: '%s'\n\tID: %d FREQ: %d COUNT: %zu\n", 
					tokens.array[i].tokenData, 
					tokens.array[i].tokenId, 
					tokens.array[i].tokenFreq, 
					tokens.array[i].tokenCount);
		}

	fclose(outfptr);
	return 0;
}

