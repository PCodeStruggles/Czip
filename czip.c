#define SV_IMPLEMENTATION
#include "czip.h"

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
	char* fileData;
	if((fileData = (char*)malloc(fileSize)) == NULL) {
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
				.tokenData = (char*)malloc(svToken.count)
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
		fprintf(tokenTable, "%d|%s\n", tokens.array[i].tokenId,
				tokens.array[i].tokenData);
		}

	if((fclose(tokenTable)) != 0) exit(EXIT_FAILURE); 
	printf("Text analysis: %s\n", outputFilePath); 
	}

