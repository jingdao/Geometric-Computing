CC = gcc
INCLUDE_DIRS = -I/usr/include/SDL 
#INCLUDE_DIRS = -I/usr/local/include/SDL2 
CFLAGS = -O2  
debug: CFLAGS = -O0 -ggdb3
LDFLAGS = -lSDL -lSDL_image  
#LDFLAGS = -lSDL2 -lSDL_image  
SOURCES = gcomp.c Pixmap.c Queue.c Map.c List.c HashTable.c Profiler.c Bitmap.c
ALL_SOURCES = $(SOURCES)
HEADERS = Pixmap.h Queue.h Map.h List.h HashTable.h Profiler.h SDL_image.h Bitmap.h
SHARED_OBJECT = libMap.so
SHARED_OBJECT_FLAGS = -shared -fPIC
OBJ = gcomp.o Pixmap.o Queue.o Map.o List.o HashTable.o Profiler.o Bitmap.o
ALL_OBJ = $(OBJ)
EXECUTABLE = gcomp

all: $(ALL_SOURCES) $(EXECUTABLE)

debug: all

shared: $(SOURCES) $(HEADERS) 
	$(CC) $(CFLAGS) -o $(SHARED_OBJECT) $(SOURCES) $(SHARED_OBJECT_FLAGS)

$(EXECUTABLE): $(ALL_OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c -o $@ $<

clean:
	rm -f $(OBJ) $(EXECUTABLE) $(SHARED_OBJECT)
