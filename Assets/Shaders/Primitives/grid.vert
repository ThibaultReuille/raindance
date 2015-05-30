#version 330

layout(location = 0) in vec2 a_Zero;

void main(void)
{
    gl_Position = vec4(a_Zero, 0.0, 1.0);
}

