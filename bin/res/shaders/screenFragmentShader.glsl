#version 330 core

out vec4 fragColor;

in vec2 outTexCoord;

uniform sampler2D tex0;

void main() {
    fragColor = texture(tex0, outTexCoord);
}
