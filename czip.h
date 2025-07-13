#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include "sv.h"

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
