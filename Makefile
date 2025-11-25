CC = gcc

# termui library location (in jcaldwell-labs monorepo)
TERMUI_DIR ?= ../jcaldwell-labs/libs/termui

CFLAGS = -Wall -Wextra -Werror -Iinclude -I$(TERMUI_DIR)/include -std=gnu99
# Link statically against termui to avoid runtime library path issues
LDFLAGS = $(TERMUI_DIR)/libtermui.a -lncurses -lm
TARGET = terminal-stars

# Check if SDL2 is available
SDL2_EXISTS := $(shell command -v sdl2-config 2> /dev/null)
ifdef SDL2_EXISTS
    CFLAGS += $(shell sdl2-config --cflags) -DHAVE_SDL2
    LDFLAGS += $(shell sdl2-config --libs)
    $(info SDL2 found - enabling gamepad support)
else
    $(info SDL2 not found - gamepad support disabled)
endif
SRCDIR = src
INCDIR = include
OBJDIR = obj
TESTDIR = tests
TESTBINDIR = $(TESTDIR)/bin

# Main application sources
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

# Library sources (everything except main.c)
LIB_SOURCES = $(filter-out $(SRCDIR)/main.c,$(SOURCES))
LIB_OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(LIB_SOURCES))

# Test sources and executables
TEST_SOURCES = $(wildcard $(TESTDIR)/test_*.c)
TEST_BINS = $(patsubst $(TESTDIR)/test_%.c,$(TESTBINDIR)/test_%,$(TEST_SOURCES))

.PHONY: all clean run test help install uninstall benchmark validate

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

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

clean:
	rm -rf $(OBJDIR) $(TARGET) $(TESTBINDIR) test_joystick

run: $(TARGET)
	./$(TARGET)

# Joystick test utility
test_joystick: test_joystick.c
	$(CC) -o test_joystick test_joystick.c -lSDL2

test-joystick: test_joystick
	@./test_joystick

# Benchmark target
$(TESTBINDIR)/benchmark: $(TESTDIR)/benchmark.c $(LIB_OBJECTS) | $(TESTBINDIR)
	$(CC) $(CFLAGS) $< $(LIB_OBJECTS) -o $@ $(LDFLAGS)

benchmark: $(TESTBINDIR)/benchmark
	@echo "Running performance benchmarks..."
	@./$(TESTBINDIR)/benchmark

# Terminal validation
validate:
	@./validate_terminal.sh

# Install target
PREFIX ?= /usr/local
BINDIR = $(PREFIX)/bin
DOCDIR = $(PREFIX)/share/doc/terminal-stars

install: $(TARGET)
	@echo "Installing terminal-stars to $(PREFIX)..."
	install -d $(BINDIR)
	install -m 755 $(TARGET) $(BINDIR)
	install -d $(DOCDIR)
	install -m 644 README.md $(DOCDIR)
	install -m 644 FEATURES.md $(DOCDIR)
	install -m 644 MODE-GUIDE.md $(DOCDIR)
	install -m 644 LICENSE $(DOCDIR)
	@echo "Installation complete!"
	@echo "Run 'terminal-stars' to start the application"

uninstall:
	@echo "Uninstalling terminal-stars..."
	rm -f $(BINDIR)/$(TARGET)
	rm -rf $(DOCDIR)
	@echo "Uninstall complete"

help:
	@echo "Terminal Stars - Build System"
	@echo "=============================="
	@echo ""
	@echo "Available targets:"
	@echo "  make              - Build the application"
	@echo "  make run          - Build and run the application"
	@echo "  make test         - Run all tests"
	@echo "  make benchmark    - Run performance benchmarks"
	@echo "  make test-joystick - Test SDL2 joystick detection"
	@echo "  make validate     - Validate terminal compatibility"
	@echo "  make install      - Install to system (default: /usr/local)"
	@echo "  make uninstall    - Uninstall from system"
	@echo "  make clean        - Clean build artifacts"
	@echo "  make help         - Show this help message"
	@echo ""
	@echo "Installation options:"
	@echo "  make install PREFIX=/custom/path  - Install to custom location"
	@echo ""
	@echo "Requirements:"
	@echo "  - GCC compiler (required)"
	@echo "  - ncurses library - libncurses-dev (required)"
	@echo "  - SDL2 library - libsdl2-dev (optional, for joystick support)"
