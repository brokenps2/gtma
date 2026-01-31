#version 300 es

out mediump vec4 fragColor;
  
in mediump vec2 outTexCoord;
in mediump vec3 outColor;
in mediump vec3 outLightColor;
in mediump float visibility;

uniform sampler2D tex0;
uniform bool lightEnabled;
uniform bool frame;
uniform bool ditherEnabled;
uniform mediump vec3 clearColor;
uniform mediump vec2 screenRes;
uniform mediump vec2 frameRes;
uniform bool ui;
uniform mediump float fboBrightness;
uniform mediump vec3 meshColor;

mediump vec3 greyscale(vec4 color) {
    mediump vec3 tcol = vec3(color.r, color.g, color.b);
    mediump float g = dot(tcol, vec3(0.299, 0.587, 0.114));
    return mix(tcol, vec3(g), color.a);
}

const int ditherMatrix8x8[64] = int[](0,  32, 8,  40, 2,  34, 10, 42,
                                     48, 16, 56, 24, 50, 18, 58, 26,
                                     12, 44, 4,  36, 14, 46, 6,  38,
                                     60, 28, 52, 20, 62, 30, 54, 22,
                                     3,  35, 11, 43, 1,  33, 9,  41,
                                     51, 19, 59, 27, 49, 17, 57, 25,
                                     15, 47, 7,  39, 13, 45, 5,  37,
                                     63, 31, 55, 23, 61, 29, 53, 21);

mediump float indexValue() {
    int x = int(mod(gl_FragCoord.x, 8.0));
    int y = int(mod(gl_FragCoord.y, 8.0));
    return float(ditherMatrix8x8[(x + y * 8)]) / 64.0;
}

mediump float ditherChannel(float color, int numLevels) {
    mediump float levelStep = 1.0 / float(numLevels - 1);
    mediump float d = indexValue();

    mediump float quantizedColor = floor(color / levelStep) * levelStep;
    mediump float nextQuantizedColor = clamp(quantizedColor + levelStep, 0.0, 1.0);

    mediump float distance = color - quantizedColor;
    return (distance < d * levelStep) ? quantizedColor : nextQuantizedColor;
}

mediump vec4 dither(vec4 color, int numLevels) {
    return vec4(
        ditherChannel(color.r, numLevels),
        ditherChannel(color.g, numLevels),
        ditherChannel(color.b, numLevels),
        color.a
    );
}

void main() {

    if(frame) {
        fragColor = texture(tex0, outTexCoord) * vec4(fboBrightness);
        return;
    }

    if(ui) {
        fragColor = texture(tex0, outTexCoord);
        return;
    }

    mediump vec4 texColor = texture(tex0, outTexCoord);
    if(texColor.a < 0.1)
        discard;

    mediump vec4 skyColor = vec4(clearColor.x/256.0, clearColor.y/256.0, clearColor.z/256.0, 1.0);

    if(lightEnabled) {
        fragColor = mix(skyColor, texture(tex0, outTexCoord) * vec4(outColor, 1.0) * vec4(outLightColor, 1.0), visibility) * vec4(meshColor, 1.0);
    } else {
        fragColor = texture(tex0, outTexCoord) * vec4(outColor, 1.0) * vec4(meshColor, 1.0);
        fragColor = mix(skyColor, fragColor, 1.0);
    }

    if(ditherEnabled) {
        fragColor = dither(fragColor, 14);
    }

}
