#version 330

layout(location = 0) in vec3 a_Position;

uniform mat4 u_ModelViewProjection;

out vec4 v_Color;

void main(void)
{
    v_Color = vec4(0.5, 0.5, 0.5, 1.0);
    gl_Position = u_ModelViewProjection * vec4(a_Position, 1.0);
}

