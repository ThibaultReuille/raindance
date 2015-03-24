#version 330

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Color;

struct Box
{
    vec3 max;
    vec3 min;
};

struct Filter
{
    int Active;
    float Value;
    float Threshold;
};

uniform mat4 u_ModelViewProjection;
uniform Box u_Box; 
uniform Filter u_FilterX;
uniform Filter u_FilterY;
uniform Filter u_FilterZ;

out vec4 vs_Color;
out vec3 vs_Alpha;

void main(void)
{
    vec3 diff = u_Box.max - u_Box.min;    
    vec3 pos;
    pos.x = (a_Position.x - u_Box.min.x) / diff.x;
    pos.y = (a_Position.y - u_Box.min.y) / diff.y;
    pos.z = (a_Position.z - u_Box.min.z) / diff.z;

    if (u_FilterX.Active > 0)
        vs_Alpha.x = max(0.0, 1.0 - abs(pos.x - u_FilterX.Value));
    if (u_FilterY.Active > 0)
        vs_Alpha.y = max(0.0, 1.0 - abs(pos.y - u_FilterY.Value));
    if (u_FilterZ.Active > 0)
        vs_Alpha.z = max(0.0, 1.0 - abs(pos.z - u_FilterZ.Value));

    vs_Color = vec4(a_Color, 1.0); 

    gl_Position = u_ModelViewProjection * vec4(a_Position, 1.0);
}
