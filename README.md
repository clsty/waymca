# WayMCA - Myopia Control Chromatic Aberration Effect for KWin

WayMCA is a KWin effect that simulates myopic chromatic aberration by selectively blurring the green and blue color channels while keeping the red channel sharp. This effect is based on research showing that such chromatic defocus may help control myopia progression during computer work.

## Background

Based on the pilot study by Swiatczak et al. (2024), exposing myopic eyes to a "red in focus" filter (which blurs green and blue channels while keeping red sharp) for 2 hours per day over 12 consecutive days resulted in:
- Progressive choroidal thickening (+18 ± 14μm, p < 0.0001)
- Axial length reduction (-31 ± 39μm, p < 0.01)

These changes suggest a potential inhibitory effect on myopia progression.

## Features

- **QML-based implementation**: No compilation required, easy to install
- **Configurable blur radii**: Separate control for green and blue channel blur
- **Gaussian or box blur**: Choose between quality and performance
- **KDE Plasma 6 integration**: Native System Settings configuration UI

## Installation

### Method 1: Quick Installation (Recommended)

1. Clone this repository:
```bash
git clone https://github.com/clsty/waymca.git
cd waymca
```

2. Run the installation script:
```bash
./install.sh
```

3. Follow the on-screen instructions to restart KWin and enable the effect.

### Method 2: Using kpackagetool6

```bash
kpackagetool6 --type=KWin/Effect --install .
```

To update an existing installation:
```bash
kpackagetool6 --type=KWin/Effect --upgrade .
```

To uninstall:
```bash
kpackagetool6 --type=KWin/Effect --remove waymca
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
   - **Green Blur Radius**: Amount of blur for the green channel (default: 3)
   - **Blue Blur Radius**: Amount of blur for the blue channel (default: 8)
   - **Use Gaussian Blur**: Enable for better quality blur (default: enabled)

## Usage

Once installed and configured:

1. Enable the effect in System Settings → Desktop Effects
2. The effect will apply globally to your entire screen
3. Use it during computer work sessions (recommended: 2 hours per day)
4. Adjust blur radii based on your comfort and visual needs

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
