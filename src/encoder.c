#include "czip.h"

int main(int argc, char* argv[]) {

	if (argc == 1) {
		printf("%s\n", "Plese provide path to input file");
		return 1;
	}	

	const char* filePath = argv[1];

	char* fileData = loadFileContent(filePath);
	
	encoderTokens tokens = generateTokensArray(fileData);
	
	tokensArrayDumpToFile(tokens, "./output/textAnalysis.txt");
	generateTranslation(fileData, tokens, "./output/translation.txt");
	free(fileData);
	return 0;
}

