#version 140

uniform sampler2D sampler;
uniform vec2 renderTextureSize;
uniform int greenRadius;
uniform int blueRadius;
uniform bool useGaussian;

in vec2 texcoord0;
out vec4 fragColor;

// Gaussian weights for various kernel sizes (precomputed)
const float gaussian3[7] = float[7](0.006, 0.061, 0.242, 0.382, 0.242, 0.061, 0.006);
const float gaussian5[11] = float[11](0.002, 0.011, 0.043, 0.115, 0.207, 0.244, 0.207, 0.115, 0.043, 0.011, 0.002);

// Box blur implementation
vec4 boxBlur(int radius) {
    vec4 color = vec4(0.0);
    float count = 0.0;
    vec2 pixelSize = 1.0 / renderTextureSize;
    
    for (int x = -radius; x <= radius; x++) {
        for (int y = -radius; y <= radius; y++) {
            vec2 offset = vec2(float(x), float(y)) * pixelSize;
            color += texture(sampler, texcoord0 + offset);
            count += 1.0;
        }
    }
    
    return color / count;
}

// Gaussian blur implementation (separable, horizontal + vertical)
vec4 gaussianBlur(int radius) {
    vec4 color = vec4(0.0);
    vec2 pixelSize = 1.0 / renderTextureSize;
    
    if (radius == 0) {
        return texture(sampler, texcoord0);
    }
    
    // For simplicity, use a 2-pass approximation with 1D Gaussian
    // Horizontal pass
    vec4 horizontalBlur = vec4(0.0);
    
    if (radius <= 3) {
        // Use smaller kernel for radius <= 3
        for (int x = -radius; x <= radius; x++) {
            vec2 offset = vec2(float(x), 0.0) * pixelSize;
            float weight = gaussian3[x + 3];
            horizontalBlur += texture(sampler, texcoord0 + offset) * weight;
        }
    } else {
        // Use larger kernel for radius > 3
        int r = min(radius, 5);
        for (int x = -r; x <= r; x++) {
            vec2 offset = vec2(float(x), 0.0) * pixelSize;
            float weight = gaussian5[x + 5];
            horizontalBlur += texture(sampler, texcoord0 + offset) * weight;
        }
    }
    
    // For performance, approximate separable 2D Gaussian as average of horizontal and vertical 1D passes
    // This is not a true 2-pass separable blur but provides reasonable quality with better performance
    vec4 verticalBlur = vec4(0.0);
    
    if (radius <= 3) {
        for (int y = -radius; y <= radius; y++) {
            vec2 offset = vec2(0.0, float(y)) * pixelSize;
            float weight = gaussian3[y + 3];
            verticalBlur += texture(sampler, texcoord0 + offset) * weight;
        }
    } else {
        int r = min(radius, 5);
        for (int y = -r; y <= r; y++) {
            vec2 offset = vec2(0.0, float(y)) * pixelSize;
            float weight = gaussian5[y + 5];
            verticalBlur += texture(sampler, texcoord0 + offset) * weight;
        }
    }
    
    // Average both passes
    return (horizontalBlur + verticalBlur) * 0.5;
}

void main() {
    // Red channel: keep sharp (original)
    float red = texture(sampler, texcoord0).r;
    
    // Green channel: apply moderate blur
    float green;
    if (greenRadius == 0) {
        green = texture(sampler, texcoord0).g;
    } else {
        if (useGaussian) {
            green = gaussianBlur(greenRadius).g;
        } else {
            green = boxBlur(greenRadius).g;
        }
    }
    
    // Blue channel: apply significant blur
    float blue;
    if (blueRadius == 0) {
        blue = texture(sampler, texcoord0).b;
    } else {
        if (useGaussian) {
            blue = gaussianBlur(blueRadius).b;
        } else {
            blue = boxBlur(blueRadius).b;
        }
    }
    
    // Preserve alpha
    float alpha = texture(sampler, texcoord0).a;
    
    fragColor = vec4(red, green, blue, alpha);
}
