# WayMCA Scripts Documentation

This document explains all scripts in the repository and when to use them.

---

## C++ Implementation Scripts (Current)

These are the scripts you should use for the current C++ implementation:

### build.sh

**Purpose:** Builds the C++ KWin effect plugin from source.

**Usage:**
```bash
./build.sh
```

**What it does:**
1. Checks for required tools (cmake, make)
2. Creates/cleans the `build/` directory
3. Runs CMake configuration
4. Compiles the code using make

**Output:**
- `build/src/kwin_effect_waymca.so` - Main effect plugin
- `build/src/kwin_effect_waymca_config.so` - Configuration module

**Requirements:**
- CMake >= 3.16
- C++ compiler (g++ or clang++)
- make or ninja
- All development dependencies (see BUILD_GUIDE.md)

**Notes:**
- Safe to run multiple times (cleans build directory)
- Uses all CPU cores with `make -j$(nproc)`
- Build artifacts go to `build/` directory (not installed yet)

---

### install-cpp.sh

**Purpose:** Installs the compiled C++ effect to system directories.

**Usage:**
```bash
./install-cpp.sh
```

**Requirements:**
- Must run `./build.sh` first
- Requires sudo access (installs to /usr)

**What it does:**
1. Checks if build directory exists
2. Runs `sudo make install` from build directory
3. Installs files to:
   - `/usr/lib/x86_64-linux-gnu/qt6/plugins/kwin/effects/plugins/`
   - `/usr/lib/x86_64-linux-gnu/qt6/plugins/kwin/effects/configs/`
   - `/usr/share/kwin/effects/waymca/`

**After Installation:**
1. Restart KWin (log out/in for Wayland, `kwin_x11 --replace` for X11)
2. Enable effect in System Settings → Desktop Effects
3. Configure via the configure button

---

### uninstall-cpp.sh

**Purpose:** Removes the installed C++ effect from system directories.

**Usage:**
```bash
./uninstall-cpp.sh
```

**Requirements:**
- Requires sudo access

**What it does:**
1. Removes effect plugin from `/usr/lib/.../plugins/`
2. Removes config module from `/usr/lib/.../configs/`
3. Removes data files from `/usr/share/kwin/effects/waymca/`

**Notes:**
- Configuration in `~/.config/kwinrc` is preserved
- Does not remove build directory

---

## QML Implementation Scripts (Deprecated)

These scripts are from the old QML implementation. They are kept for reference but should not be used:

### ⚠️ install.sh (OLD - Don't Use)

**Purpose:** Was used to install QML-based effect.

**Status:** DEPRECATED - QML implementation doesn't work in KWin 6

**Why kept:** Reference for potential future QML support if KWin API improves

---

### ⚠️ uninstall.sh (OLD - Don't Use)

**Purpose:** Was used to uninstall QML-based effect.

**Status:** DEPRECATED

---

### ⚠️ install-local.sh (OLD - Don't Use)

**Purpose:** Was used to install QML effect to user's home directory.

**Status:** DEPRECATED

---

### ⚠️ uninstall-local.sh (OLD - Don't Use)

**Purpose:** Was used to uninstall local QML installation.

**Status:** DEPRECATED

---

## Workflow Examples

### Fresh Installation

```bash
# 1. Clone repository
git clone https://github.com/clsty/waymca.git
cd waymca

# 2. Check dependencies (see BUILD_GUIDE.md)
# Install any missing packages

# 3. Build
./build.sh

# 4. Install
./install-cpp.sh

# 5. Restart KWin
# For Wayland: Log out and log back in
# For X11: kwin_x11 --replace &

# 6. Enable in System Settings
# System Settings → Desktop Effects → WayMCA
```

### Rebuilding After Code Changes

```bash
# Clean build and rebuild
./build.sh

# Reinstall
./uninstall-cpp.sh  # Optional: clean install
./install-cpp.sh

# Restart KWin
kwin_x11 --replace &  # or log out/in
```

### Testing Without Installation

You cannot test without installation. KWin loads effects from system directories only.

However, you can:
1. Build: `./build.sh`
2. Inspect build artifacts in `build/src/`
3. Check for compilation errors

### Cleaning Up

```bash
# Remove installed effect
./uninstall-cpp.sh

# Remove build directory
rm -rf build/

# Remove configuration (optional)
# Edit ~/.config/kwinrc and remove [Effect-waymca] section
```

---

## Script Maintenance

### Adding a New Script

1. Create script in root directory
2. Add execution permission: `chmod +x scriptname.sh`
3. Follow existing script conventions:
   - Start with `#!/bin/bash`
   - Use `set -e` to exit on errors
   - Add descriptive echo messages
   - Check prerequisites before running
4. Document here in SCRIPTS.md

### Script Conventions

All scripts follow these patterns:

**Header:**
```bash
#!/bin/bash
set -e

echo "=== Script Name ==="
echo ""
```

**Error Checking:**
```bash
# Check for required tools
command -v tool >/dev/null 2>&1 || { 
    echo "Error: tool is required but not installed."
    exit 1
}
```

**Success Message:**
```bash
echo ""
echo "✓ Operation complete!"
echo ""
echo "Next steps:"
echo "  1. Do this"
echo "  2. Do that"
```

---

## Troubleshooting Scripts

### build.sh Fails

**"cmake: command not found"**
- Install CMake: `sudo apt install cmake` (Ubuntu/Debian)

**"Could not find KWin"**
- Install KWin development files: See BUILD_GUIDE.md

**Compilation errors**
- Check all dependencies are installed
- See BUILD_GUIDE.md for your distribution

### install-cpp.sh Fails

**"Build directory not found"**
- Run `./build.sh` first

**"Permission denied"**
- Script requires sudo access
- Run: `./install-cpp.sh` (it will prompt for password)

### Effect Doesn't Load After Installation

1. **Verify installation:**
   ```bash
   find /usr -name "*waymca*"
   ```
   Should show files in:
   - `/usr/lib/.../plugins/`
   - `/usr/share/kwin/effects/`

2. **Check for errors:**
   ```bash
   journalctl -f | grep waymca
   ```

3. **Restart KWin completely:**
   - Log out and log back in

4. **Check effect is enabled:**
   - System Settings → Desktop Effects
   - Search for "WayMCA"

---

## Future Script Ideas

### Planned

- **test-build.sh** - Run unit tests after building
- **package.sh** - Create distribution packages (deb, rpm, etc.)
- **dev-install.sh** - Install to ~/.local for development

### Not Planned (But Could Be Useful)

- **benchmark.sh** - Measure effect performance
- **validate-shader.sh** - Check shader syntax
- **update-from-git.sh** - Pull latest and rebuild

---

## See Also

- [BUILD_GUIDE.md](BUILD_GUIDE.md) - Complete build instructions
- [STATUS.md](STATUS.md) - Current project status
- [TECHNICAL.md](TECHNICAL.md) - Technical implementation details
