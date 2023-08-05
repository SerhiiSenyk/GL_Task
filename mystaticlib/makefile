CC=gcc
CFLAGS=-Wall

SRC = file_searcher.c
OBJ = $(SRC:.c=.o)
OUT = libmysearcher.a
INCLUDES = -I.

.SUFFIXES: .c

default: $(OUT)

.c.o:
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

$(OUT): $(OBJ)
	ar rcs $(OUT) $(OBJ)