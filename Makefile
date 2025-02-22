# Ian Leuty ileuty@pdx.edu 2/16/2025

CC = g++

DEFINES =
DEBUG = -g
STANDARD = -std=c++17
WERROR = -Werror
FLAGS = -Wall $(STANDARD) $(DEBUG) $(DEFINES) $(WERROR)
SOURCES = *.cpp
#OBJECTS = *.o

PROG1 = program3

PROGS = $(PROG1) $(PROG2) $(PROG3)

all: $(PROGS)

$(PROG1): $(SOURCES)
	$(CC) $(FLAGS) $(SOURCES) $(OBJECTS) -o $(PROG1)

clean:
	rm -f $(PROGS) $(OBJECTS) *~ \#*

run:
	./$(PROG1)

valgrind:
	valgrind ./$(PROG1)

gdb:
	gdb ./$(PROG1)

