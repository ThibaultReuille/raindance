#version 330

#ifdef GL_ES
precision mediump float;
#endif

in vec2 v_Texcoord;

in vec2 v_Step;
in vec2 v_Division;

in vec4 v_Color;
in vec4 v_BackgroundColor;

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
	float threshold = 0.70;

	vec2 d;

	d = computeModuloDistance(v_Texcoord, v_Step);

	if (d.x <= threshold || d.y <= threshold)
	{
	    FragColor = v_Color;
	}
	else
	{
		vec2 divStep = vec2(v_Step.x / v_Division.x, v_Step.y / v_Division.y);

		d = computeModuloDistance(v_Texcoord, divStep);

		if (d.x <= threshold || d.y <= threshold)
		{
			FragColor = 0.7 * v_Color;
		}
		else
		{
			FragColor = v_BackgroundColor;
		}
	}
}
