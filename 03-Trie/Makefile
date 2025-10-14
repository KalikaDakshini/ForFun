CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Werror -pedantic -O2
LDFLAGS := 

SRC := main.cpp trie.cpp
OBJ := $(SRC:.cpp=.o)
EXEC := tranny.app

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp trie.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)

.PHONY: all clean
