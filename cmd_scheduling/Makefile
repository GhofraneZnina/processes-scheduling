CC = gcc
CFLAGS = -Wall -Wextra

SOURCES = menu2.c 

OBJECTS = menu2.o 

EXECUTABLE = menu 

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f $(OBJECTS) $(EXECUTABLE)