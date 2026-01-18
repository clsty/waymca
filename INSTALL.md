# Installation Instructions for WayMCA

## Prerequisites

### Debian 13 (Trixie) or later

```bash
sudo apt update
sudo apt install cmake build-essential extra-cmake-modules \
    libkf6config-dev libkf6configwidgets-dev libkf6coreaddons-dev \
    libkf6i18n-dev kwin-dev qtbase6-dev libepoxy-dev
```

### Arch Linux

```bash
sudo pacman -S cmake extra-cmake-modules kwin qt6-base
```

### Fedora

```bash
sudo dnf install cmake extra-cmake-modules kf6-kconfig-devel \
    kf6-kconfigwidgets-devel kf6-kcoreaddons-devel kf6-ki18n-devel \
    kwin-devel qt6-qtbase-devel libepoxy-devel
```

## Building from Source

### Quick Build

Use the provided build script:

```bash
./build.sh
```

### Manual Build

```bash
mkdir build
cd build
cmake ..
make
sudo make install
```

## Post-Installation

After installation, you need to restart KWin to load the new effect:

### Option 1: Restart KWin (Wayland)

```bash
kwin_wayland --replace &
```

### Option 2: Log out and log back in

Simply log out of your KDE session and log back in.

## Enabling the Effect

1. Open **System Settings**
2. Navigate to **Desktop Effects** (or search for "effects")
3. Find **WayMCA** in the effects list (under "Appearance" category)
4. Check the box to enable it
5. Click the settings/configure icon to adjust parameters:
   - **Green Blur Radius**: 0-20 (default: 3)
   - **Blue Blur Radius**: 0-20 (default: 8)
   - **Use Gaussian Blur**: Toggle between Gaussian (smoother) and Box blur (faster)

## Uninstallation

From the build directory:

```bash
cd build
sudo make uninstall
```

Or manually remove:

```bash
sudo rm /usr/lib/*/kwin/effects/plugins/libwaymca.so
sudo rm /usr/lib/*/kwin/effects/configs/libwaymca_config.so
sudo rm -r /usr/share/kwin/effects/waymca
```

## Troubleshooting

### Effect doesn't appear in System Settings

- Make sure you restarted KWin after installation
- Check that the plugin was installed correctly:
  ```bash
  find /usr -name "libwaymca.so" 2>/dev/null
  ```

### Effect is enabled but not working

- Check KWin logs for errors:
  ```bash
  journalctl -b | grep -i waymca
  ```
  or
  ```bash
  journalctl --user -b | grep -i kwin
  ```

- Ensure your system is using OpenGL compositing:
  - System Settings → Display and Monitor → Compositor
  - Rendering backend should be "OpenGL"

### Shader compilation errors

- Make sure your graphics drivers are up to date
- Try with a different blur algorithm (Box blur vs Gaussian)
- Reduce the blur radius values if performance is an issue

## Performance Notes

- The effect applies real-time blur to the entire screen, which may impact performance on older hardware
- Box blur is faster than Gaussian blur
- Lower blur radii will perform better
- Integrated graphics may see more performance impact than dedicated GPUs

## Development

To rebuild after making changes:

```bash
cd build
make
sudo make install
# Then restart KWin
```

For development with live shader editing, you can modify `/usr/share/kwin/effects/waymca/waymca.frag` directly and restart the effect (toggle it off and on in System Settings).
