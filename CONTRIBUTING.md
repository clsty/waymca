# Contributing to WayMCA

Thank you for your interest in contributing to WayMCA! This document provides guidelines for contributing to the project.

## How to Contribute

### Reporting Issues

If you encounter bugs or have feature requests:

1. Check if the issue already exists in the [issue tracker](https://github.com/clsty/waymca/issues)
2. If not, create a new issue with:
   - Clear description of the problem or feature
   - Steps to reproduce (for bugs)
   - Your environment (KDE Plasma version, Qt version, GPU)
   - Expected vs actual behavior

### Code Contributions

1. **Fork the repository**
   ```bash
   git clone https://github.com/clsty/waymca.git
   cd waymca
   ```

2. **Create a feature branch**
   ```bash
   git checkout -b feature/your-feature-name
   ```

3. **Make your changes**
   - Follow the existing code style
   - Test your changes thoroughly
   - Update documentation if needed

4. **Commit your changes**
   ```bash
   git add .
   git commit -m "Description of your changes"
   ```

5. **Push and create a pull request**
   ```bash
   git push origin feature/your-feature-name
   ```

## Development Guidelines

### QML Code Style

- Use 4 spaces for indentation
- Use descriptive property and function names
- Add comments for complex logic
- Keep components modular and reusable

Example:
```qml
FullScreenEffect {
    id: effectRoot
    
    // Configuration properties
    property int blurRadius: 5
    
    // Effect implementation
    FullScreenShaderEffect {
        // ...
    }
}
```

### GLSL Shader Style

- Use descriptive variable names
- Add comments explaining algorithms
- Optimize for performance
- Ensure compatibility with GLSL 1.40

Example:
```glsl
// Calculate blur with normalized weights
vec3 calculateBlur(vec2 texCoord, float radius) {
    // Implementation
}
```

### Configuration Schema

- Use meaningful entry names
- Provide sensible defaults
- Include min/max bounds for numeric values
- Add descriptions in comments

### Documentation

- Update README.md for user-facing changes
- Update TECHNICAL.md for implementation details
- Add code comments for complex logic
- Keep examples up to date

## Testing

Before submitting a pull request:

1. **Install and test the effect**
   ```bash
   ./install.sh
   # Test in System Settings
   ```

2. **Verify shader compilation**
   - Check KWin logs for errors
   - Test with different configurations

3. **Test on different environments** (if possible)
   - X11 and Wayland
   - Different KDE Plasma versions
   - Different GPUs (Intel, AMD, NVIDIA)

4. **Performance testing**
   - Monitor frame rates during use
   - Test with different blur radii
   - Check CPU and GPU usage

## Areas for Contribution

### High Priority

- [ ] Testing on different KDE Plasma 6 versions
- [ ] Performance optimization (separable blur)
- [ ] Better error handling and logging
- [ ] User feedback and bug fixes

### Medium Priority

- [ ] Additional blur algorithms
- [ ] Per-monitor configuration
- [ ] Time-based scheduling
- [ ] Intensity control

### Low Priority

- [ ] Packaging for distributions
- [ ] Integration tests
- [ ] Benchmark suite
- [ ] Additional language translations

## Research Contributions

If you have expertise in:
- **Ophthalmology/Optometry**: Validate the scientific implementation
- **Vision Science**: Suggest improvements to the effect parameters
- **Clinical Studies**: Share feedback from real-world usage

Please reach out through issues or discussions!

## Code Review Process

1. Maintainers will review your pull request
2. Address any feedback or requested changes
3. Once approved, changes will be merged
4. Your contribution will be acknowledged in release notes

## Questions?

- Open an issue for questions
- Check existing documentation (README.md, TECHNICAL.md)
- Review the research paper for scientific background

## License

By contributing, you agree that your contributions will be licensed under the GPL-3.0 license.

## Code of Conduct

- Be respectful and inclusive
- Provide constructive feedback
- Focus on the project's goals
- Help create a welcoming community

Thank you for contributing to WayMCA!
