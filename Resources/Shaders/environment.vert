attribute vec3 a_Position;
attribute vec3 a_Normal;
attribute vec2 a_Texcoord;

uniform mat4 u_ModelViewProjection;

varying vec2 v_Texcoord;
varying vec3 v_Normal;

void main(void)
{
        v_Texcoord = a_Texcoord;
		v_Normal = a_Normal;
        gl_Position = u_ModelViewProjection * vec4(a_Position, 1.0);
}

