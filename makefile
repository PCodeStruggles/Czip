CC = gcc
CFLAGS = -Wall -Wextra -pedantic

all: encoder decoder

./bin/czip.o: ./src/czip.c ./src/czip.h
	mkdir -p ./bin
	$(CC) $(CFLAGS) -c ./src/czip.c -o ./bin/czip.o

encoder: ./bin/czip.o ./src/encoder.c ./src/czip.h
	$(CC) $(CFLAGS) ./src/encoder.c ./bin/czip.o -o ./bin/encoder
	mkdir -p output

decoder: ./bin/czip.o ./src/decoder.c ./src/czip.h
	$(CC) $(CFLAGS) ./src/decoder.c ./bin/czip.o -o ./bin/decoder
clean:
	rm ./bin/encoder
	rm ./bin/decoder
	rm ./bin/czip.o
	rm -r output

make clout:
	rm -r output
