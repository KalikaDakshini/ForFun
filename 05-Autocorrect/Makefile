# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++20 -Wall -O2 -g -Werror -pedantic
LDFLAGS := -lfmt

# Target executable
TARGET := tranny.app

# Source and object files
SRCS := main.cpp bk_tree.cpp ac.cpp
OBJS := $(SRCS:.cpp=.o)

# Headers
HDRS := bk_tree.hpp ac.hpp

# Default target
all: $(TARGET)

# Link object files to produce executable
$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Compile source files into object files
%.o: %.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
