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
Tokens generateTokensArray(char* fileData);
void generateTokensIDs(Tokens* tokens);
int isTokenPresent(Tokens tokens, String_View svToken);
void generateTranslation(char* fileData, Tokens tokens, const char* outputFilePath);
void tokensArrayDumpToFile(Tokens tokens, const char* outputFilePath);

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

//O(n)
int isTokenPresent(Tokens tokens, String_View svToken) {
	for(size_t i = 0; i < tokens.count; i++) {
		if(tokens.array[i].tokenData && ((strncmp(tokens.array[i].tokenData, svToken.data, svToken.count)) == 0))
			return i;
	}
	return -1;
}

//O(n)
void generateTokensIDs(Tokens* tokens) {
	for (size_t i = 0; i < tokens->count; i++) {
		tokens->array[i].tokenId = i;
	}
}

//O(n^3)??
Tokens generateTokensArray(char* fileData) {

	String_View fileContent = { .count = strlen(fileData), .data = fileData };
	Tokens tokens = { .count = 0, .array = {0} };

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
				.tokenId = 0,
				.tokenFreq = 1,
				.tokenCount = svToken.count,
				.tokenData = malloc(svToken.count)
				};
			memcpy(token.tokenData, svToken.data, svToken.count);
			tokens.array[tokens.count++] = token;
			}
		}

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
	generateTokensIDs(&tokens);
	return tokens;
}

void generateTranslation(char* fileData, Tokens tokens, const char* outputFilePath) {

	String_View fileContent = { .count = strlen(fileData), .data = fileData };

	FILE* textTranslation;
	if((textTranslation = fopen(outputFilePath, "w")) == NULL) {
		printf("ERROR: %d %s - %s\n", errno, outputFilePath, strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	while(fileContent.count > 0) {
		String_View svToken = sv_chop_by_delim(&fileContent, ' ');
		int i = isTokenPresent(tokens, svToken);
		fprintf(textTranslation, "%d ", tokens.array[i].tokenId);
	}
	if((fclose(textTranslation)) != 0) exit(EXIT_FAILURE); 
	printf("Text translation: %s\n", outputFilePath); 
}

//O(n)
void tokensArrayDumpToFile(Tokens tokens, const char* outputFilePath) {
	FILE* tokenTable;
	if((tokenTable = fopen(outputFilePath, "w")) == NULL) {
		printf("ERROR: %d %s - %s\n", errno, outputFilePath, strerror(errno));
		exit(EXIT_FAILURE);
		}

	for(size_t i = 0; i < tokens.count; i++) {
		fprintf(tokenTable, "[%d][%s][Freq: %d]\n", tokens.array[i].tokenId,
				tokens.array[i].tokenData, tokens.array[i].tokenFreq);
		}

	if((fclose(tokenTable)) != 0) exit(EXIT_FAILURE); 
	printf("Text analysis: %s\n", outputFilePath); 
	}

int main(int argc, char* argv[]) {

	if (argc == 1) {
		printf("%s\n", "Plese provide path to input file");
		return 1;
	}	

	const char* filePath = argv[1];

	char* fileData = loadFileContent(filePath);
	
	Tokens tokens = generateTokensArray(fileData);
	
	tokensArrayDumpToFile(tokens, "textAnalysis.txt");
	generateTranslation(fileData, tokens, "translation.txt");
	free(fileData);
	return 0;
}

