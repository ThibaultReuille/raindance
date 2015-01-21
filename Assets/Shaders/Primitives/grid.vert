#version 330

layout(location = 0) in vec2 a_Position;

uniform mat4 u_ModelViewProjection;
uniform vec4 u_Color;
uniform vec4 u_BackgroundColor;
uniform vec2 u_Step;
uniform vec2 u_Division;
uniform vec2 u_Shift;

out vec2 v_Texcoord;

out vec2 v_Step;
out vec2 v_Division;
out vec4 v_Color;
out vec4 v_BackgroundColor;

void main(void)
{
	v_Step = u_Step;
	v_Division = u_Division;

    v_Color = u_Color;
    v_BackgroundColor = u_BackgroundColor;
    v_Texcoord = a_Position + u_Shift;

    gl_Position = u_ModelViewProjection * vec4(a_Position, 0.0, 1.0);
}

