#!/bin/bash
set -e

echo "=== WayMCA C++ Build Script ==="
echo ""

# Check for required tools
command -v cmake >/dev/null 2>&1 || { echo "Error: cmake is required but not installed."; exit 1; }
command -v make >/dev/null 2>&1 || { echo "Error: make is required but not installed."; exit 1; }

# Create build directory
BUILD_DIR="build"
if [ -d "$BUILD_DIR" ]; then
    echo "Cleaning existing build directory..."
    rm -rf "$BUILD_DIR"
fi

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

echo "Running CMake..."
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=/usr

echo ""
echo "Building..."
make -j$(nproc)

echo ""
echo "✓ Build complete!"
echo ""
echo "To install, run:"
echo "  sudo make install"
echo "  (or run ./install-cpp.sh)"
echo ""
echo "After installation:"
echo "  1. Restart KWin (logout/login or run: kwin_wayland --replace)"
echo "  2. Enable in System Settings → Desktop Effects"
