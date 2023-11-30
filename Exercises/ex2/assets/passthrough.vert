#version 430
layout( location = 0 ) in vec2 iPosition;
layout( location = 1 ) in vec3 iColor;
out vec3 v2fColor;

void main()
{
    // Copy input color to the output color attribute.
    v2fColor = iColor;

    // Copy position to the built-in gl Position attribute
    // This attribute is the clip space position (homogeneous vertex position), which is always a vec4. For
    // now, we set z and w to zero and one, respectively. (z = zero is in the middle of the OpenGL clip space.
    // w = one, since this is a point and not a direction.)
    gl_Position = vec4( iPosition.xy, 0.0, 1.0 );
}
