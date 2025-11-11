# Folder Structure
SRCDIR := src
INCDIR := include
LIBDIR := lib

BLDDIR := build
DBGDIR := debug
TSTDIR := tests

BINDIR := bin
NAME := tranny
EXEC := $(BINDIR)/$(NAME).app

# Project files
SRCS = $(wildcard $(SRCDIR)/*.cpp)
DEPS = $(patsubst $(SRCDIR)/%.cpp, $(BLDDIR)/%.d, $(SRCS))
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(BLDDIR)/%.o, $(SRCS))

# Test settings
TESTSRCS := $(wildcard $(TSTDIR)/*_test.cpp)
TESTDEPS := $(patsubst $(TSTDIR)/%.cpp, $(BLDDIR)/%.d, $(TESTSRCS))
TESTOBJS := $(patsubst $(TSTDIR)/%.cpp, $(BLDDIR)/%.o, $(TESTSRCS))
TESTEXEC := $(patsubst $(TSTDIR)/%.cpp, $(BINDIR)/%.app, $(TESTSRCS))


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
	CXXFLAGS := $(CXXFLAGS) -ggdb -O0
	BLDDIR := $(DBGDIR)
	EXEC := $(BINDIR)/$(NAME).dbg
endif

.PHONY: all clean run setflag distclean tests
.DEFAULT_GOAL := run

all: $(EXEC) run

$(EXEC): $(BLDDIR) $(BINDIR) $(OBJS)
	$(CC) -o $@ $(LDFLAGS) $(OBJS) $(LDLIBS)

$(BLDDIR) $(BINDIR):
	[ -d $@ ] || mkdir -p $@

run: $(EXEC)
	@echo -e "====== Running ======"
	@$(EXEC)

$(BLDDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) -o $@ $(CXXFLAGS) $< -I $(INCDIR)

-include $(DEPS)
$(BLDDIR)/%.d: $(SRCDIR)/%.cpp

# === Tests ===
tests: $(TESTEXEC)
	@echo -e "====== Running Tests ======"
	@for t in $(TESTEXEC); do \
		echo -e "\nRunning $$t"; \
		$$t || exit 1; \
	done
	@echo;

$(BINDIR)/%.app: $(BLDDIR)/%.o | $(BINDIR)
	$(CC) -o $@ $(LDFLAGS) $< $(LDLIBS)

$(BLDDIR)/%.o: $(TSTDIR)/%.cpp | $(BLDDIR)
	$(CC) -o $@ $(CXXFLAGS) $< -I $(INCDIR)


-include $(TESTDEPS)
$(BLDDIR)/%.d: $(TSTDIR)/%.cpp


clean:
	@echo "Removing compilation intermediates"
	-rm -rf $(BLDDIR)/ $(DBGDIR)/

distclean: clean
	@echo "Removing executables"
	-rm -rf $(BINDIR)/
