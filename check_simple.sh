#!/bin/bash
echo "=== USB Devices (lsusb) ==="
lsusb 2>/dev/null || echo "lsusb not available"

echo ""
echo "=== /dev/input Devices ==="
ls -la /dev/input/ 2>/dev/null || echo "/dev/input not found"

echo ""
echo "=== Input Devices Info ==="
if [ -f /proc/bus/input/devices ]; then
    cat /proc/bus/input/devices | head -20
else
    echo "/proc/bus/input/devices not found"
fi
