#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D u_Texture;
uniform float u_TextureWidth;
uniform float u_TextureHeight;
uniform mat3 u_Convolution;

varying vec2 v_Texcoord;

vec2 offset[9];

void main(void)
{
    float step_w = 1.0 / u_TextureWidth;
    float step_h = 1.0 / u_TextureHeight;

    offset[0] = vec2(-step_w, -step_h); offset[1] = vec2(0.0, -step_h); offset[2] = vec2(step_w, -step_h);
    offset[3] = vec2(-step_w, 0.0);     offset[4] = vec2(0.0, 0.0);     offset[5] = vec2(step_w, 0.0);
    offset[6] = vec2(-step_w, step_h);  offset[7] = vec2(0.0, step_h);  offset[8] = vec2(step_w, step_h);

   vec4 sum = vec4(0.0);
   
   //if (v_Texcoord.x < 0.499)
   //{
       sum += texture2D(u_Texture, v_Texcoord + offset[0]) * u_Convolution[0][0];
       sum += texture2D(u_Texture, v_Texcoord + offset[1]) * u_Convolution[1][0];
       sum += texture2D(u_Texture, v_Texcoord + offset[2]) * u_Convolution[2][0];
       sum += texture2D(u_Texture, v_Texcoord + offset[3]) * u_Convolution[0][1];
       sum += texture2D(u_Texture, v_Texcoord + offset[4]) * u_Convolution[1][1];
       sum += texture2D(u_Texture, v_Texcoord + offset[5]) * u_Convolution[2][1];
       sum += texture2D(u_Texture, v_Texcoord + offset[6]) * u_Convolution[0][2];
       sum += texture2D(u_Texture, v_Texcoord + offset[7]) * u_Convolution[1][2];
       sum += texture2D(u_Texture, v_Texcoord + offset[8]) * u_Convolution[2][2];
   /*
   }
   else if (v_Texcoord.x > 0.501)
   {
       sum = texture2D(u_Texture, v_Texcoord);
   }
   else
   {
       sum = vec4(1.0, 0.0, 0.0, 1.0);
   }
   */
   gl_FragColor = sum;
   // gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0) - sum;
}
