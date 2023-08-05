CC=gcc
CFLAGS= -L ./mystaticlib -lmysearcher
TARGET=searcher
LDFLAGS=-lpthread
SOURCES=$(wildcard *.c)

OBJECTS=$(SOURCES:.c=.o)

$(TARGET): $(OBJECTS)
	${CC} -o $@ $(OBJECTS) ${LDFLAGS} ${CFLAGS}

%.o: %.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm $(TARGET) $(OBJECTS)
