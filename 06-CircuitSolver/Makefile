# Project Settings
SRCDIR := src
INCDIR := include
LIBDIR := lib
BLDDIR := build
DBGDIR := debug

BINDIR := bin
NAME := tranny
EXEC := $(BINDIR)/$(NAME).app

# Project files
SRCS = $(wildcard $(SRCDIR)/*.cpp)
DEPS = $(patsubst $(SRCDIR)/%.cpp, $(BLDDIR)/%.d, $(SRCS))
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(BLDDIR)/%.o, $(SRCS))

# Compiler Settings
CC := g++
CXXFLAGS := -c -MMD -MP\
	-std=c++20 -Wall -Wextra -Wshadow -Wnon-virtual-dtor \
	-Wold-style-cast -Wcast-align -Woverloaded-virtual \
	-Weffc++ -pedantic -Werror
LDFLAGS :=
LDLIBS :=

# Debug options
DEBUG ?= 0
ifeq ($(DEBUG), 1)
	CXXFLAGS := $(CXXFLAGS) --ggdb -fsanitize=address -O0
	BLDDIR := $(DBGDIR)
	EXEC := $(DBGDIR)/$(NAME).dbg
endif

.PHONY: all clean run setflag distclean
.DEFAULT_GOAL := $(EXEC)

all: $(EXEC) run

$(EXEC): $(BLDDIR) $(BINDIR) $(OBJS)
	$(CC) -o $@ $(LDFLAGS) $(OBJS) $(LDLIBS)

$(BLDDIR) $(BINDIR):
	[ -d $@ ] || mkdir -p $@

run: $(EXEC)
	@echo -e "\n====== Running ======"
	@$(EXEC)

$(BLDDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) -o $@ $(CXXFLAGS) $< -I $(INCDIR)

-include $(DEPS)
$(BLDDIR)/%.d: $(SRCDIR)/%.cpp

clean:
	@echo "Removing compilation intermediates"
	-rm -rf $(BLDDIR)/ $(DBGDIR)/

distclean: clean
	@echo "Removing executables"
	-rm -rf $(BINDIR)/
