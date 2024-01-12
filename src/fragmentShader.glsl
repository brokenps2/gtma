#version 330 core
out vec4 FragColor;

in vec3 nColor;

void main() {
  FragColor = vec4(nColor, 1.0);
} 
