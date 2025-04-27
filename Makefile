CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude
LIBS = 

all: goldis

goldis: goldis.c
	$(CC) $(CFLAGS) -o goldis goldis.c $(LIBS)
	
clean:
	rm goldis

