#version 330 core

out vec4 fragColor;
  
in vec2 outTexCoord;
in vec3 outLightColor;

uniform sampler2D tex0;
uniform bool lightEnabled;

void main() {

    if(lightEnabled) {
        fragColor = texture(tex0, outTexCoord) * vec4(outLightColor, 1.0);
    } else {
        fragColor = texture(tex0, outTexCoord);
    }

}
