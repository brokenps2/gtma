#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTex;

out vec3 ourColor;
out vec2 texCoord;

uniform mat4 camMatrix;
uniform mat4 transMatrix;

vec4 snap(vec4 vertex, vec2 resolution) {

    vec4 snappedPos = vertex;
    snappedPos.xyz = vertex.xyz / vertex.w; // convert to normalised device coordinates (NDC)
    snappedPos.xy = floor(resolution * snappedPos.xy) / resolution; // snap the vertex to the lower-resolution grid
    snappedPos.xyz *= vertex.w; // convert back to projection-space
    return snappedPos;

}

void main(){
    gl_Position = camMatrix * transMatrix * vec4(aPos, 1.0);
    gl_Position = snap(gl_Position, vec2(256,192));
    ourColor = aColor;
    texCoord = aTex;
}
