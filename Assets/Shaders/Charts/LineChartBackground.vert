#version 330

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_Texcoord;

uniform mat4 u_ModelViewProjectionMatrix;
uniform vec4 u_Color;

out vec4 v_Color;

void main(void)
{
    v_Color = u_Color + 0.0 * vec4(a_Texcoord, 1.0, 1.0) + 0.0 * vec4(a_Normal, 1.0);
    gl_Position = u_ModelViewProjectionMatrix * vec4(a_Position, 1.0);
}