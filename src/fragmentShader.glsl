#version 330 core
out vec4 FragColor;

in vec3 nColor;
in vec2 TexCoord;

uniform float alpha = 1.0f;
uniform sampler2D coolTexture;

void main() {
  FragColor = vec4(nColor, 1.0);
} 
