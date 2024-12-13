#version 330 core

out vec4 fragColor;
  
in vec2 outTexCoord;
in vec3 outColor;
in vec3 outLightColor;
in float visibility;

uniform sampler2D tex0;
uniform bool lightEnabled;

uniform bool frame = false;

uniform vec3 clearColor = vec3(0, 0, 0);

vec3 greyscale(vec3 color, float str) {
    float g = dot(color, vec3(0.299, 0.587, 0.114));
    return mix(color, vec3(g), str);
}

void main() {

    if(frame) {
        vec4 color = texture(tex0, outTexCoord);
        int colorLevels = 128;
        vec3 quantColor = floor(color.rgb * colorLevels) / colorLevels;
        fragColor = vec4(quantColor, color.a);
        return;
    }

    vec4 skyColor = vec4(clearColor/255, 1);

    if(lightEnabled) {
        fragColor = mix(skyColor, texture(tex0, outTexCoord) * vec4(outColor, 1.0) * vec4(outLightColor, 1.0), visibility);
        //fragColor = texture(tex0, outTexCoord) * vec4(outColor, 1.0) * vec4(outLightColor, 1.0);
    } else {
        fragColor = texture(tex0, outTexCoord) * vec4(outColor, 1.0);
        fragColor = mix(skyColor, fragColor, visibility);
    }

}
