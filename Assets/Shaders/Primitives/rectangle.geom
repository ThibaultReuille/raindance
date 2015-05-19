#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform vec2 u_Dimension;

uniform mat4 u_ModelViewMatrix;
uniform mat4 u_ProjectionMatrix;

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
		vec2(0.0, 1.0),
		vec2(1.0, 1.0),
		vec2(0.0, 0.0),
		vec2(1.0, 0.0)
	);

	for (int i = 0; i < 4; i++)
	{
		gl_Position = u_ProjectionMatrix * u_ModelViewMatrix * vec4(pos[i] * u_Dimension, 0.0, 1.0);
		gs_UV = uv[i];
		EmitVertex();
	}
  	EndPrimitive();
}
