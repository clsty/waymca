#!/bin/bash
set -e

echo "=== WayMCA Local Install Script ==="
echo "Installing to ~/.local/share (user directory)"
echo ""

# Check if build directory exists
if [ ! -d "build" ]; then
    echo "Error: Build directory not found."
    echo "Please run ./build.sh first."
    exit 1
fi

# Create directories (C++ plugin doesn't need KPackage structure)
echo "Creating directories..."
mkdir -p ~/.local/share/kwin/effects/waymca/shaders
mkdir -p ~/.local/lib/qt6/plugins/kwin/effects/plugins

# Copy plugin binary
echo "Installing plugin binary..."
cp build/bin/kwin_effect_waymca.so ~/.local/lib/qt6/plugins/kwin/effects/plugins/

# Copy shader (metadata is compiled into the .so file)
echo "Installing shader..."
cp data/shaders/waymca.frag ~/.local/share/kwin/effects/waymca/shaders/

echo ""
echo "✓ Local installation complete!"
echo ""
echo "Files installed to:"
echo "  ~/.local/lib/qt6/plugins/kwin/effects/plugins/kwin_effect_waymca.so (C++ plugin)"
echo "  ~/.local/share/kwin/effects/waymca/shaders/waymca.frag (shader)"
echo ""
echo "Next steps:"
echo "  1. Restart KWin:"
echo "     For Wayland: Log out and log back in"
echo "     For X11: Run 'kwin_x11 --replace' in a terminal"
echo ""
echo "  2. Enable the effect:"
echo "     System Settings → Workspace Behavior → Desktop Effects"
echo "     Search for 'WayMCA' and enable it"
echo ""
echo "  3. Check logs if not visible:"
echo "     journalctl --user -f | grep -i waymca"
echo ""
echo "To uninstall, run: ./uninstall-local.sh"
