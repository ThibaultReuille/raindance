attribute vec2 a_Position;
attribute vec4 a_Color;

uniform mat4 u_ModelViewProjection;

uniform float u_Mode;
uniform vec3 u_StartPosition;
uniform vec3 u_EndPosition;
uniform vec3 u_ExtrudeDirection;

uniform vec4 u_Tint;

varying vec4 v_Color;
varying vec2 v_Texcoord;
varying float v_Mode;

void main(void)
{
    vec3 point;
    float u;

    if (u_Mode < 0.5)
    {
        if (sign(a_Position.x) < 0.0)
            gl_Position = u_ModelViewProjection * vec4(u_StartPosition, 1.0);
        else
            gl_Position = u_ModelViewProjection * vec4(u_EndPosition, 1.0);
    }
    else
    {   
        if (sign(a_Position.x) < 0.0)
        {
            point = u_StartPosition + sign(a_Position.y) * u_ExtrudeDirection;
            u = 0.0;
        }
        else
        {
            point = u_EndPosition + sign(a_Position.y) * u_ExtrudeDirection;
            u = length(u_EndPosition - u_StartPosition) / (2.0 * length(u_ExtrudeDirection));
        }

        gl_Position = u_ModelViewProjection * vec4(point, 1.0);

        v_Texcoord = vec2(u, 0.5 * a_Position.y + 0.5);
    }

    v_Mode = u_Mode;
    v_Color = a_Color * u_Tint;
}
