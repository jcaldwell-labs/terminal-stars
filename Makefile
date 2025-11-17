CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iinclude -std=gnu99
LDFLAGS = -lncurses -lm
TARGET = terminal-stars
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

.PHONY: all clean run test help

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
	rm -rf $(OBJDIR) $(TARGET) $(TESTBINDIR)

run: $(TARGET)
	./$(TARGET)

help:
	@echo "Terminal Stars - Build System"
	@echo "=============================="
	@echo ""
	@echo "Available targets:"
	@echo "  make          - Build the application"
	@echo "  make run      - Build and run the application"
	@echo "  make test     - Run all tests"
	@echo "  make clean    - Clean build artifacts"
	@echo "  make help     - Show this help message"
	@echo ""
	@echo "Requirements:"
	@echo "  - GCC compiler"
	@echo "  - ncurses library (libncurses-dev)"
