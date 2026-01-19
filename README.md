# WayMCA - Myopia Control Chromatic Aberration Effect for KWin

WayMCA is a KWin effect that simulates myopic chromatic aberration by selectively blurring the green and blue color channels while keeping the red channel sharp. This effect is based on research showing that such chromatic defocus may help control myopia progression during computer work.

## ⚠️ Current Status

**This is a C++ implementation for KDE Plasma 6.** The project builds and installs successfully, but there is a known issue with the configuration UI where the Apply button doesn't respond to changes. See [STATUS.md](STATUS.md) for complete details.

## Background

Based on the pilot study by Swiatczak et al. (2024), exposing myopic eyes to a "red in focus" filter (which blurs green and blue channels while keeping red sharp) for 2 hours per day over 12 consecutive days resulted in:
- Progressive choroidal thickening (+18 ± 14μm, p < 0.0001)
- Axial length reduction (-31 ± 39μm, p < 0.01)

These changes suggest a potential inhibitory effect on myopia progression.

## Features

- **C++ native implementation**: Proper KWin 6 OffscreenEffect integration
- **Configurable blur radii**: Separate control for green and blue channel blur
- **Gaussian or box blur**: Choose between quality and performance
- **Debug mode**: Full-screen blur option for testing
- **KDE Plasma 6 integration**: Native System Settings configuration UI

## System Requirements

- KDE Plasma 6
- KWin 6
- CMake >= 3.16
- Qt6 and KF6 development libraries
- C++20 compiler

See [BUILD_GUIDE.md](BUILD_GUIDE.md) for detailed dependency information.

## Installation

### Quick Start

1. **Clone the repository:**
```bash
git clone https://github.com/clsty/waymca.git
cd waymca
```

2. **Build the effect:**
```bash
./build.sh
```

3. **Install (requires sudo):**
```bash
./install-cpp.sh
```

4. **Restart KWin:**
   - **For Wayland:** Log out and log back in
   - **For X11:** Run `kwin_x11 --replace` in a terminal

5. **Enable the effect:**
   - Open System Settings → Workspace Behavior → Desktop Effects
   - Find "WayMCA" in the list
   - Check the box to enable it

### Uninstallation

```bash
./uninstall-cpp.sh
```

## Configuration

Once installed, you can configure the effect through System Settings:

1. System Settings → Workspace Behavior → Desktop Effects
2. Find "WayMCA" and click the "Configure" button
3. Adjust the following settings:
   - **Green Blur Radius** (0-20px, default: 3)
   - **Blue Blur Radius** (0-20px, default: 8)
   - **Use Gaussian Blur** (default: enabled)
   - **Full-Screen Blur** (debug mode, default: disabled)
   - **Full-Screen Blur Radius** (0-50px, default: 10)

**Note:** There is currently a known issue where the Apply button may not respond. See [STATUS.md](STATUS.md) for details.

## How It Works

The effect uses a GLSL fragment shader that processes each pixel:

1. **Red Channel**: Kept sharp (no blur)
2. **Green Channel**: Moderate blur (default 3px radius)
3. **Blue Channel**: Stronger blur (default 8px radius)

This creates a "red in focus" chromatic aberration that may help reduce myopia progression during screen time.

### Debug Mode

The full-screen blur option applies uniform blur to the entire screen (not per-channel). This is useful for:
- Testing if the effect is loading correctly
- Verifying configuration changes are being applied
- Debugging shader issues

## Documentation

- **[STATUS.md](STATUS.md)** - Current project status and known issues
- **[BUILD_GUIDE.md](BUILD_GUIDE.md)** - Detailed build instructions
- **[TECHNICAL.md](TECHNICAL.md)** - Technical implementation details
- **[IMPLEMENTATION_NOTE.md](IMPLEMENTATION_NOTE.md)** - Why C++ instead of QML
- **[CONTRIBUTING.md](CONTRIBUTING.md)** - Contribution guidelines

## Why C++ Instead of QML?

This project was initially attempted as a QML scripted effect but was rewritten in C++ because:

1. `FullScreenEffect` is not available in QML (C++ only)
2. Shader effects require the C++ OffscreenEffect API
3. All maintained KWin 6 shader effects use C++

See [IMPLEMENTATION_NOTE.md](IMPLEMENTATION_NOTE.md) for complete explanation.

## Troubleshooting

### Build Fails

Check that you have all required dependencies:
```bash
# See BUILD_GUIDE.md for your distribution
```

### Effect Doesn't Appear

1. Verify installation:
```bash
find /usr -name "*waymca*"
```

2. Check for errors:
```bash
journalctl -f | grep waymca
```

3. Restart KWin completely (log out/in)

### Effect Appears But Doesn't Work

1. Enable debug full-screen blur mode
2. Set blur radius to 30+
3. If screen blurs, the effect is working but per-channel mode may need adjustment
4. If nothing happens, check journalctl for shader errors

## Research Reference

Swiatczak, L., Schaeffel, F., & Prousali, E. (2024). Exposure to red light increases choroidal thickness and reduces axial length in myopic eyes. *Investigative Ophthalmology & Visual Science*, 65(7), 2344.

## Contributing

Contributions are welcome! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

## License

GPL-3.0 - See [LICENSE](LICENSE) file for details.

## Acknowledgments

- KDE Plasma and KWin teams for the effect framework
- LightlyShaders project for C++ implementation reference
- Research by Swiatczak et al. for the scientific basis

---

**Project Repository:** https://github.com/clsty/waymca  
**Current Status:** See [STATUS.md](STATUS.md)
```

### Method 3: Manual Installation

If you prefer to install manually:

```bash
# Create the KWin effects directory if it doesn't exist
mkdir -p ~/.local/share/kwin/effects/waymca

# Copy the effect files
cp metadata.json ~/.local/share/kwin/effects/waymca/
cp -r contents ~/.local/share/kwin/effects/waymca/

# Restart KWin
kwin_x11 --replace &  # For X11
# or log out and log back in for Wayland
```

### Uninstallation

To remove the effect, run:
```bash
./uninstall.sh
```

Or manually:
```bash
rm -rf ~/.local/share/kwin/effects/waymca
```

## Configuration

1. Open **System Settings**
2. Navigate to **Workspace Behavior** → **Desktop Effects**
3. Find **WayMCA** in the list
4. Click the **Configure** button to adjust:
   - **Green Blur Radius**: Amount of blur for the green channel (default: 3, range: 0-20)
   - **Blue Blur Radius**: Amount of blur for the blue channel (default: 8, range: 0-20)
   - **Use Gaussian Blur**: Enable for better quality blur (default: enabled)
   - **Debug: Full-Screen Blur**: Enable to test if the effect is working (blurs entire screen)
   - **Full-Screen Blur Radius**: Blur amount for debug mode (default: 10, range: 0-50)

## Usage

Once installed and configured:

1. Enable the effect in System Settings → Desktop Effects
2. The effect will apply globally to your entire screen
3. Use it during computer work sessions (recommended: 2 hours per day)
4. Adjust blur radii based on your comfort and visual needs

## Troubleshooting

If you don't see any visual effect even with maximum blur settings:

1. Open the WayMCA configuration dialog
2. Enable the **"Full-Screen Blur"** debug option
3. Set the **Full-Screen Blur Radius** to 30-50
4. Apply the settings
5. If the screen becomes blurred, the effect works but per-channel mode may need adjustment
6. If nothing happens, check:
   - KWin has been restarted after installation
   - The effect is enabled in Desktop Effects
   - Check KWin logs: `journalctl -f | grep kwin` for errors

## Technical Details

### Shader Implementation

The effect uses a GLSL fragment shader that:
- Keeps the red channel unmodified (sharp)
- Applies configurable blur to the green channel
- Applies stronger blur to the blue channel
- Supports both Gaussian and box blur algorithms

### Default Values

Based on the research paper, the default values are calibrated for simulating myopic chromatic aberration:
- Green blur radius: 3 pixels
- Blue blur radius: 8 pixels
- Gaussian blur: enabled

## Why QML Instead of C++?

This implementation uses QML/declarative scripting instead of C++ because:
- **No compilation required**: Works out of the box without complex build dependencies
- **Easier maintenance**: No need to match KWin's exact C++ API versions
- **Simpler distribution**: Just copy files, no building needed
- **Cross-version compatibility**: More likely to work across different KWin versions

The previous C++ implementation (PR #1) encountered build issues with missing KWinEffects packages and complex dependencies, making it difficult to build and distribute.

## Compatibility

- **KDE Plasma**: 6.0 or later
- **Qt**: 6.0 or later
- **KWin**: 6.0 or later

## Medical Disclaimer

This software is based on pilot research and is provided for experimental purposes. It is not a medical device and should not be considered a substitute for professional medical advice or treatment. Consult with an eye care professional before using this or any other myopia control intervention.

## License

GPL-3.0 - See LICENSE file for details

## References

Swiatczak, B., Ingrassia, L., Scholl, H. P. N., & Schaeffel, F. (2024). Pilot study: simulating myopic chromatic aberration on a computer screen induces progressive choroidal thickening in myopes. *Journal of Optometry*, 17(2).

## Contributing

Contributions are welcome! Please feel free to submit issues or pull requests.

## Repository

https://github.com/clsty/waymca
