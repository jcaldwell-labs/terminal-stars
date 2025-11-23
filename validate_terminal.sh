#!/bin/bash
#
# Terminal Validation Script for Terminal Stars
#
# Validates terminal compatibility, capabilities, and performance
# Tests various TERM values, color support, and terminal features

# Note: Not using 'set -e' to allow collecting all issues before exiting

# Color codes for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Print colored output
print_header() {
    echo -e "${BLUE}========================================${NC}"
    echo -e "${BLUE}$1${NC}"
    echo -e "${BLUE}========================================${NC}"
}

print_success() {
    echo -e "${GREEN}✓${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}⚠${NC} $1"
}

print_error() {
    echo -e "${RED}✗${NC} $1"
}

print_info() {
    echo -e "${BLUE}ℹ${NC} $1"
}

# Get terminal dimensions
get_terminal_size() {
    if command -v tput &> /dev/null; then
        COLS=$(tput cols)
        ROWS=$(tput lines)
    else
        # Fallback to stty
        ROWS=$(stty size | cut -d' ' -f1)
        COLS=$(stty size | cut -d' ' -f2)
    fi
    echo "${COLS}x${ROWS}"
}

# Check color support
check_color_support() {
    if command -v tput &> /dev/null; then
        COLORS=$(tput colors 2>/dev/null || echo "0")
        echo "$COLORS"
    else
        echo "unknown"
    fi
}

# Main validation
main() {
    print_header "Terminal Stars - Terminal Validation"
    echo ""

    # 1. Basic terminal information
    print_header "Terminal Environment"
    echo ""
    print_info "TERM: ${TERM:-not set}"
    print_info "Terminal size: $(get_terminal_size)"

    COLOR_COUNT=$(check_color_support)
    print_info "Color support: ${COLOR_COUNT} colors"

    if [ -n "$COLORTERM" ]; then
        print_info "COLORTERM: $COLORTERM"
    fi
    echo ""

    # 2. Check required capabilities
    print_header "Required Capabilities"
    echo ""

    # Check ncurses
    if command -v tput &> /dev/null; then
        print_success "tput available (ncurses present)"
    else
        print_error "tput not found (ncurses may not be installed)"
    fi

    # Check color support
    if [ "$COLOR_COUNT" -ge 8 ]; then
        print_success "Color support adequate ($COLOR_COUNT colors)"
    elif [ "$COLOR_COUNT" = "unknown" ]; then
        print_warning "Color support unknown - may still work"
    else
        print_error "Insufficient color support ($COLOR_COUNT colors, need at least 8)"
    fi

    # Check terminal dimensions
    SIZE=$(get_terminal_size)
    COLS=${SIZE%x*}
    ROWS=${SIZE#*x}

    if [ "$COLS" -ge 80 ] && [ "$ROWS" -ge 24 ]; then
        print_success "Terminal size adequate (${SIZE})"
    else
        print_warning "Terminal size small (${SIZE}), recommend at least 80x24"
    fi

    echo ""

    # 3. Test various TERM values
    print_header "Testing Common TERM Values"
    echo ""

    TERM_VALUES=("xterm" "xterm-256color" "screen" "screen-256color" "tmux" "tmux-256color")

    for term_val in "${TERM_VALUES[@]}"; do
        # Check if terminfo exists for this TERM
        if infocmp "$term_val" &> /dev/null; then
            print_success "$term_val: terminfo available"
        else
            print_warning "$term_val: terminfo not found"
        fi
    done

    echo ""

    # 4. Check dependencies
    print_header "Build Dependencies"
    echo ""

    # Check GCC
    if command -v gcc &> /dev/null; then
        GCC_VERSION=$(gcc --version | head -n1)
        print_success "GCC: $GCC_VERSION"
    else
        print_error "GCC not found"
    fi

    # Check ncurses development files
    if [ -f /usr/include/ncurses.h ] || [ -f /usr/local/include/ncurses.h ]; then
        print_success "ncurses development headers found"
    else
        print_warning "ncurses development headers not found (may need libncurses-dev)"
    fi

    # Check SDL2 (optional)
    if command -v sdl2-config &> /dev/null; then
        SDL2_VERSION=$(sdl2-config --version)
        print_success "SDL2: $SDL2_VERSION (joystick support available)"
    else
        print_info "SDL2 not found (joystick support will be disabled)"
    fi

    echo ""

    # 5. Terminal feature tests
    print_header "Terminal Feature Tests"
    echo ""

    # Check cursor visibility control
    if tput civis &> /dev/null && tput cnorm &> /dev/null; then
        print_success "Cursor visibility control available"
    else
        print_warning "Cursor visibility control may not work"
    fi

    # Check clear screen
    if tput clear &> /dev/null; then
        print_success "Clear screen capability available"
    else
        print_warning "Clear screen capability not detected"
    fi

    # Check alternate screen buffer
    if tput smcup &> /dev/null && tput rmcup &> /dev/null; then
        print_success "Alternate screen buffer available"
    else
        print_warning "Alternate screen buffer not available"
    fi

    echo ""

    # 6. Performance recommendations
    print_header "Performance Recommendations"
    echo ""

    if [ "$COLS" -le 120 ] && [ "$ROWS" -le 40 ]; then
        print_success "Terminal size good for performance (${SIZE})"
    else
        print_warning "Large terminal (${SIZE}) may impact performance"
        print_info "Consider running at 120x40 or smaller for best FPS"
    fi

    if [ "$COLOR_COUNT" = "256" ] || [ "$COLOR_COUNT" -gt 256 ]; then
        print_info "256+ colors supported - visual quality will be excellent"
    elif [ "$COLOR_COUNT" = "8" ]; then
        print_info "8 colors supported - basic visual quality"
    fi

    echo ""

    # 7. Build test
    print_header "Build Test"
    echo ""

    if [ -f "Makefile" ]; then
        print_info "Attempting to build terminal-stars..."
        if make clean && make; then
            if [ -x "./terminal-stars" ]; then
                print_success "Build successful!"
                print_success "Executable created: ./terminal-stars"
            else
                print_error "Build completed but executable not found or not executable"
            fi
        else
            print_error "Build failed - check compiler errors above"
        fi
    else
        print_warning "Makefile not found in current directory"
    fi

    echo ""

    # 8. Final summary
    print_header "Validation Summary"
    echo ""

    ISSUES=0

    if [ "$COLOR_COUNT" -lt 8 ] && [ "$COLOR_COUNT" != "unknown" ]; then
        ((ISSUES++))
    fi

    if [ "$COLS" -lt 80 ] || [ "$ROWS" -lt 24 ]; then
        ((ISSUES++))
    fi

    if ! command -v gcc &> /dev/null; then
        ((ISSUES++))
    fi

    if [ $ISSUES -eq 0 ]; then
        print_success "All checks passed! Terminal is compatible with Terminal Stars."
        echo ""
        print_info "To run the application:"
        print_info "  make run"
        echo ""
        print_info "To run tests:"
        print_info "  make test"
        echo ""
        print_info "For best performance, use a terminal size of 80x24 to 120x40"
    else
        print_warning "Found $ISSUES potential issue(s)"
        print_info "The application may still work, but some features might be limited"
    fi

    echo ""
}

# Run validation
main "$@"
