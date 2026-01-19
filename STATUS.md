# WayMCA Project Status

**Last Updated:** 2026-01-19  
**Current Implementation:** C++ KWin Effect for Plasma 6  
**Build Status:** ✅ Compiles Successfully  
**Installation Status:** ✅ Installs Successfully  
**Functionality Status:** ⚠️ Partial - Known Issues Below

---

## Current State

### What Works ✅

1. **Build System**
   - CMake configuration works correctly
   - All dependencies resolve properly
   - Compiles without errors using `./build.sh`
   - Generates both effect plugin and config module

2. **Installation**
   - System-wide installation via `./install-cpp.sh` works
   - Files install to correct locations:
     - Effect: `/usr/lib/x86_64-linux-gnu/qt6/plugins/kwin/effects/plugins/`
     - Config: `/usr/lib/x86_64-linux-gnu/qt6/plugins/kwin/effects/configs/`
     - Data: `/usr/share/kwin/effects/waymca/`
   
3. **KDE Integration**
   - Effect appears in System Settings → Desktop Effects
   - Effect can be enabled/disabled
   - Configure button appears next to the effect
   - No journalctl errors after removing Id field from metadata

4. **Code Structure**
   - Proper OffscreenEffect base class usage
   - Shader loading from file system
   - Configuration reading from kwinrc
   - Plugin metadata correctly derived from filename

### Known Issues ⚠️

1. **Configuration UI - Apply Button Not Working**
   - **Status:** UNRESOLVED
   - **Symptom:** Configuration dialog opens, but Apply/OK buttons don't respond to changes
   - **What's Been Tried:**
     - Added signal connections for all UI widgets (QSlider, QCheckBox)
     - Implemented `setNeedsSave(true)` when values change
     - Implemented `setNeedsSave(false)` after save/load
     - Fixed signal types (QSlider::valueChanged vs QSpinBox::valueChanged)
   - **Likely Cause:** Additional KCModule setup may be required (e.g., addConfig() pattern)
   - **Next Steps to Try:**
     - Review LightlyShaders' blur_config.cpp more carefully
     - Check if KConfigXT automatic binding is needed
     - Test if manual signal-slot connections interfere with KCModule

2. **Effect Functionality - Not Verified**
   - **Status:** UNKNOWN
   - **Reason:** Cannot test effect until configuration can be changed and saved
   - **Shader Implementation:** Complete (per-channel blur + debug full-screen blur)
   - **Effect Logic:** Complete (OffscreenEffect with shader application)

---

## Project Structure

### Core Implementation Files

```
src/
├── waymca.h                    # Effect header (OffscreenEffect)
├── waymca.cpp                  # Effect implementation
├── waymca_config.cpp          # Configuration module (KCModule)
├── waymca_config.ui           # Qt Designer UI file
├── waymca.kcfg                # KConfigXT schema
├── metadata.json              # Plugin metadata (no Id field)
├── main.cpp                   # Plugin entry point
└── CMakeLists.txt             # Build configuration

data/
├── waymca.json                # Effect metadata (no Id field)
├── shaders/
│   └── waymca.frag           # GLSL fragment shader
└── CMakeLists.txt             # Data installation
```

### Build & Installation Scripts

- **build.sh** - Builds the C++ plugin using CMake
- **install-cpp.sh** - Installs built plugin system-wide (requires sudo)
- **uninstall-cpp.sh** - Removes installed plugin
- **install.sh** - OLD: QML installation (deprecated, kept for reference)
- **uninstall.sh** - OLD: QML uninstallation (deprecated)
- **install-local.sh** - OLD: Local QML install (deprecated)
- **uninstall-local.sh** - OLD: Local QML uninstall (deprecated)

### Documentation Files

- **README.md** - User-facing documentation (needs update to reflect C++ version)
- **BUILD_GUIDE.md** - Detailed build instructions for C++ version
- **TECHNICAL.md** - Technical implementation details
- **IMPLEMENTATION_NOTE.md** - Why C++ was chosen over QML
- **CONTRIBUTING.md** - Contribution guidelines
- **STATUS.md** - This file - current project status

---

## Why C++ Instead of QML?

The project was initially implemented in QML but had to be rewritten in C++ because:

1. **FullScreenEffect Not Available in QML**
   - `FullScreenEffect` is only a C++ class in KWin 6
   - Never registered with QML/JavaScript engine
   - QML scripted effects have limited API access

2. **Shader Effects Require C++ API**
   - Full-screen shader effects need OffscreenEffect base class
   - Direct OpenGL/shader management only available in C++
   - QML effects had syntax errors when trying to load shaders

3. **Working Examples Use C++**
   - LightlyShaders, Blur, and other shader effects are C++
   - Burn-My-Windows (JavaScript) is for animations, not shaders
   - All maintained KWin 6 shader effects use C++ API

---

## Configuration Schema

### Settings (defined in waymca.kcfg)

| Setting | Type | Range | Default | Description |
|---------|------|-------|---------|-------------|
| GreenBlurRadius | Int | 0-20 | 3 | Green channel blur radius in pixels |
| BlueBlurRadius | Int | 0-20 | 8 | Blue channel blur radius in pixels |
| UseGaussianBlur | Bool | - | true | Use Gaussian blur (vs box blur) |
| FullScreenBlur | Bool | - | false | Debug: blur entire screen |
| FullScreenBlurRadius | Int | 0-50 | 10 | Debug: full-screen blur radius |

### Storage Location

Settings are stored in `~/.config/kwinrc` under the `[Effect-waymca]` group:

```ini
[Effect-waymca]
GreenBlurRadius=3
BlueBlurRadius=8
UseGaussianBlur=true
FullScreenBlur=false
FullScreenBlurRadius=10
```

---

## Build Instructions (Quick Reference)

```bash
# 1. Build
./build.sh

# 2. Install (requires sudo)
./install-cpp.sh

# 3. Restart KWin
# Wayland: Log out and log back in
# X11: kwin_x11 --replace

# 4. Enable effect
# System Settings → Desktop Effects → WayMCA

# 5. Uninstall (if needed)
./uninstall-cpp.sh
```

---

## Dependencies Summary

### Build-time
- CMake >= 3.16
- C++20 compiler
- Qt6 (Core, DBus, Gui, Widgets)
- KF6 (Config, ConfigWidgets, CoreAddons, GlobalAccel, I18n, KCMUtils, WindowSystem)
- KWin development files
- libepoxy

### Runtime
- KDE Plasma 6
- KWin 6
- Qt6 runtime
- KF6 runtime

---

## Testing Checklist

### When Configuration Issue is Fixed

- [ ] Open configuration dialog
- [ ] Change green blur radius slider
- [ ] Verify Apply button becomes enabled
- [ ] Click Apply
- [ ] Verify settings save to kwinrc
- [ ] Close and reopen dialog
- [ ] Verify settings persist
- [ ] Enable debug full-screen blur
- [ ] Set radius to 30+
- [ ] Apply and verify screen blurs
- [ ] Test per-channel blur with various radii
- [ ] Toggle Gaussian vs box blur
- [ ] Verify visual difference

---

## Development Notes

### Metadata Changes Made

1. **Removed Id field from KPlugin** (commit 2b9989c)
   - Was causing: "explicitly states an Id in the embedded metadata, which is different from the one derived from the filename"
   - Now: Id derived from filename (kwin_effect_waymca.so → waymca)

2. **Added X-KDE-ConfigModule** (commit 4a7d443)
   - Links effect to configuration module
   - Value: "kwin_effect_waymca_config" (matches CMake target name)

3. **Fixed Signal Connections** (commit d171194)
   - Changed QSpinBox::valueChanged to QSlider::valueChanged
   - UI uses QSlider widgets, not QSpinBox

### Still To Investigate

1. **KConfigXT Integration**
   - LightlyShaders uses `addConfig(BlurConfig::self(), widget())`
   - Automatically binds UI to config without manual signal connections
   - May need to generate config skeleton class from .kcfg file

2. **Config Module Registration**
   - Verify kcm plugin is properly registered with KWin
   - Check if desktop file is needed (likely not for C++ modules)

3. **Effect Reconfiguration Signal**
   - May need to call `interface.reconfigureEffect("waymca")` on save
   - Ensures effect reloads settings after Apply

---

## Future Work

### Priority 1 - Fix Configuration

- [ ] Investigate KConfigXT automatic binding
- [ ] Compare with LightlyShaders config implementation line-by-line
- [ ] Test if removing manual signal connections helps
- [ ] Verify config skeleton generation from .kcfg

### Priority 2 - Test Effect Functionality

- [ ] Once config works, test debug full-screen blur
- [ ] Test per-channel blur with various radii
- [ ] Measure performance impact
- [ ] Verify shader correctness

### Priority 3 - Polish

- [ ] Update README.md to reflect C++ implementation
- [ ] Add screenshots to documentation
- [ ] Create PKGBUILD for Arch Linux
- [ ] Add CI/CD for automated builds

---

## References

- **LightlyShaders:** https://github.com/a-parhom/LightlyShaders
  - Working C++ KWin effect with configuration
  - Uses KConfigXT with addConfig() pattern
  
- **KWin Effect Tutorial:** https://develop.kde.org/docs/plasma/kwin/
  - Official KDE documentation for effect development

- **Research Paper:** Swiatczak et al. (2024)
  - "Exposure to red light increases choroidal thickness and reduces axial length in myopic eyes"
  - Basis for the chromatic aberration approach

---

## Contributors

- Initial QML implementation: WayMCA Contributors
- C++ rewrite: Assisted by AI (Copilot, Cline)
- Repository: https://github.com/clsty/waymca

---

## License

GPL-3.0 - See LICENSE file for details
