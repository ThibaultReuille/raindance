
#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D u_Texture;

varying vec4 v_Color;
varying vec2 v_Texcoord;

void main(void)
{
    gl_FragColor = texture2D(u_Texture, v_Texcoord);
    gl_FragColor.r *= v_Color.r;
    gl_FragColor.g *= v_Color.g;
    gl_FragColor.b *= v_Color.b;
    gl_FragColor.a *= v_Color.a;
}

