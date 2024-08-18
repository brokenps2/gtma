#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 color;

struct PointLight {    
    vec3 position;
    vec3 color;
    bool active;
    bool sunMode;
};

uniform PointLight pointLights[32]; //remember this incase I somehow need more
uniform int actualLightCount = 0;

out vec2 outTexCoord;
out vec3 outColor;
out vec3 outLightColor;

uniform bool lightEnabled;

uniform bool frame = false;

uniform vec3 viewPos;

uniform mat4 camMatrix;
uniform mat4 transMatrix;

vec3 calcPointLight(PointLight light) {

    vec3 lPos = vec3(transMatrix * vec4(position, 1.0));
    vec3 lNormal = mat3(transpose(inverse(transMatrix))) * normal;

    vec3 ambient = vec3(0.05);

    vec3 norm = normalize(lNormal);
    vec3 lightDir = normalize(light.position - lPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.color;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - lPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * light.color;

    float constant = 1.0f;
    float linear = 0.022f;
    float quadratic = 0.0019f;

    if(!light.sunMode) {
        float distance    = length(light.position - position);
        float attenuation = 1.0 / (constant + (linear * light.color[0]) * distance + 
  		    (quadratic * light.color[0]) * (distance * distance));
        diffuse  *= attenuation;
        specular *= attenuation;
    }
    
    vec3 lightColor = ambient + diffuse;// + specular;
    
    if(!light.active) {
        return ambient;
    } else {
        return lightColor;
    }
}

vec4 snap(vec4 vertex, vec2 resolution) {

    vec4 snappedPos = vertex;
    snappedPos.xyz = vertex.xyz / vertex.w;
    snappedPos.xy = floor(resolution * snappedPos.xy) / resolution;
    snappedPos.xyz *= vertex.w;
    return snappedPos;

}

void main() {

    if(frame) {
        gl_Position = vec4(position, 1.0);
        outTexCoord = texCoord;
        return;
    }

    vec3 totalLight = vec3(0.0);
    int activeLights = 0;

    if (lightEnabled) {
        for (int i = 0; i < actualLightCount; i++) {
            if (pointLights[i].active) {
                totalLight += calcPointLight(pointLights[i]);
                activeLights++;
            }
        }
        if (activeLights > 0) {
            totalLight /= float(activeLights); // Normalize by the number of active lights
        }
    } else {
        totalLight = vec3(1.0); // Default color when lighting is disabled
    }

    outLightColor += totalLight;

    gl_Position = camMatrix * transMatrix * vec4(position, 1.0);
    gl_Position = snap(gl_Position, vec2(320, 240));
    outColor = color;
    outTexCoord = texCoord;

}
