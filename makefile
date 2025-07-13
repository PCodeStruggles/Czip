CC = gcc
CFLAGS = -Wall -Wextra -pedantic

encoder: encoder.c czip.c czip.h
	$(CC) $(CFLAGS) -c czip.c -o czip.o
	$(CC) $(CFLAGS) encoder.c czip.o -o encoder

decoder: decoder.c czip.c czip.h
	$(CC) $(CFLAGS) decoder.c czip.o -o decoder
clean:
	rm encoder
	rm decoder
	rm czip.o
	rm textAnalysis.txt
	rm translation.txt 

make clout:
	rm textAnalysis.txt
	rm translation.txt 
