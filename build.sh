#!/bin/bash
# Build script for WayMCA KWin Effect (Arch Linux)

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}WayMCA Build Script (Arch Linux)${NC}"
echo "===================================="

# Check if running on Arch Linux
if [ -f /etc/arch-release ]; then
    echo -e "${GREEN}✓${NC} Running on Arch Linux"
else
    echo -e "${YELLOW}⚠${NC} This script is optimized for Arch Linux"
fi

# Check for required commands
echo -e "\n${YELLOW}Checking dependencies...${NC}"
command -v cmake >/dev/null 2>&1 || { echo -e "${RED}cmake is required but not installed.${NC}" >&2; exit 1; }
command -v make >/dev/null 2>&1 || { echo -e "${RED}make is required but not installed.${NC}" >&2; exit 1; }

echo -e "${GREEN}✓${NC} cmake found"
echo -e "${GREEN}✓${NC} make found"

# Check for Arch Linux specific packages
echo -e "\n${YELLOW}Checking Arch Linux packages...${NC}"
MISSING_PACKAGES=()

for pkg in extra-cmake-modules kwin qt6-base; do
    if ! pacman -Qi "$pkg" &> /dev/null; then
        MISSING_PACKAGES+=("$pkg")
    fi
done

if [ ${#MISSING_PACKAGES[@]} -gt 0 ]; then
    echo -e "${RED}✗${NC} Missing required packages: ${MISSING_PACKAGES[*]}"
    echo -e "\n${YELLOW}Install with:${NC}"
    echo "  sudo pacman -S ${MISSING_PACKAGES[*]}"
    exit 1
else
    echo -e "${GREEN}✓${NC} All required Arch packages are installed"
fi

# Create build directory
echo -e "\n${YELLOW}Creating build directory...${NC}"
mkdir -p build
cd build

# Run CMake
echo -e "\n${YELLOW}Running CMake...${NC}"
if cmake ..; then
    echo -e "${GREEN}✓${NC} CMake configuration successful"
else
    echo -e "${RED}✗${NC} CMake configuration failed"
    echo -e "${YELLOW}Make sure you have installed all dependencies:${NC}"
    echo "  sudo pacman -S cmake extra-cmake-modules kwin qt6-base"
    exit 1
fi

# Build
echo -e "\n${YELLOW}Building WayMCA...${NC}"
if make -j$(nproc); then
    echo -e "${GREEN}✓${NC} Build successful"
else
    echo -e "${RED}✗${NC} Build failed"
    exit 1
fi

echo -e "\n${GREEN}Build completed successfully!${NC}"
echo -e "\nTo install, run: ${YELLOW}sudo make install${NC}"
echo -e "Then restart KWin: ${YELLOW}kwin_wayland --replace &${NC}"
echo -e "Or log out and log back in."
