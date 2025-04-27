CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude
LIBS = 

all: goldis

goldis: main.o server.o handler.o
	$(CC) $(CFLAGS) -o goldis main.o server.o handler.o $(LIBS)
	
clean:
	rm -f goldis *.o

