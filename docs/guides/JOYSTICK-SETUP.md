# Joystick Setup for WSL2

This guide explains how to use USB joysticks/gamepads with Terminal Stars in WSL2.

## Why USB Passthrough is Needed

WSL2 runs in a lightweight VM and doesn't have direct access to USB devices. The `joydev` kernel module isn't available in WSL2's kernel. Instead, we use **USB/IP** to pass USB devices from Windows to WSL2.

## Quick Setup (First Time Only)

### 1. Install USBIPD-WIN on Windows

Open **PowerShell as Administrator** and run:

```powershell
winget install --interactive --exact dorssel.usbipd-win
```

### 2. Verify WSL2 Setup

Your WSL2 environment already has:
- ✓ SDL2 library installed
- ✓ usbip client installed
- ✓ terminal-stars compiled with SDL2 support

## Attaching Your Joystick

### Option A: Use the Helper Script (Recommended)

1. Copy `attach_joystick.ps1` to your Windows user directory
2. Right-click PowerShell → **Run as Administrator**
3. Navigate to the directory and run:
   ```powershell
   .\attach_joystick.ps1
   ```
4. Follow the prompts

### Option B: Manual Steps

**In Windows PowerShell (as Administrator):**

```powershell
# List all USB devices
usbipd list

# Find your joystick (look for Xbox, PlayStation, Logitech, etc.)
# Note the BUSID (e.g., 1-4)

# Bind the device (only needed once per device)
usbipd bind --busid 1-4

# Attach to WSL2 (needed every time you reconnect the joystick)
usbipd attach --wsl --busid 1-4
```

### Option C: Batch File

Run `attach_joystick.bat` as Administrator (less preferred than PowerShell).

## Verify Joystick Detection

In WSL2, run:

```bash
./check_joystick.sh
```

You should see:
- Your joystick listed in USB devices
- `/dev/input/js0` device present
- SDL2 library detected
- terminal-stars compiled with SDL2 support

## Running the Game

```bash
make run
```

The game will auto-detect connected joysticks and display how many were found.

## Troubleshooting

### Joystick not detected in WSL2

1. **Check Windows PowerShell**:
   ```powershell
   usbipd list
   ```
   Verify the device shows as "Attached - WSL"

2. **Check WSL2**:
   ```bash
   lsusb | grep -i "joy\|game\|xbox\|playstation"
   ls /dev/input/js*
   ```

3. **Re-attach the device**:
   ```powershell
   usbipd detach --busid 1-4
   usbipd attach --wsl --busid 1-4
   ```

### "Access denied" in Windows

- Make sure PowerShell is running **as Administrator**

### Joystick disappears after reconnecting

You need to run `usbipd attach` again:
```powershell
usbipd attach --wsl --busid 1-4
```

### Game doesn't detect joystick

1. Run diagnostics:
   ```bash
   ./check_joystick.sh
   ```

2. Rebuild the game:
   ```bash
   make clean && make
   ```

3. Verify SDL2 is linked:
   ```bash
   ldd ./terminal-stars | grep SDL2
   ```

## Supported Controllers

The game supports any SDL2-compatible controller:
- ✓ Xbox One/Series controllers
- ✓ Xbox 360 controllers
- ✓ PlayStation 4/5 DualShock/DualSense
- ✓ Nintendo Switch Pro Controller
- ✓ Logitech gamepads
- ✓ Generic USB joysticks

## Performance Notes

- USB passthrough adds minimal latency (~1-2ms)
- 60fps target is maintained
- Haptic feedback (rumble) is supported if your controller has it

## Fallback: Keyboard Controls

If you can't set up USB passthrough, the game works perfectly with keyboard:

**Player 1:**
- W/S: Pitch up/down
- A/D: Yaw left/right
- Q/E: Roll left/right
- Space: Thrust
- F: Fire weapon

See README.md for complete keyboard controls.

## References

- [USBIPD-WIN Documentation](https://github.com/dorssel/usbipd-win)
- [WSL2 USB Support](https://learn.microsoft.com/en-us/windows/wsl/connect-usb)
- [SDL2 Documentation](https://wiki.libsdl.org/SDL2/FrontPage)
