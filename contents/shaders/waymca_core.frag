#version 140
#define KWIN
#define PLASMA6

#if defined(PLASMA6)
#include "colormanagement.glsl"
#endif

uniform sampler2D sampler;
uniform int textureWidth;
uniform int textureHeight;

in vec2 texcoord0;
out vec4 fragColor;

vec2 uSize     = vec2(textureWidth, textureHeight);
vec2 iTexCoord = vec2(texcoord0.x, 1.0 - texcoord0.y);

uniform float uEnabled;
uniform float uRadius;
uniform float uStrength;

vec4 getInputColor(vec2 coords) {
  vec4 color = texture2D(sampler, vec2(coords.x, 1.0 - coords.y));
  if (color.a > 0.0) {
    color.rgb /= color.a;
  }
  return color;
}

void setOutputColor(vec4 outColor) {
  vec4 premul = vec4(outColor.rgb * outColor.a, outColor.a);

#if defined(PLASMA6)
  premul = sourceEncodingToNitsInDestinationColorspace(premul);
  premul = nitsToDestinationEncoding(premul);
#endif

  fragColor = premul;
}

float kernelWeight(int d) {
  int ad = d < 0 ? -d : d;
  if (ad == 0) return 6.0;
  if (ad == 1) return 4.0;
  if (ad == 2) return 1.0;
  return 0.0;
}

void main() {
  vec4 orig = getInputColor(iTexCoord);

  if (uEnabled < 0.5 || uStrength <= 0.0) {
    setOutputColor(orig);
    return;
  }

  vec2 texel = vec2(1.0) / uSize;
  float r = max(uRadius, 0.0);

  vec3 accum = vec3(0.0);
  float accumA = 0.0;

  for (int y = -2; y <= 2; y++) {
    float wy = kernelWeight(y);
    for (int x = -2; x <= 2; x++) {
      float wx = kernelWeight(x);
      float w = wx * wy;
      vec4 s = getInputColor(iTexCoord + vec2(float(x), float(y)) * texel * r);
      accum += s.rgb * s.a * w;
      accumA += s.a * w;
    }
  }

  vec3 blurred = (accumA > 0.0001) ? (accum / accumA) : orig.rgb;

  vec4 outColor = orig;
  float t = clamp(uStrength, 0.0, 1.0);
  outColor.g = mix(orig.g, blurred.g, t);
  outColor.b = mix(orig.b, blurred.b, t);

  setOutputColor(outColor);
}
