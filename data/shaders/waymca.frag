#version 140

// Input from vertex shader
varying vec2 texcoord0;

// Uniforms
uniform sampler2D sampler;
uniform float greenBlurRadius;
uniform float blueBlurRadius;
uniform float useGaussianBlur;
uniform float fullScreenBlur;
uniform float fullScreenBlurRadius;

// Gaussian blur weights for a 9-tap kernel (normalized, sum = 1.0)
// Using a standard 3x3 Gaussian kernel with sigma = 1.0
const float gaussianWeights[9] = float[](
    0.0751136, 0.1238414, 0.0751136,
    0.1238414, 0.2041800, 0.1238414,
    0.0751136, 0.1238414, 0.0751136
);

// Box blur (simple average)
vec3 boxBlur(vec2 texCoord, float radius) {
    vec3 result = vec3(0.0);
    float count = 0.0;
    
    if (radius < 0.5) {
        return texture2D(sampler, texCoord).rgb;
    }
    
    // Get texture size manually for GLSL 140 compatibility
    vec2 pixelSize = vec2(1.0 / 1920.0, 1.0 / 1080.0); // Approximate screen size
    float radiusInt = floor(radius);
    
    for (float x = -radiusInt; x <= radiusInt; x += 1.0) {
        for (float y = -radiusInt; y <= radiusInt; y += 1.0) {
            vec2 offset = vec2(x, y) * pixelSize;
            vec3 sample = texture2D(sampler, texCoord + offset).rgb;
            result += sample;
            count += 1.0;
        }
    }
    
    return result / count;
}

// Gaussian blur (optimized 9-tap for better quality)
vec3 gaussianBlur(vec2 texCoord, float radius) {
    vec3 result = vec3(0.0);
    
    if (radius < 0.5) {
        return texture2D(sampler, texCoord).rgb;
    }
    
    // Get texture size manually for GLSL 140 compatibility
    vec2 pixelSize = vec2(1.0 / 1920.0, 1.0 / 1080.0); // Approximate screen size
    float radiusScale = radius / 3.0; // Scale the kernel based on radius
    
    int idx = 0;
    for (float x = -1.0; x <= 1.0; x += 1.0) {
        for (float y = -1.0; y <= 1.0; y += 1.0) {
            vec2 offset = vec2(x, y) * pixelSize * radiusScale;
            vec3 sample = texture2D(sampler, texCoord + offset).rgb;
            result += sample * gaussianWeights[idx];
            idx++;
        }
    }
    
    return result;
}

void main() {
    vec2 texCoord = texcoord0;
    
    // Get the original pixel
    vec4 originalColor = texture2D(sampler, texCoord);
    
    // Debug mode: Full-screen blur
    if (fullScreenBlur > 0.5) {
        vec3 blurred;
        if (useGaussianBlur > 0.5) {
            blurred = gaussianBlur(texCoord, fullScreenBlurRadius);
        } else {
            blurred = boxBlur(texCoord, fullScreenBlurRadius);
        }
        gl_FragColor = vec4(blurred, originalColor.a);
        return;
    }
    
    // Normal mode: Per-channel blur
    // Red channel stays sharp (no blur)
    float red = originalColor.r;
    
    // Green channel with moderate blur
    // Note: We blur the entire image and extract the green channel because
    // the blur algorithm needs to sample from all RGB channels to produce
    // accurate results. This is necessary for proper chromatic aberration simulation.
    vec3 greenBlurred;
    if (useGaussianBlur > 0.5) {
        greenBlurred = gaussianBlur(texCoord, greenBlurRadius);
    } else {
        greenBlurred = boxBlur(texCoord, greenBlurRadius);
    }
    float green = greenBlurred.g;
    
    // Blue channel with more blur
    // Separate blur operation needed because blue requires a different radius
    // than green (typically 8px vs 3px to simulate myopic chromatic aberration)
    vec3 blueBlurred;
    if (useGaussianBlur > 0.5) {
        blueBlurred = gaussianBlur(texCoord, blueBlurRadius);
    } else {
        blueBlurred = boxBlur(texCoord, blueBlurRadius);
    }
    float blue = blueBlurred.b;
    
    // Combine the channels: sharp red, blurred green, strongly blurred blue
    gl_FragColor = vec4(red, green, blue, originalColor.a);
}
