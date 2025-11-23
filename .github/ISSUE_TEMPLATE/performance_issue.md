---
name: Performance Issue
about: Report performance problems or frame rate issues
title: '[PERFORMANCE] '
labels: performance
assignees: ''
---

## Performance Issue Description

Describe the performance problem you're experiencing.

## Symptoms

- [ ] Low frame rate / stuttering
- [ ] High CPU usage
- [ ] High memory usage
- [ ] Slow startup
- [ ] Input lag
- [ ] Other: ___________

## Measurements

If possible, provide performance metrics:

**Frame Rate**: [e.g., 30fps instead of expected 60fps]
**CPU Usage**: [e.g., 80% on one core]
**Memory Usage**: [e.g., 100MB RSS]

## Environment

- **OS**: [e.g., Ubuntu 22.04]
- **CPU**: [e.g., Intel i7-9700K]
- **Terminal**: [e.g., gnome-terminal]
- **Terminal size**: [e.g., 200x60 - large terminal]
- **Game mode**: [e.g., Skeet Shooting with many objects]
- **Settings**: [e.g., starfield effect: WARP, speed: maximum]

## Profiling Data

If you've done any profiling, include the results here:

```
Paste profiling output or performance data here
```

## Expected Performance

What performance level do you expect?
Ex: 60fps at 80x24 terminal size

## Additional Context

- Does the issue occur with all game modes or specific ones?
- Does it improve with smaller terminal size?
- Any other observations about when performance degrades?

## Checklist

- [ ] I have tested with the default terminal size (80x24)
- [ ] I have tested with different starfield effects
- [ ] I have checked if SDL2 is causing the issue (test without joystick)
- [ ] I have checked system resources during the issue
