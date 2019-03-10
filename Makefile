CC = gcc
CFLAGS = -g Wall

all: main

main: main.o ...o ...o
	$(CC) $(CFLAGS) -o main main.o ..o ...o

main.o: main.c e h dentro dele
	$(CC) -o main.o $(CFLAGS) -c main.c

...igual para os outros .o

clean:
	rm -rf *.o

