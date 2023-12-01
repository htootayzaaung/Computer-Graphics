#version 430

layout(location = 0) in vec3 position; // Vertex position (x, y, z)
layout(location = 1) in vec3 color;    // Vertex color (r, g, b)

layout(location = 0) uniform mat4 uProjCameraWorld; // Add this line

out vec3 fragColor; // Pass color to fragment shader

void main() {
    fragColor = color; // Pass color to fragment shader
    gl_Position = uProjCameraWorld * vec4(position, 1.0); // Transform vertex position
}
