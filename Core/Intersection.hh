#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Geometry.hh>
#include <raindance/Core/Primitives/Ray.hh>

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

	static bool PointBox(const glm::vec3& p, const glm::vec3& min, const glm::vec3& max)
	{
        if (p.x < min.x || p.y < min.y || p.z < min.z)
            return false;
        if (p.x > max.x || p.y > max.y || p.z > max.z)
            return false;

        return true;
	}

	static bool SegmentBox(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& min, const glm::vec3& max)
	{
	    const float epsilon = std::numeric_limits<float>::epsilon();

	    glm::vec3 d = (p2 - p1) * 0.5f;
	    glm::vec3 e = (max - min) * 0.5f;
	    glm::vec3 c = p1 + d - (min + max) * 0.5f;
	    glm::vec3 ad = glm::vec3(fabs(d.x), fabs(d.y), fabs(d.z));

	    if (fabs(c[0]) > e[0] + ad[0])
	        return false;
	    if (fabs(c[1]) > e[1] + ad[1])
	        return false;
	    if (fabs(c[2]) > e[2] + ad[2])
	        return false;

	    if (fabs(d[1] * c[2] - d[2] * c[1]) > e[1] * ad[2] + e[2] * ad[1] + epsilon)
	        return false;
	    if (fabs(d[2] * c[0] - d[0] * c[2]) > e[2] * ad[0] + e[0] * ad[2] + epsilon)
	        return false;
	    if (fabs(d[0] * c[1] - d[1] * c[0]) > e[0] * ad[1] + e[1] * ad[0] + epsilon)
	        return false;

	    return true;
	}

};

