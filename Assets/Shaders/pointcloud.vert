attribute vec3 a_Position;
attribute vec3 a_Color;

struct Box
{
    vec3 max;
    vec3 min;
};

struct Filter
{
    int active;
    float value;
    float threshold;
};

uniform mat4 u_ModelViewProjection;
uniform Box u_Box; 
uniform Filter u_FilterX;
uniform Filter u_FilterY;
uniform Filter u_FilterZ;

varying vec4 v_Color;
varying vec3 v_Alpha;

void main(void)
{
    vec3 diff = u_Box.max - u_Box.min;    
    vec3 pos;
    pos.x = (a_Position.x - u_Box.min.x) / diff.x;
    pos.y = (a_Position.y - u_Box.min.y) / diff.y;
    pos.z = (a_Position.z - u_Box.min.z) / diff.z;

    if (u_FilterX.active > 0)
        v_Alpha.x = max(0.0, 1.0 - abs(pos.x - u_FilterX.value));
    if (u_FilterY.active > 0)
        v_Alpha.y = max(0.0, 1.0 - abs(pos.y - u_FilterY.value));
    if (u_FilterZ.active > 0)
        v_Alpha.z = max(0.0, 1.0 - abs(pos.z - u_FilterZ.value));

    v_Color = vec4(a_Color, 1.0); 

    gl_Position = u_ModelViewProjection * vec4(a_Position, 1.0);
}
