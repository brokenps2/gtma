#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform bool frame = false;

out vec2 outTexCoord;
out vec3 outLightColor;

uniform bool lightEnabled;

uniform vec3 lightPos = vec3(90, 90, 90);
uniform vec3 viewPos;
uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);

uniform mat4 camMatrix;
uniform mat4 transMatrix;

vec4 snap(vec4 vertex, vec2 resolution) {

    vec4 snappedPos = vertex;
    snappedPos.xyz = vertex.xyz / vertex.w;
    snappedPos.xy = floor(resolution * snappedPos.xy) / resolution;
    snappedPos.xyz *= vertex.w;
    return snappedPos;

}

void main() {

    if(!frame) {
        if(lightEnabled) {
            vec3 lPos = vec3(transMatrix * vec4(position, 1.0));
            vec3 lNormal = mat3(transpose(inverse(transMatrix))) * normal;

            float ambientStrength = 0.05;
            vec3 ambient = ambientStrength * lightColor;

            vec3 norm = normalize(lNormal);
            vec3 lightDir = normalize(lightPos - lPos);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * lightColor;

            float specularStrength = 0.5;
            vec3 viewDir = normalize(viewPos - lPos);
            vec3 reflectDir = reflect(-lightDir, norm);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
            vec3 specular = specularStrength * spec * lightColor;

            outLightColor = ambient + diffuse;// + specular;
        }

        gl_Position = camMatrix * transMatrix * vec4(position, 1.0);
        gl_Position = snap(gl_Position, vec2(128, 96));
        outTexCoord = texCoord;
    } else {
        outTexCoord = texCoord;
        gl_Position = vec4(position.x, position.y, 0.0, 1.0); 
    }


}
