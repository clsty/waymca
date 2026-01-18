#!/bin/bash
# Build script for WayMCA KWin Effect

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}WayMCA Build Script${NC}"
echo "===================="

# Check for required commands
echo -e "\n${YELLOW}Checking dependencies...${NC}"
command -v cmake >/dev/null 2>&1 || { echo -e "${RED}cmake is required but not installed.${NC}" >&2; exit 1; }
command -v make >/dev/null 2>&1 || { echo -e "${RED}make is required but not installed.${NC}" >&2; exit 1; }

echo -e "${GREEN}✓${NC} cmake found"
echo -e "${GREEN}✓${NC} make found"

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
    echo "  - extra-cmake-modules"
    echo "  - libkf6config-dev"
    echo "  - libkf6configwidgets-dev"
    echo "  - libkf6coreaddons-dev"
    echo "  - libkf6i18n-dev"
    echo "  - kwin-dev"
    echo "  - qtbase6-dev"
    echo "  - libepoxy-dev"
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
