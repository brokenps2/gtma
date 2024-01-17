#version 330 core
out vec4 FragColor;

in vec3 nColor;

uniform float alpha = 1.0f;

void main() {
  FragColor = vec4(nColor, alpha);
} 
