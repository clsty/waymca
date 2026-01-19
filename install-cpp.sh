#!/bin/bash
set -e

echo "=== WayMCA C++ Install Script ==="
echo ""

# Check if build directory exists
if [ ! -d "build" ]; then
    echo "Error: Build directory not found."
    echo "Please run ./build.sh first."
    exit 1
fi

cd build

echo "Installing WayMCA effect..."
sudo make install

echo ""
echo "✓ Installation complete!"
echo ""
echo "Next steps:"
echo "  1. Restart KWin:"
echo "     For Wayland: Log out and log back in"
echo "     For X11: Run 'kwin_x11 --replace' in a terminal"
echo ""
echo "  2. Enable the effect:"
echo "     System Settings → Workspace Behavior → Desktop Effects"
echo "     Find 'WayMCA' and enable it"
echo ""
echo "  3. Configure (optional):"
echo "     Click the configure button next to WayMCA"
echo ""
echo "To uninstall, run ./uninstall-cpp.sh"
