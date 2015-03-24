#version 330

#ifdef GL_ES
precision mediump float;
#endif

struct Filter
{
    int Active;
    float Value;
    float Threshold;
};

in vec4 vs_Color;
in vec3 vs_Alpha;

uniform Filter u_FilterX;
uniform Filter u_FilterY;
uniform Filter u_FilterZ;

out vec4 FragColor;

void main(void)
{
    if (u_FilterX.Active > 0 && vs_Alpha.x < u_FilterX.Threshold)
        discard;
    if (u_FilterY.Active > 0 && vs_Alpha.y < u_FilterY.Threshold)
        discard;
    if (u_FilterZ.Active > 0 && vs_Alpha.z < u_FilterZ.Threshold)
        discard;

    FragColor = vs_Color;
}
