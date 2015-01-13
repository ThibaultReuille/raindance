#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D u_Texture;

varying float v_Mode;
varying vec4 v_Color;
varying vec2 v_Texcoord;

#define M_PI 3.1415926535897932384626433832795

void main(void)
{
    if (v_Mode < 0.5)
    {
        gl_FragColor = v_Color;
    }
    else if (v_Mode < 1.5)
    {
        gl_FragColor = v_Color * texture2D(u_Texture, v_Texcoord);
    }
    else
    {
    	float filter = max(sin(M_PI * v_Texcoord.y), 0.0);
    	gl_FragColor = v_Color * vec4(filter, filter, filter, filter);
    }
}
