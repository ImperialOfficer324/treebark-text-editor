CFLAGS = `pkg-config --cflags gtk+-3.0 --libs gtk+-3.0` -g
CC = gcc

LIBS = 

C_FILES = main.c
OBJ_FILES = main.o
OUT_FILE = ../build/installer
all: installer

installer: $(OBJ_FILES)
		$(CC) $(CFLAGS) -o $(OUT_FILE) $(OBJ_FILES)

main.o: main.c
		$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm *.o