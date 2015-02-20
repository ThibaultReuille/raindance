#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 8) out;

uniform mat4 u_ModelViewMatrix;
uniform mat4 u_ProjectionMatrix;
uniform vec4 u_Color;
uniform float u_Value;

out vec4 gs_Color;

void main()
{	
	vec2 pos[4] = vec2[]
	(
		vec2(0.0,  0.0),
        vec2(1.0,  0.0),
        vec2(0.0, -1.0),
        vec2(1.0, -1.0)
    );

	for (int i = 0; i < 4; i++)
	{
		gl_Position = u_ProjectionMatrix * u_ModelViewMatrix * vec4(pos[i], 0.0, 1.0);
		gs_Color = u_Color;
		EmitVertex();
	}
  	EndPrimitive();
	
	for (int i = 0; i < 4; i++)
	{
		gl_Position = u_ProjectionMatrix * u_ModelViewMatrix * vec4(pos[i].x * u_Value, pos[i].y, 0.0, 1.0);
		gs_Color = u_Color;
		EmitVertex();
	}
  	EndPrimitive();
}
