#!/bin/bash
# WayMCA KWin Effect Uninstallation Script

set -e

EFFECT_NAME="waymca"
INSTALL_DIR="$HOME/.local/share/kwin/effects/$EFFECT_NAME"

echo "=== WayMCA KWin Effect Uninstaller ==="
echo

if [ ! -d "$INSTALL_DIR" ]; then
    echo "WayMCA effect is not installed at: $INSTALL_DIR"
    exit 1
fi

echo "This will remove the WayMCA effect from: $INSTALL_DIR"
read -p "Continue? (y/N) " -n 1 -r
echo
if [[ ! $REPLY =~ ^[Yy]$ ]]; then
    echo "Uninstallation cancelled."
    exit 0
fi

echo "Removing effect files..."
rm -rf "$INSTALL_DIR"

echo
echo "✓ Uninstallation complete!"
echo
echo "You may need to restart KWin for changes to take effect:"
echo "   For X11: kwin_x11 --replace &"
echo "   For Wayland: Log out and log back in"
echo
