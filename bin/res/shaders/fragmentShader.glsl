#version 330 core

out vec4 fragColor;
  
in vec2 outTexCoord;
in vec3 outColor;
in vec3 outLightColor;

uniform sampler2D tex0;
uniform bool lightEnabled;

uniform bool frame = false;

vec3 greyscale(vec3 color, float str) {
    float g = dot(color, vec3(0.299, 0.587, 0.114));
    return mix(color, vec3(g), str);
}

void main() {

    if(frame) {
        fragColor = texture(tex0, outTexCoord);
        return;
    }

    if(lightEnabled) {
        fragColor = texture(tex0, outTexCoord) * vec4(outColor, 1.0) * vec4(outLightColor, 1.0);
    } else {
        fragColor = texture(tex0, outTexCoord) * vec4(outColor, 1.0);
    }

}
