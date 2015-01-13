#ifdef GL_ES
precision mediump float;
#endif

struct Filter
{
    int active;
    float value;
    float threshold;
};

varying vec4 v_Color;
varying vec3 v_Alpha;

uniform Filter u_FilterX;
uniform Filter u_FilterY;
uniform Filter u_FilterZ;

void main(void)
{
    if (u_FilterX.active > 0 && v_Alpha.x < u_FilterX.threshold)
        discard;
    if (u_FilterY.active > 0 && v_Alpha.y < u_FilterY.threshold)
        discard;
    if (u_FilterZ.active > 0 && v_Alpha.z < u_FilterZ.threshold)
        discard;

    gl_FragColor = v_Color;
}
