#!/bin/bash
set -e

echo "=== WayMCA C++ Uninstall Script ==="
echo ""

echo "Removing WayMCA effect files..."

# Remove plugin files
sudo rm -f /usr/lib/*/qt6/plugins/kwin/effects/plugins/kwin_waymca.so
sudo rm -f /usr/lib/*/kwin/effects/plugins/kwin_waymca.so

# Remove config plugin
sudo rm -f /usr/lib/*/qt6/plugins/kwin/effects/configs/kwin_waymca_config.so
sudo rm -f /usr/lib/*/kwin/effects/configs/kwin_waymca_config.so

# Remove data files
sudo rm -rf /usr/share/kwin/effects/waymca
sudo rm -f /usr/share/kwin/effects/waymca.json

echo ""
echo "✓ Uninstallation complete!"
echo ""
echo "You may need to restart KWin for changes to take effect:"
echo "  For Wayland: Log out and log back in"
echo "  For X11: Run 'kwin_x11 --replace'"
