#!/bin/bash
# WayMCA KWin Effect Installation Script

set -e

EFFECT_NAME="waymca"
INSTALL_DIR="$HOME/.local/share/kwin/effects/$EFFECT_NAME"

echo "=== WayMCA KWin Effect Installer ==="
echo

# Check if kwin is available
if ! command -v kwin_x11 &> /dev/null && ! command -v kwin_wayland &> /dev/null; then
    echo "Warning: KWin does not appear to be installed or not in PATH"
    echo "This effect requires KDE Plasma 6 with KWin"
    read -p "Continue anyway? (y/N) " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        exit 1
    fi
fi

# Create installation directory
echo "Creating installation directory: $INSTALL_DIR"
mkdir -p "$INSTALL_DIR"

# Copy files
echo "Copying effect files..."
cp metadata.json "$INSTALL_DIR/"
cp -r contents "$INSTALL_DIR/"

echo
echo "✓ Installation complete!"
echo
echo "Next steps:"
echo "1. Restart KWin:"
echo "   For X11: Run 'kwin_x11 --replace' in a terminal"
echo "   For Wayland: Log out and log back in"
echo
echo "2. Enable the effect:"
echo "   System Settings → Workspace Behavior → Desktop Effects"
echo "   Find 'WayMCA' and enable it"
echo
echo "3. Configure the effect (optional):"
echo "   Click the configure button next to WayMCA in Desktop Effects"
echo
echo "To uninstall, run: ./uninstall.sh"
echo
