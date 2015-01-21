#version 330

#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D u_Font;

in vec2 v_Texcoord;
in vec4 v_Color;

out vec4 FragColor;

void main(void)
{
    vec4 c = texture(u_Font, v_Texcoord);

    if (c.a < 0.2)
    {
        discard;
    }
    else
    {
        FragColor = c.a * v_Color;
    }
}
