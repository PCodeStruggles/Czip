#include "czip.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int tokenId;
	const char* data;
} decoderToken;

typedef struct {
	size_t count;
	decoderToken array[TOKEN_CAP];
} decoderTokens;

int isIdPresent(decoderTokens tokens, int translationID);


int isIdPresent(decoderTokens tokens, int translationID){
	for(int i = 0; i < tokens.count; i++) {
		if(tokens.array[i].tokenId == translationID)
			return i;
	}
	return -1;
}

int main(int argc, char* argv[]) {

	if (argc == 1) {
		printf("%s\n", "Plese provide path translation table and encoded text");
		return 1;
	}	

	const char* translationTableFile = argv[1];
	const char* translationToDecodeFile = argv[2];	
	decoderTokens tokens = { .count = 0, .array = {0} };

	char* translationTable = loadFileContent(translationTableFile);

	String_View svTranslationTable = { .count = strlen(translationTable), .data = translationTable };
	while(svTranslationTable.count > 0) {
		String_View svtokenId = sv_chop_by_delim(&svTranslationTable, '|');
		String_View svtoken = sv_chop_by_delim(&svTranslationTable, '\n');
		decoderToken token = { .tokenId = atoi(svtokenId.data), .data = (char*)malloc(svtoken.count) };
		memcpy(token.data, svtoken.data, svtoken.count);
		tokens.array[tokens.count++] = token;
	}
	free(translationTable);

	const char* decoderOutputFile = "./output/decodedText.txt";
	FILE* decoderOutput;
	if ((decoderOutput = fopen(decoderOutputFile, "w")) == NULL) {
		printf("ERROR: %d %s - %s\n", errno, decoderOutputFile, strerror(errno));
		return 1;
	}
	char* translationToDecode = loadFileContent(translationToDecodeFile);

	String_View svTranslationToDecode = { .count = strlen(translationToDecode), .data = translationToDecode };
	while(svTranslationToDecode.count > 0) {
		String_View svtranslationId = sv_chop_by_delim(&svTranslationToDecode, ' ');
		int translationId = atoi(svtranslationId.data);	
		int i = isIdPresent(tokens, translationId);
		fprintf(decoderOutput, "%s ", tokens.array[i].data);
	}
	free(translationToDecode);

	printf("Decode output generated: %s\n", decoderOutputFile);
	fclose(decoderOutput);
	return 0;
}
