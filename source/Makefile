CFLAGS = `pkg-config --cflags gtk+-3.0 --libs gtk+-3.0` -g
CC = gcc

LIBS = 

C_FILES = main.c settings.c message.c
OBJ_FILES = main.o settings.o message.o
OUT_FILE = ../build/treebark
all: treebark

treebark: $(OBJ_FILES)
		$(CC) $(CFLAGS) -o $(OUT_FILE) $(OBJ_FILES)

main.o: main.c
		$(CC) $(CFLAGS) -c -o $@ $^

settings.o: settings.c
		$(CC) $(CFLAGS) -c -o $@ $^

message.o: message.c
		$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm *.o