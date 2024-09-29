#version 330

in vec2 fragTexCoord;
out vec4 fragColor;

uniform sampler2D texture0;
uniform float blurStrength;

void main() {
    vec4 sum = vec4(0.0);
    vec2 texOffset = vec2(blurStrength) / vec2(textureSize(texture0, 0)); // Texel offset

    // Gaussian weights for a 5x1 kernel
    float weights[5] = float[](0.227027, 0.1945946, 0.1216216, 0.0540541, 0.016216);

    // Apply horizontal blur
    for (int i = -4; i <= 4; i++) {
        sum += texture(texture0, fragTexCoord + vec2(texOffset.x * float(i), 0.0)) * weights[abs(i)];
    }

    fragColor = sum;
}
