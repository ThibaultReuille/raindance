#version 330

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_Texcoord;

uniform mat4 u_ModelViewProjection;
uniform vec4 u_Color;

out vec2 v_Texcoord;
out vec4 v_Color;

void main(void)
{
    v_Color = u_Color;
    v_Texcoord = a_Texcoord;
    gl_Position = u_ModelViewProjection * vec4(a_Position, 0.0, 1.0);
}

