CC = gcc

CFLAGS = `pkg-config --cflags gtk+-3.0`

LDFLAGS = `pkg-config --libs gtk+-3.0`

SOURCES = newmenu.c

OBJECTS = newmenu.o

EXECUTABLE = gant

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f $(OBJECTS) $(EXECUTABLE)