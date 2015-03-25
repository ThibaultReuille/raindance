#version 330

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec4 a_Color;

uniform mat4 u_ModelViewProjectionMatrix;

out vec4 vs_Color;

void main(void)
{
    gl_Position = u_ModelViewProjectionMatrix * vec4(a_Position, 1.0);
    vs_Color = a_Color;
}

