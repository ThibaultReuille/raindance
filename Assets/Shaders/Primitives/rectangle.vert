#version 330

layout(location = 0) in vec3 a_Zero;

uniform vec3 u_Position;

void main(void)
{
    gl_Position = vec4(a_Zero + u_Position, 1.0);
}

