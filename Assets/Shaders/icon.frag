#version 330

#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D u_Texture;

in vec4 v_Color;
in vec2 v_Texcoord;

out vec4 FragColor;

void main(void)
{
    FragColor = v_Color * texture(u_Texture, v_Texcoord).rgba;
}

