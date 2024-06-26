#version 330 core

uniform bool frame;

out vec4 fragColor;
  
in vec2 outTexCoord;
in vec3 outLightColor;

uniform sampler2D tex0;
uniform sampler2D screenTexture;
uniform bool lightEnabled;

void main() {

    if(!frame) {
        if(lightEnabled) {
            fragColor = texture(tex0, outTexCoord) * vec4(outLightColor, 1.0);
        } else {
            fragColor = texture(tex0, outTexCoord);
        }
    } else {
        vec3 col = vec3(texture(screenTexture, outTexCoord)).rgb;
        fragColor = vec4(col, 1.0);
    }


}
