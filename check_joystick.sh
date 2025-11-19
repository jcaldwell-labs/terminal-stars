#!/bin/bash
# Joystick detection and diagnostics for terminal-stars

echo "=== Joystick Detection ==="
echo

# Check USB devices
echo "1. USB Devices:"
lsusb | grep -i -E "(joy|game|xbox|playstation|controller|logitech)" || echo "   No gaming devices found via lsusb"
echo

# Check input devices
echo "2. Input Devices:"
if ls /dev/input/js* 2>/dev/null; then
    for js in /dev/input/js*; do
        echo "   Found: $js"
        if [ -r "$js" ]; then
            echo "     ✓ Readable"
        else
            echo "     ✗ Not readable (may need permissions)"
        fi
    done
else
    echo "   No /dev/input/js* devices found (this is normal in WSL2)"
    echo "   SDL2 will use evdev (/dev/input/event*) instead"
fi
echo

# Check event devices
echo "3. Event Devices:"
if [ -d /dev/input/by-id ]; then
    ls -la /dev/input/by-id/ 2>/dev/null | grep -i -E "(joy|game|xbox|playstation)" || echo "   No joystick event devices found"
else
    echo "   /dev/input/by-id not found"
fi
echo

# Check SDL2 availability
echo "4. SDL2 Library:"
if ldconfig -p | grep -q libSDL2; then
    echo "   ✓ SDL2 is installed"
    ldconfig -p | grep libSDL2 | head -1
else
    echo "   ✗ SDL2 not found"
fi
echo

# Check if terminal-stars binary has SDL2 support
echo "5. Terminal-Stars Build:"
if [ -f "./terminal-stars" ]; then
    if ldd ./terminal-stars 2>/dev/null | grep -q SDL2; then
        echo "   ✓ terminal-stars is compiled with SDL2 support"
    else
        echo "   ✗ terminal-stars does NOT have SDL2 support"
        echo "     Run 'make clean && make' to rebuild"
    fi
else
    echo "   terminal-stars binary not found (run 'make' first)"
fi
echo

echo "6. SDL2 Joystick Detection:"
if [ -f "./test_joystick" ]; then
    ./test_joystick | grep -E "(Found|Joystick|Name:|Axes:|Buttons:)" | head -10
else
    echo "   Run 'gcc -o test_joystick test_joystick.c -lSDL2' to build test"
fi
echo

echo "=== Setup Instructions ==="
echo
echo "If no joysticks detected, you need USB passthrough from Windows:"
echo
echo "Windows PowerShell (as Administrator):"
echo "  1. Install: winget install --interactive --exact dorssel.usbipd-win"
echo "  2. List USB: usbipd list"
echo "  3. Bind device: usbipd bind --busid X-Y"
echo "  4. Attach to WSL: usbipd attach --wsl --busid X-Y"
echo
echo "Then run this script again to verify."
echo
