#version 430

in vec3 fragColor; // Color from vertex shader

out vec4 outputColor; // Final color output

void main() {
    outputColor = vec4(fragColor, 1.0); // Set fragment color
}