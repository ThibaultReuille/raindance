#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 u_ModelViewProjectionMatrix;

uniform vec2 u_Dimension;
uniform vec2 u_Shift;

out vec2 gs_UV;

void main()
{	
	vec2 pos[4] = vec2[]
	(
		gl_in[0].gl_Position.xy + vec2(0.0,  0.0),
        gl_in[0].gl_Position.xy + vec2(1.0,  0.0),
        gl_in[0].gl_Position.xy + vec2(0.0,  1.0),
        gl_in[0].gl_Position.xy + vec2(1.0,  1.0)
    );

	vec2 uv[4] = vec2[]
	(
		vec2(0.0, 0.0),
		vec2(1.0, 0.0),
		vec2(0.0, 1.0),
		vec2(1.0, 1.0)
	);

	for (int i = 0; i < 4; i++)
	{
		gl_Position = u_ModelViewProjectionMatrix * vec4(pos[i] * u_Dimension, 0.0, 1.0);
		gs_UV = uv[i] * u_Dimension + u_Shift;
		EmitVertex();
	}
  	EndPrimitive();
}
