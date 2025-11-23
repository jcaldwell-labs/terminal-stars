# Changelog

All notable changes to Terminal Stars will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- CONTRIBUTING.md with comprehensive contribution guidelines
- CHANGELOG.md for tracking project history
- GitHub Actions CI/CD workflow for automated testing
- Issue and pull request templates
- Basic unit test suite
- Performance benchmarking tools
- Terminal validation testing script
- Install target in Makefile

### Changed
- Enhanced documentation structure

## [1.0.0] - 2025-11-23

### Added
- Comprehensive documentation (#6)
  - FEATURES.md with detailed feature descriptions
  - MODE-GUIDE.md with complete gameplay guides
  - Enhanced README.md with setup instructions
- Horizon rendering for skeet shooting mode (#5)
- Skeet shooting game mode with:
  - Clay pigeon ballistic physics
  - Three launcher positions
  - Progressive difficulty system
  - Target circle reticle
- WSL2 gamepad hang fix for SDL2 support
- Five complete game modes:
  - Single Player vs AI
  - Dual Player competitive
  - Co-op formation flying
  - Training simulator
  - Skeet shooting
- USB joystick/gamepad support (#4)
  - Up to 2 simultaneous controllers
  - 8-direction analog stick control with dead zones
  - Button mapping for all flight controls
  - Haptic feedback on weapon fire
  - Support for Xbox, PlayStation, and generic USB controllers
- Training simulator with:
  - Four target patterns (static, moving, orbiting, random)
  - Score tracking and accuracy statistics
  - Real-time performance feedback
- Advanced HUD systems:
  - 6-DOF attitude indicator
  - Enemy direction indicator
  - Radar overlay
  - Weapons status and cooldown displays
  - Speed and health readouts
- Weapons system (#3):
  - Dual-missile firing system (left/right offset)
  - Guided missile trajectories
  - Collision detection against targets
  - Expanding explosion animations
  - Visual feedback effects
- Single player combat:
  - AI opponent with orbital behavior
  - Ship-to-ship combat mechanics
  - Health tracking system
- Two-player modes:
  - Manual controls for Player 2
  - AI toggle functionality
  - Competitive and cooperative gameplay
- Second ship with 3D rendering:
  - AI orbital behavior
  - Formation flying patterns
  - Independent physics simulation
- 6-DOF camera system:
  - Cockpit view
  - Chase camera
  - Free camera mode
  - Smooth camera transitions
- Torus roller coaster special effect
- Six starfield effects:
  - Linear (forward motion)
  - Spiral (rotation pattern)
  - Warp (hyperspeed with stretching)
  - Tunnel (cylindrical effect)
  - Explode (outward expansion)
  - Wave (undulating motion)

### Changed
- Disabled background starfield during skeet mode for clarity
- Improved rendering performance with frame buffering
- Enhanced physics calculations with delta-time normalization

### Fixed
- SDL2 gamepad hanging issue in WSL2 environments
- Lowercase 'q' key conflict with quit functionality
- Missile visibility issues in certain viewing angles
- All issues from Copilot AI PR review feedback

## [0.1.0] - Initial Release

### Added
- Basic starfield visualization
- Terminal rendering with ncurses
- Frame buffering for smooth 60fps animation
- 3D perspective projection
- Star management and rendering
- Terminal resize handling
- Color support (8 colors)
- Basic keyboard input
- Main game loop with delta-time physics

---

## Version History Summary

- **1.0.0**: Full-featured 3D space combat simulator with multiple game modes
- **0.1.0**: Initial starfield visualization prototype

[Unreleased]: https://github.com/jcaldwell-labs/terminal-stars/compare/v1.0.0...HEAD
[1.0.0]: https://github.com/jcaldwell-labs/terminal-stars/compare/v0.1.0...v1.0.0
[0.1.0]: https://github.com/jcaldwell-labs/terminal-stars/releases/tag/v0.1.0
