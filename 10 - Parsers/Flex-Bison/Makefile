CC := g++
CFLAGS := -Wall -Werror -Wextra --std=c++20
FLEX := flex++
BISON := bison

EXEC := tranny.app

.DEFAULT_GOAL := $(EXEC)
.PHONY: all clean

all: $(EXEC)
	./$(EXEC)

$(EXEC): scanner.o parser.o driver.o main.o
	$(CC) $^ -o $@ 

%.o: %.cpp
	$(CC) -c $(CFLAGS) -o $@ $<

%.o: %.cc
	$(CC) -c $(CFLAGS) -o $@ $<

# Scanner rules
scanner.cc: scanner.l scanner.hpp parser.hh
	$(FLEX) -o $@ $<

# Parser rules
parser.hh parser.cc: parser.y
	$(BISON) -o parser.cc $<


clean:
	-rm -rf *.cc *.hh *.o