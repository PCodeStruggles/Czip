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
} encoderToken;

typedef struct {
	size_t count;
	encoderToken array[TOKEN_CAP];
} encoderTokens;

char* loadFileContent(const char* filePath);
encoderTokens generateTokensArray(char* fileData);
void generateTokensIDs(encoderTokens* tokens);
int isTokenPresent(encoderTokens tokens, String_View svToken);
void generateTranslation(char* fileData, encoderTokens tokens, const char* outputFilePath);
void tokensArrayDumpToFile(encoderTokens tokens, const char* outputFilePath);
