#version 460

in vec3 Colour;
layout (location = 0) out vec4 FragColor;

void main() {
    FragColor = vec4(Colour, 5.0);
}
