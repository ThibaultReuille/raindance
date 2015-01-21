#version 330

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_Texcoord;

uniform mat4 u_ModelViewProjection;
uniform vec4 u_Color;

out vec4 v_Color;
out vec2 v_Texcoord;

void main(void)
{
    v_Color = u_Color;
    v_Texcoord = a_Texcoord;
    gl_Position = u_ModelViewProjection * vec4(a_Position, 1.0);
}

