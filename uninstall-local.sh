#!/bin/bash
set -e

echo "=== WayMCA Local Uninstall Script ==="
echo ""

echo "Removing WayMCA effect files from ~/.local..."

# Remove plugin binary (both old and new names)
rm -f ~/.local/lib/qt6/plugins/kwin/effects/plugins/kwin_waymca.so
rm -f ~/.local/lib/qt6/plugins/kwin/effects/plugins/kwin_effect_waymca.so

# Remove KPackage directory
rm -rf ~/.local/share/kwin/effects/waymca

echo ""
echo "✓ Local uninstallation complete!"
echo ""
echo "You may need to restart KWin for changes to take effect:"
echo "  For Wayland: Log out and log back in"
echo "  For X11: Run 'kwin_x11 --replace'"
