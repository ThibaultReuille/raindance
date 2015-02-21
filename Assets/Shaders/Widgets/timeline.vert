#version 330

layout(location = 0) in vec2 a_Position;

void main(void)
{
    gl_Position = vec4(a_Position, 0.0, 1.0);
}

