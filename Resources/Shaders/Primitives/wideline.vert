attribute vec3 a_Position;
attribute vec4 a_Color;
attribute int a_Number;

uniform mat4 u_ModelViewProjection;
uniform vec3 u_StartPosition;
uniform vec3 u_EndPosition;
uniform vec4 u_Tint;
uniform float u_Width;

varying vec4 v_Color;

void main(void)
{
    v_Color = a_Color * u_Tint;

    vec4 start = u_ModelViewProjection * vec4(u_StartPosition, 1.0);
    vec4 end = u_ModelViewProjection * vec4(u_EndPosition, 1.0);

    vec4 point = u_ModelViewProjection * vec4(a_Position, 1.0);

    vec2 slope = normalize(end.xy - start.xy);
    slope = vec(slope.y, -slope.x);

    if (a_Number == 0 || a_Number == 2)
    {
        gl_Position = vec4(point.xy + slope.xy * start.w, start.zw); 
    }
    else
    {
        gl_Position = vec4(point.xy + slope.xy * end.w, end.zw);         
    }
}
