attribute vec3 a_Position;
attribute vec2 a_Texcoord;

uniform mat4 u_ModelViewProjection;
uniform vec4 u_Color;

varying vec4 v_Color;
varying vec2 v_Texcoord;

void main(void)
{
    v_Color = u_Color;
    v_Texcoord = a_Texcoord;
    gl_Position = u_ModelViewProjection * vec4(a_Position, 1.0);
}

