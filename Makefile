CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude
LIBS = 

all: tailee

tailee: tailee.c
	$(CC) $(CFLAGS) -o tailee tailee.c $(LIBS)
	
clean:
	rm tailee

