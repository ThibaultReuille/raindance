#version 330

#ifdef GL_ES
precision mediump float;
#endif

uniform int u_Textured;
uniform sampler2D u_Texture;
uniform vec4 u_Color;

in vec2 gs_UV;

out vec4 FragColor;

void main(void)
{
	if (u_Textured > 0)
   		FragColor = u_Color * texture(u_Texture, gs_UV);
   	else
   		FragColor = u_Color;
}
