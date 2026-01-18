# Technical Documentation

## Architecture Overview

WayMCA is implemented as a declarative KWin effect using QML and GLSL shaders. This document describes the technical details of the implementation.

## File Structure

```
waymca/
├── metadata.json              # KPackage metadata
├── contents/
│   ├── config/
│   │   └── main.xml          # Configuration schema (KConfigXT)
│   ├── ui/
│   │   ├── main.qml          # Main effect logic
│   │   └── config.qml        # Configuration UI
│   └── shaders/
│       └── waymca.frag       # Fragment shader (GLSL)
├── install.sh                # Installation script
├── uninstall.sh              # Uninstallation script
└── README.md                 # User documentation
```

## Component Details

### metadata.json

Defines the KPackage structure and plugin metadata:
- **KPackageStructure**: `KWin/Effect` - Identifies this as a KWin effect
- **X-Plasma-API**: `declarativescript` - Uses QML/declarative implementation
- **Id**: `waymca` - Unique identifier for the effect
- **Category**: `Appearance` - Category in System Settings

### contents/config/main.xml

KConfigXT schema defining three configuration options:
- **GreenBlurRadius** (Int, 0-20, default: 3)
- **BlueBlurRadius** (Int, 0-20, default: 8)
- **UseGaussianBlur** (Bool, default: true)

### contents/ui/main.qml

Main effect implementation using:
- **FullScreenEffect**: KWin component for full-screen effects
- **FullScreenShaderEffect**: Applies the GLSL shader to the entire screen
- Reads configuration values using `KWin.readConfig()`
- Passes uniforms to the shader

### contents/ui/config.qml

Configuration UI using:
- **SimpleKCM**: KDE Config Module base component
- **Slider**: For blur radius adjustment (0-20 pixels)
- **CheckBox**: For Gaussian blur toggle
- Property aliases bind UI controls to configuration values

### contents/shaders/waymca.frag

GLSL fragment shader implementing per-channel blur:

#### Shader Inputs
- **sampler**: Source texture (screen content)
- **greenBlurRadius**: Blur radius for green channel (float)
- **blueBlurRadius**: Blur radius for blue channel (float)
- **useGaussianBlur**: Toggle between Gaussian and box blur (float)

#### Blur Algorithms

**Gaussian Blur** (useGaussianBlur = 1.0):
- Uses a 3x3 kernel with normalized weights
- Weights based on Gaussian distribution (σ = 1.0)
- Kernel is scaled by radius parameter for larger blur
- Higher quality but more computationally expensive

**Box Blur** (useGaussianBlur = 0.0):
- Simple average of surrounding pixels
- Kernel size based on radius (2*radius + 1)²
- Faster but lower quality
- Good for larger blur radii

#### Per-Channel Processing
1. **Red channel**: Copied directly from source (no blur)
2. **Green channel**: Blurred with greenBlurRadius
3. **Blue channel**: Blurred with blueBlurRadius
4. **Alpha channel**: Preserved from source

## Scientific Background

### Myopic Chromatic Aberration Simulation

The human eye has longitudinal chromatic aberration (LCA) where:
- Red light focuses slightly behind the retina
- Blue light focuses in front of the retina
- Green light is between red and blue

In a myopic (nearsighted) eye, this creates "myopic defocus" where blue is sharper than red. WayMCA simulates the opposite ("red in focus" filter) to potentially help control myopia progression.

### Implementation Rationale

**Default blur radii** (based on Swiatczak et al., 2024):
- Green: 3 pixels - Moderate blur simulating slight defocus
- Blue: 8 pixels - Strong blur simulating significant defocus
- Red: 0 pixels - Sharp, simulating proper focus

These values are calibrated for typical monitor pixel densities and viewing distances to approximate the optical defocus used in the research study.

### Gaussian vs Box Blur

**Gaussian blur** is preferred because:
- More closely approximates optical blur
- Smoother blur transitions
- Better quality appearance

**Box blur** is provided as an alternative for:
- Lower-end hardware
- Very large blur radii (where Gaussian becomes expensive)
- User preference for different visual characteristics

## Performance Considerations

### Shader Optimization

The shader is optimized for real-time performance:

1. **Conditional blur**: If radius < 0.5, skip blur computation
2. **Small kernel**: 9-tap Gaussian (3x3) for good quality/performance balance
3. **Separable blur**: Could be improved by separating horizontal/vertical passes
4. **Radius scaling**: Single kernel scaled for different radii

### Performance Impact

Expected performance characteristics:
- **Minimal blur (defaults)**: < 1ms per frame on modern GPUs
- **Large blur (20px)**: 2-5ms per frame depending on resolution
- **Screen resolution**: Linear impact (4K = 4x cost of 1080p)

### Potential Optimizations

Future improvements could include:
1. **Separable Gaussian**: Two 1D passes instead of one 2D pass
2. **Mipmap sampling**: For very large blur radii
3. **Compute shader**: For better GPU utilization
4. **Adaptive quality**: Reduce quality during window motion

## KWin Integration

### Effect Lifecycle

1. **Loading**: KWin loads metadata.json and instantiates main.qml
2. **Configuration**: Effect reads config values from KConfigXT
3. **Rendering**: Shader is compiled and applied on each frame
4. **Updates**: Config changes trigger shader uniform updates

### System Settings Integration

The effect appears in:
- **Desktop Effects** list with name "WayMCA"
- **Configure** button opens config.qml
- Changes are saved to `~/.config/kwinrc`

## Installation Details

### Manual Installation Path

```
~/.local/share/kwin/effects/waymca/
├── metadata.json
└── contents/
    ├── config/
    ├── ui/
    └── shaders/
```

### kpackagetool6 Installation

Alternative installation using KDE's package tool:
```bash
kpackagetool6 --type=KWin/Effect --install .
```

Installs to the same location with additional validation.

## Debugging

### Common Issues

**Effect doesn't appear in Desktop Effects:**
- Ensure KWin was restarted after installation
- Check file permissions
- Verify metadata.json syntax

**Shader errors:**
- Check KWin logs: `journalctl -f | grep kwin`
- Verify GLSL version compatibility
- Check uniform names match QML properties

**Poor performance:**
- Reduce blur radii
- Try box blur instead of Gaussian
- Check GPU driver support

### Testing

Manual testing checklist:
1. Install effect
2. Enable in System Settings
3. Verify screen appears with red-tinted blur
4. Adjust green blur radius → see green channel blur change
5. Adjust blue blur radius → see blue channel blur change
6. Toggle Gaussian blur → see quality change
7. Disable effect → screen returns to normal

## Compatibility

### Requirements

- **KDE Plasma**: 6.0 or later
- **KWin**: 6.0 or later
- **Qt**: 6.0 or later
- **OpenGL**: 3.0 or later (for GLSL 1.40)

### Tested Environments

This effect should work on:
- ✓ KDE Plasma 6.x on X11
- ✓ KDE Plasma 6.x on Wayland
- ✗ KDE Plasma 5.x (different API)
- ✗ Other desktop environments (KWin-specific)

## Future Enhancements

Potential improvements:
1. **Per-monitor configuration**: Different settings for different displays
2. **Time-based scheduling**: Auto-enable/disable at specific times
3. **Intensity control**: Overall effect strength multiplier
4. **Color temperature shift**: Additional red bias for enhanced effect
5. **Eye strain reduction**: Blue light filter mode
6. **Performance profiling**: Built-in FPS monitoring

## References

- KWin Effect Development: https://develop.kde.org/docs/plasma/kwin/
- KPackage Structure: https://api.kde.org/frameworks/kpackage/html/
- GLSL Specification: https://www.khronos.org/opengl/wiki/OpenGL_Shading_Language
- Research Paper: Swiatczak et al. (2024), Journal of Optometry, 17(2)

## License

GPL-3.0 - See LICENSE file for details
