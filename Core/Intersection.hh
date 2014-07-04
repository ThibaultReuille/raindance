#pragma once

#include "Core/Headers.hh"
#include "Core/Geometry.hh"
#include "Core/Primitives/Ray.hh"

class Intersection
{
public:
	struct Hit
	{
		float distance;
	};

	static bool RaySphere(const Ray ray, const glm::vec3 center, const float radius, Hit& hit)
	{
		// Solutions for t if the ray intersects
		float t0;
		// float t1;

		glm::vec3 l = center - ray.position();
		float tca = glm::dot(l, ray.direction());
		if (tca < 0)
			return false;

		float d2 = glm::dot(l, l) - tca * tca;
		float radius2 = radius * radius;
		if (d2 > radius2)
			return false;

		float thc = sqrt(radius2 - d2);
		t0 = tca - thc;
		// t1 = tca + thc;

		// if (t0 > ray.tmax)
		//	return false;
		//else
		//	ray.tmax = t0;

		hit.distance = t0;

		return true;
	}
};

