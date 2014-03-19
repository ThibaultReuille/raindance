#pragma once

#include "Core/Headers.hh"
#include "Core/Geometry.hh"

class Ray
{
public:
	Ray(glm::vec3 position, glm::vec3 direction)
	{
		m_Position = position;
		m_Direction = direction;
	}

	virtual ~Ray()
	{
	}

	inline const glm::vec3 position() const { return m_Position; }

	inline const glm::vec3 direction() const { return m_Direction; }

private:
	glm::vec3 m_Position;
	glm::vec3 m_Direction;
};

class RayPacket
{
public:

	RayPacket()
	{
		m_Shader = ResourceManager::getInstance().loadShader("ray", Resources_Shaders_ray_vert, sizeof(Resources_Shaders_ray_vert), Resources_Shaders_ray_frag, sizeof(Resources_Shaders_ray_frag));
		// m_Shader->dump();
	}

	virtual ~RayPacket()
	{
		ResourceManager::getInstance().unload(m_Shader);
	}

	void add(Ray ray)
	{
		if (m_Buffer.size() == 0)
		{
			m_Buffer << ray.position() << glm::vec3(1.0, 1.0, 1.0) << ray.position() + 100.0f * ray.direction() << glm::vec3(0.0, 1.0, 1.0);

			m_Buffer.describe("a_Position", 3, GL_FLOAT, 6 * sizeof(GLfloat), 0);
			m_Buffer.describe("a_Color",    3, GL_FLOAT, 6 * sizeof(GLfloat), 3 * sizeof(GLfloat));
			m_Buffer.generate(Buffer::DYNAMIC);
		}
		else
		{
			m_Buffer << ray.position() << glm::vec3(1.0, 1.0, 1.0) << ray.position() + 100.0f * ray.direction() << glm::vec3(0.0, 1.0, 1.0);

			m_Buffer.update();
			m_Buffer.describe("a_Position", 3, GL_FLOAT, 6 * sizeof(GLfloat), 0);
			m_Buffer.describe("a_Color",    3, GL_FLOAT, 6 * sizeof(GLfloat), 3 * sizeof(GLfloat));
		}
	}

	void draw(Context* context, glm::mat4 mvp)
	{
		m_Shader->use();
		m_Shader->uniform("u_ModelViewProjection").set(mvp);

		context->geometry().bind(m_Buffer, *m_Shader);
		context->geometry().drawArrays(GL_LINES, 0, m_Buffer.size() / (3 * sizeof(GLfloat)));
		context->geometry().unbind(m_Buffer);
	}

private:
	Buffer m_Buffer;
	Shader::Program* m_Shader;
};

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

