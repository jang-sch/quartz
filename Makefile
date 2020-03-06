CC=g++
CFLAGS=-std=c++11 -Wall 
CFLAGS+=-I./include
LFLAGS=-L./lib -lSDL2main -lSDL2 -lGL -lGLEW

all: bin/main

debug: CFLAGS+= -DDEBUG
debug: bin/main

bin/main: main.cpp
	@mkdir -p bin
	$(CC) main.cpp $(CFLAGS) -o bin/main $(LFLAGS)

# Defined so make doesn't think these are filenames
.PHONY: clean veryclean

# Deletes only the built executable
clean:
	@rm -f bin/main

# Deletes the whole bin directory
veryclean:
	@rm -rf bin

.PHONY: archive

# Creates a tarball of the directory contents

DIRNAME=SDL_demo

archive: veryclean
	cd .. ; tar -czvf $(DIRNAME).tgz $(DIRNAME) ; mv $(DIRNAME).tgz $(DIRNAME)/.