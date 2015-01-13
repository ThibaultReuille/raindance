attribute vec3 a_Position;

uniform mat4 u_ModelViewProjection;
uniform int u_Mode;

varying vec4 v_Color;

void main(void)
{
    if (u_Mode == 0) // Pane
    {
        v_Color = vec4(0.0, 0.0, 0.5, 0.5);
    }
    else if (u_Mode == 1) // Border
    {
        v_Color = vec4(0.54, 0.85, 1.0, 0.5);
    }
            
    gl_Position = u_ModelViewProjection * vec4(a_Position, 1.0);
}

