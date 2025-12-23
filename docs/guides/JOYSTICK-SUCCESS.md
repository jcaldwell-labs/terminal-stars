# ✅ Joystick Setup Complete

## Your Configuration

**Device:** Trooper V2
**Connection:** USB passthrough via USBIPD-WIN (BUSID 2-4)
**Detection:** ✓ Working via SDL2
**Capabilities:**
- 3 analog axes
- 8 buttons
- No haptic feedback

## Verification Results

```
SDL2 Joystick Test: ✓ PASSED
Found 1 joystick(s)

Joystick 0:
  Name: Trooper V2
  Axes: 3
  Buttons: 8
```

## Quick Reference

### Reconnecting After Reboot

**In Windows PowerShell (as Administrator):**
```powershell
usbipd attach --wsl --busid 2-4
```

Or use the helper script:
```powershell
.\attach_joystick.ps1
```

### Verify Detection

**In WSL2:**
```bash
./check_joystick.sh
```

### Run the Game

```bash
make run
```

The game will automatically detect and use your joystick!

## Joystick Controls

Based on your 3 axes and 8 buttons, the typical mapping is:

- **Axis 0 (Left Stick X):** Yaw left/right
- **Axis 1 (Left Stick Y):** Pitch up/down
- **Axis 2 (Twist/Throttle):** Roll or thrust (depends on joystick)
- **Buttons:** Fire, camera toggle, thrust, etc.

Test in the game to see exact mappings for your Trooper V2!

## Troubleshooting

### Joystick stops working

Re-attach the USB device:
```powershell
# In Windows PowerShell (Admin)
usbipd detach --busid 2-4
usbipd attach --wsl --busid 2-4
```

### Permissions error

Make sure you're in the `input` group (you already are):
```bash
groups | grep input
```

### Game doesn't detect joystick

Rebuild the game:
```bash
make clean && make
```

## Files Created

- `check_joystick.sh` - Diagnostic tool (run anytime)
- `test_joystick.c` - SDL2 test program
- `test_joystick` - Compiled test binary
- `attach_joystick.ps1` - Windows helper script
- `JOYSTICK-SETUP.md` - Complete setup guide

## Next Steps

You're all set! Just run:

```bash
make run
```

Enjoy your space combat simulator with full joystick control! 🚀🎮
