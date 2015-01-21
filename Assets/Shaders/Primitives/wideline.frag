#version 330

#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D u_Texture;

in float v_Mode;
in vec4 v_Color;
in vec2 v_Texcoord;

out vec4 FragColor;

#define M_PI 3.1415926535897932384626433832795

void main(void)
{
    if (v_Mode < 0.5)
    {
        FragColor = v_Color;
    }
    else if (v_Mode < 1.5)
    {
        FragColor = v_Color * texture(u_Texture, v_Texcoord);
    }
    else
    {
    	float f = max(sin(M_PI * v_Texcoord.y), 0.0);
    	FragColor = v_Color * vec4(f, f, f, f);
    }
}
