#version 330

#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_Step;
uniform vec2 u_Division;

uniform vec4 u_Color;

in vec2 gs_UV;

out vec4 FragColor;

vec2 computeModuloDistance(vec2 position, vec2 step)
{
	vec2 d;

	d.x = mod(position.x + step.x / 2.0, step.x);
	d.y = mod(position.y + step.y / 2.0, step.y);

	d.x = abs(d.x - step.x / 2.0);
	d.y = abs(d.y - step.y / 2.0);

	return d;
}

void main(void)
{
	float threshold = 1.0;

	vec2 d;

	d = computeModuloDistance(gs_UV, u_Step);

	if (d.x <= threshold || d.y <= threshold)
	{
	    FragColor = u_Color;
	}
	else
	{
		vec2 divStep = vec2(u_Step.x / u_Division.x, u_Step.y / u_Division.y);

		d = computeModuloDistance(gs_UV, divStep);

		if (d.x <= threshold || d.y <= threshold)
		{
			FragColor = 0.7 * u_Color;
		}
		else
		{
			discard;
		}
	}
}
