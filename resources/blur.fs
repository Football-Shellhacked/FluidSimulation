#version 330

in vec2 fragTexCoord;              // Fragment texture coordinate (input)
out vec4 fragColor;                // Fragment color (output)

uniform sampler2D texture0;        // Input texture
uniform float blurStrength;        // Blur strength (amount of blur)

const float offset = 1.0 / 300.0;  // Offset for sampling, adjust for different blur scales

void main()
{
    vec4 result = vec4(0.0);

    // Calculate texture coordinate offsets based on blur strength
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // Top-left
        vec2( 0.0f,    offset), // Top-center
        vec2( offset,  offset), // Top-right
        vec2(-offset,  0.0f),   // Center-left
        vec2( 0.0f,    0.0f),   // Center-center
        vec2( offset,  0.0f),   // Center-right
        vec2(-offset, -offset), // Bottom-left
        vec2( 0.0f,   -offset), // Bottom-center
        vec2( offset, -offset)  // Bottom-right
    );

    // Define weights for bilinear blur (Gaussian-like distribution)
    float weights[9] = float[](
        1.0, 2.0, 1.0,
        2.0, 4.0, 2.0,
        1.0, 2.0, 1.0
    );

    // Normalize weights to ensure the sum is 1
    float weightSum = 0.0;
    for (int i = 0; i < 9; i++) {
        weightSum += weights[i];
    }

    // Sum weighted texture samples
    for (int i = 0; i < 9; i++) {
        result += texture(texture0, fragTexCoord + offsets[i] * blurStrength) * weights[i];
    }

    // Divide by the sum of weights to normalize the result
    fragColor = result / weightSum;
}
