#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D u_Texture;

varying float v_Mode;
varying vec4 v_Color;
varying vec2 v_Texcoord;

void main(void)
{
    if (v_Mode < 0.5)
    {
        gl_FragColor = v_Color;
    }
    else
    {
        gl_FragColor = v_Color * texture2D(u_Texture, v_Texcoord);
    }
}
