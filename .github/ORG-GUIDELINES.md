# Organization Guidelines for Terminal Stars

This document outlines the repository structure and documentation standards for the Terminal Stars project, following jcaldwell-labs organization conventions.

## Repository Structure

### Root Directory (Essential Files Only)

```
terminal-stars/
├── README.md           # Project overview, quick start, feature summary
├── LICENSE             # MIT License
├── CONTRIBUTING.md     # Contribution guidelines
├── CHANGELOG.md        # Version history
├── CLAUDE.md           # AI assistant development guidelines
├── llms.txt            # AI discoverability file
├── Makefile            # Build configuration
├── src/                # Source code
├── include/            # Header files
├── tests/              # Test suite
├── docs/               # User documentation
└── .github/            # GitHub configuration
```

### Documentation Structure

```
docs/
├── README.md           # Documentation index
├── FEATURES.md         # Complete feature reference
└── guides/             # How-to guides
    ├── MODE-GUIDE.md   # Game mode walkthroughs
    └── JOYSTICK-SETUP.md # Controller setup
```

### GitHub Configuration

```
.github/
├── workflows/          # CI/CD pipelines
├── ISSUE_TEMPLATE/     # Issue templates
├── PULL_REQUEST_TEMPLATE.md
├── ORG-GUIDELINES.md   # This file
├── planning/           # Internal planning docs
│   └── ROADMAP.md      # Development roadmap
└── copilot-instructions.md # GitHub Copilot context
```

## Documentation Standards

### README Requirements

Every README should include:
1. **Badges**: License, CI status, language
2. **Value proposition**: "Why [Project]?" section
3. **Quick start**: Under 5 minutes to first run
4. **Feature overview**: Tables or bullet lists
5. **Controls reference**: For game projects
6. **Documentation links**: Clear navigation

### AI Discoverability

Include `llms.txt` in repository root with:
- Project description
- Key capabilities
- Quick start commands
- Architecture overview
- Common tasks/patterns

### Planning Documents

Keep planning docs in `.github/planning/`:
- `ROADMAP.md` - Future development plans
- `BACKLOG.md` - Ideas and feature requests (if applicable)
- Session notes and analysis docs (can be archived)

## Code Standards

### C Code (Terminal Stars Specific)

- **Standard**: C99
- **Compiler**: GCC/Clang
- **Headers**: All in `include/` directory
- **Sources**: All in `src/` directory
- **Tests**: In `tests/` directory

### Module Organization

Each major feature should have:
- Header file in `include/`
- Implementation in `src/`
- Tests in `tests/`

## Version Control

### Branch Naming

- `master` - Stable release
- `develop` - Development integration
- `feature/*` - New features
- `fix/*` - Bug fixes
- `release/*` - Release preparation

### Commit Messages

Follow conventional commits:
```
type(scope): description

feat(modes): add skeet shooting mode
fix(render): correct horizon line calculation
docs(readme): add gamepad controls section
```

## Quality Checklist

Before merge/release:
- [ ] README is current and accurate
- [ ] All new features documented
- [ ] Tests pass (`make test`)
- [ ] Build succeeds (`make`)
- [ ] No compiler warnings
- [ ] CHANGELOG updated

## Maturity Levels

### Minimal (Current)
- Working code
- Basic README
- MIT License

### Polished (Target)
- Comprehensive README with "Why" section
- Full documentation in `docs/`
- `llms.txt` for AI discoverability
- Clean repository structure
- CI/CD pipeline

### Showcase
- Demo recordings/screenshots
- Blog post or article
- Cross-project integration examples
