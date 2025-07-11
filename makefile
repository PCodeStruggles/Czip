CC = gcc
CFLAGS = -Wall -Wextra -pedantic

main: main.c
	$(CC) $(CFLAGS) main.c -o main

clean:
	rm main
	rm output.txt
