#version 330

layout(location = 0) in vec3 a_Position;

uniform mat4 u_ModelViewProjection;

out vec4 vs_Color;

void main(void)
{
    vs_Color = vec4(1.0, 1.0, 1.0, 1.0);
    gl_Position = u_ModelViewProjection * vec4(a_Position, 1.0);
}

