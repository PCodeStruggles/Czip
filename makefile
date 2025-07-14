CC = gcc
CFLAGS = -Wall -Wextra -pedantic

encoder: ./src/encoder.c ./src/czip.c ./src/czip.h
	$(CC) $(CFLAGS) -c ./src/czip.c -o ./bin/czip.o
	$(CC) $(CFLAGS) ./src/encoder.c ./bin/czip.o -o ./bin/encoder
	mkdir output

decoder: ./src/decoder.c ./src/czip.c ./src/czip.h
	$(CC) $(CFLAGS) ./src/decoder.c ./bin/czip.o -o ./bin/decoder
clean:
	rm ./bin/encoder
	rm ./bin/decoder
	rm ./bin/czip.o
	rm -r output

make clout:
	rm -r output
