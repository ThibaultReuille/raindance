#version 330

layout(location = 0) in vec2 a_Position;

uniform mat4 u_ModelViewProjectionMatrix;
uniform vec4 u_LineColor;

out vec4 v_Color;

void main(void)
{
    v_Color = u_LineColor;
    gl_Position = u_ModelViewProjectionMatrix * vec4(a_Position, 0.0, 1.0);
}