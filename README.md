# WayMCA - Myopic Chromatic Aberration Effect for KDE Plasma 6

WayMCA is a KWin effect plugin for KDE Plasma 6 (Wayland) that simulates longitudinal chromatic aberration for myopia prevention/control.

## What It Does

This effect simulates the chromatic aberration found in myopic (nearsighted) eyes by:
- Keeping the **Red channel** clear (original sharp image)
- Applying a **slight blur to the Green channel**
- Applying a **significant blur to the Blue channel**

This simulates the "red in focus" condition described in research on myopia control, where the longer wavelength (red) light is in focus while shorter wavelengths (blue) are defocused behind the retina.

## Research Background

See the `reference/` directory for the scientific paper that inspired this effect:
- Swiatczak et al. (2024): "Pilot study: simulating myopic chromatic aberration on a computer screen induces progressive choroidal thickening in myopes"

## Requirements

- KDE Plasma 6
- Qt6 (Core, Gui, Widgets)
- KF6 (Config, ConfigWidgets, CoreAddons, I18n)
- KWin development files
- OpenGL/epoxy
- CMake 3.16+

### On Debian 13 (Trixie) or later:

```bash
sudo apt install cmake build-essential extra-cmake-modules \
    libkf6config-dev libkf6configwidgets-dev libkf6coreaddons-dev \
    libkf6i18n-dev kwin-dev qtbase6-dev libepoxy-dev
```

## Building

```bash
mkdir build
cd build
cmake ..
make
sudo make install
```

## Usage

1. After installation, restart KWin:
   ```bash
   kwin_wayland --replace &
   ```
   Or log out and log back in.

2. Open System Settings → Desktop Effects
3. Find "WayMCA" under the "Appearance" category
4. Enable the effect
5. Click the settings/configure icon to adjust:
   - Green blur radius (default: 3)
   - Blue blur radius (default: 8)
   - Blur algorithm (Gaussian or Box blur)

## Configuration

The effect can be configured through KDE System Settings:
- **Green Blur Radius**: Controls the amount of blur applied to the green channel (0-20 pixels)
- **Blue Blur Radius**: Controls the amount of blur applied to the blue channel (0-20 pixels)
- **Use Gaussian Blur**: Toggle between Gaussian blur (smoother) and Box blur (faster)

## Technical Details

### Files

- `metadata.json` - Plugin metadata
- `src/waymca.h` - Effect header
- `src/waymca.cpp` - Effect implementation
- `src/waymca.frag` - GLSL fragment shader
- `src/waymca.kcfg` - Configuration schema
- `src/waymca_config.ui` - Configuration UI
- `src/waymca_config.h` - Configuration module header
- `src/waymca_config.cpp` - Configuration module implementation

### How It Works

The effect uses OpenGL shaders to process the screen output:
1. Captures the rendered screen to an offscreen texture
2. Applies the chromatic aberration shader:
   - Red channel: passes through unmodified
   - Green channel: applies blur based on configured radius
   - Blue channel: applies stronger blur based on configured radius
3. Renders the result to the screen

## License

GPL-3.0 - See LICENSE file for details

## Disclaimer

This software is provided for research and experimental purposes. Consult with an eye care professional before using this for myopia control. The effectiveness of this approach for myopia prevention/control in real-world usage has not been fully established.

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues.

## References

Swiatczak, B., Ingrassia, L., Scholl, H. P. N., & Schaeffel, F. (2024). Pilot study: simulating myopic chromatic aberration on a computer screen induces progressive choroidal thickening in myopes. *Scientific Journal of Vision Sciences*, 17(2).
