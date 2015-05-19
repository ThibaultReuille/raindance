#version 330

#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D u_Texture;
uniform vec4 u_Color;

in vec2 gs_UV;

out vec4 FragColor;

void main(void)
{
    FragColor = u_Color * texture(u_Texture, gs_UV);
}
