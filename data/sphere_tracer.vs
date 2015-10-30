#version 110

uniform vec2 resolution;
varying vec4 q;

void main()
{
    gl_Position = gl_Vertex;
    q = vec4(gl_Vertex.xy, gl_MultiTexCoord0.xy);
}
