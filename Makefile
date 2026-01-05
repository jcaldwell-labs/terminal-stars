# Terminal Stars - Educational Starfield Visualizer
# ================================================
# A clean, simple build for the starfield visualizer demo.
#
# Requirements:
#   - GCC compiler (required)
#   - ncurses library - libncurses-dev (required)

CC = gcc

# Compiler flags
#   -Wall -Wextra: Enable warnings for better code quality
#   -Werror: Treat warnings as errors
#   -std=gnu99: Use GNU C99 for clock_gettime and other POSIX features
CFLAGS = -Wall -Wextra -Werror -Iinclude -std=gnu99

# Linker flags
#   - ncurses for terminal handling
#   - libm for math functions (sin, cos, sqrt, atan2)
LDFLAGS = -lncurses -lm

TARGET = terminal-stars

# Directory structure
SRCDIR = src
INCDIR = include
OBJDIR = obj
TESTDIR = tests
TESTBINDIR = $(TESTDIR)/bin

# Source files (5 files for the educational version)
SOURCES = $(SRCDIR)/main.c \
          $(SRCDIR)/starfield.c \
          $(SRCDIR)/effects.c \
          $(SRCDIR)/render.c \
          $(SRCDIR)/terminal.c

OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

# Library objects (everything except main.c) for tests
LIB_SOURCES = $(filter-out $(SRCDIR)/main.c,$(SOURCES))
LIB_OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(LIB_SOURCES))

# Test sources and executables
TEST_SOURCES = $(wildcard $(TESTDIR)/test_*.c)
TEST_BINS = $(patsubst $(TESTDIR)/test_%.c,$(TESTBINDIR)/test_%,$(TEST_SOURCES))

.PHONY: all clean run test help install uninstall benchmark

# Default target
all: $(TARGET)

# Link the final executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Compile source files to object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create object directory
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Test compilation
$(TESTBINDIR)/test_%: $(TESTDIR)/test_%.c $(LIB_OBJECTS) | $(TESTBINDIR)
	$(CC) $(CFLAGS) $< $(LIB_OBJECTS) -o $@ $(LDFLAGS)

$(TESTBINDIR):
	mkdir -p $(TESTBINDIR)

# Run all tests
test: $(TEST_BINS)
	@echo "Running all tests..."
	@for test in $(TEST_BINS); do \
		echo ""; \
		echo "=========================================="; \
		echo "Running $$test..."; \
		echo "=========================================="; \
		$$test || exit 1; \
	done
	@echo ""
	@echo "=========================================="
	@echo "All tests passed!"
	@echo "=========================================="

# Clean build artifacts
clean:
	rm -rf $(OBJDIR) $(TARGET) $(TESTBINDIR)

# Build and run
run: $(TARGET)
	./$(TARGET)

# Benchmark target
$(TESTBINDIR)/benchmark: $(TESTDIR)/benchmark.c $(LIB_OBJECTS) | $(TESTBINDIR)
	$(CC) $(CFLAGS) $< $(LIB_OBJECTS) -o $@ $(LDFLAGS)

benchmark: $(TESTBINDIR)/benchmark
	@echo "Running performance benchmarks..."
	@./$(TESTBINDIR)/benchmark

# Installation
PREFIX ?= /usr/local
BINDIR = $(PREFIX)/bin
DOCDIR = $(PREFIX)/share/doc/terminal-stars

install: $(TARGET)
	@echo "Installing terminal-stars to $(PREFIX)..."
	install -d $(BINDIR)
	install -m 755 $(TARGET) $(BINDIR)
	install -d $(DOCDIR)
	install -m 644 README.md $(DOCDIR)
	install -m 644 LICENSE $(DOCDIR)
	@echo "Installation complete!"
	@echo "Run 'terminal-stars --help' for usage"

uninstall:
	@echo "Uninstalling terminal-stars..."
	rm -f $(BINDIR)/$(TARGET)
	rm -rf $(DOCDIR)
	@echo "Uninstall complete"

# Help
help:
	@echo "Terminal Stars - Educational Starfield Visualizer"
	@echo "=================================================="
	@echo ""
	@echo "Build targets:"
	@echo "  make              Build the application"
	@echo "  make run          Build and run"
	@echo "  make test         Run unit tests"
	@echo "  make benchmark    Run performance benchmarks"
	@echo "  make clean        Remove build artifacts"
	@echo ""
	@echo "Installation:"
	@echo "  make install      Install to /usr/local"
	@echo "  make install PREFIX=/path  Custom install location"
	@echo "  make uninstall    Remove installed files"
	@echo ""
	@echo "Requirements:"
	@echo "  - GCC compiler"
	@echo "  - ncurses library (libncurses-dev)"
	@echo ""
	@echo "For the full terminal shader program, see:"
	@echo "  https://github.com/jcaldwell-labs/atari-style"
