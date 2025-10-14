# Flags
CXX := g++
CXXFLAGS := -Wall -Werror -pedantic -O1
LIBS := -lsfml-graphics -lsfml-window -lsfml-system

# Targets
all: tranny.app

run: tranny.app
	./tranny.app < input.txt

tranny.app: main.o
	$(CXX) -o $@ $^ $(LIBS)

main.o: main.cpp
	$(CXX) -c $(CXXFLAGS) $<

# Clean up object files and binary
clean:
	rm -rf *.o tranny.app
