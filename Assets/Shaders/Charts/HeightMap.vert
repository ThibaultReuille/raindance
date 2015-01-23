#version 330

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

uniform mat4 u_ModelViewProjectionMatrix;
uniform int u_Mode;
uniform vec4 u_LineColor;

out vec4 v_Color;

void main(void)
{
    if (u_Mode == 0)
    {
        v_Color = a_Color;
    }
    else
    {
        v_Color = u_LineColor;
    }
           
    gl_Position = u_ModelViewProjectionMatrix * vec4(a_Position, 1.0);
 }
