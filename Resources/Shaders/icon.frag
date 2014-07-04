
#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D u_Texture;

varying vec4 v_Color;
varying vec2 v_Texcoord;

void main(void)
{
    gl_FragColor = v_Color * texture2D(u_Texture, v_Texcoord);
}

