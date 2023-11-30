#version 430
layout( location = 0 ) in vec2 iPosition;
layout( location = 1 ) in vec3 iColor;
layout(location = 1) uniform mat2 uRotation; // New uniform for the rotation matrix

out vec3 v2fColor;

void main()
{
    vec2 rotatedPosition = uRotation * iPosition;
    gl_Position = vec4(rotatedPosition, 0.0, 1.0);
    
    // Copy input color to the output color attribute.
    v2fColor = iColor;
    //gl_Position = vec4( iPosition.xy, 0.0, 1.0 );
}
