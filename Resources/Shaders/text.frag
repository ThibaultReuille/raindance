#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D u_Font;

varying vec2 v_Texcoord;
varying vec4 v_Color;

void main(void)
{
    vec4 c = texture2D(u_Font, v_Texcoord);

    if (c.a < 0.2)
    {
        discard;
    }
    else
    {
        gl_FragColor = c.a * v_Color;
    }
}
