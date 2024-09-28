#version 330

in vec2 fragTexCoord;
out vec4 fragColor;

uniform sampler2D texture0;
uniform float blurStrength;

void main() {
    vec4 sum = vec4(0.0);
    vec2 texOffset = vec2(blurStrength) / vec2(textureSize(texture0, 0)); // Calculate texel offset for blur amount
    sum += texture(texture0, fragTexCoord + texOffset * vec2(-1.0, -1.0)) * 0.0625;
    sum += texture(texture0, fragTexCoord + texOffset * vec2(-1.0,  0.0)) * 0.125;
    sum += texture(texture0, fragTexCoord + texOffset * vec2(-1.0,  1.0)) * 0.0625;
    sum += texture(texture0, fragTexCoord + texOffset * vec2( 0.0, -1.0)) * 0.125;
    sum += texture(texture0, fragTexCoord + texOffset * vec2( 0.0,  0.0)) * 0.25;
    sum += texture(texture0, fragTexCoord + texOffset * vec2( 0.0,  1.0)) * 0.125;
    sum += texture(texture0, fragTexCoord + texOffset * vec2( 1.0, -1.0)) * 0.0625;
    sum += texture(texture0, fragTexCoord + texOffset * vec2( 1.0,  0.0)) * 0.125;
    sum += texture(texture0, fragTexCoord + texOffset * vec2( 1.0,  1.0)) * 0.0625;

    fragColor = sum;
}
