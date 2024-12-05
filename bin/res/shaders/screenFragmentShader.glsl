#version 330 core

out vec4 fragColor;

in vec2 outTexCoord;

uniform sampler2D tex0;

int colorLevels = 2;

void main() {

    vec4 color = texture(tex0, texCoords);

    vec3 quantColor = floor(color.rgb * colorLevels) / colorLevels;
    
    fragColor = vec4(quantColor, color.a);
}
