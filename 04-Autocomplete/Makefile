# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++20 -Wall -O2 -g

# Target executable
TARGET := tranny.app

# Source and object files
SRCS := main.cpp ac.cpp trie.cpp
OBJS := $(SRCS:.cpp=.o)

# Headers
HDRS := ac.hpp trie.hpp

# Default target
all: $(TARGET)

# Link object files to produce executable
$(TARGET): $(OBJS)
	$(CXX) -o $@ $^

# Compile source files into object files
%.o: %.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
