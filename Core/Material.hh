#pragma once

#include <raindance/Core/Headers.hh>

class Material
{
public:
	Material()
	{
		m_Ambient = glm::vec3(0.0, 0.0, 0.0);
		m_Diffuse = glm::vec4(0.5, 0.5, 0.5, 1.0);
		m_Specular = glm::vec3(1.0, 1.0, 1.0);
		m_Shininess = 45.0f;
	}

	~Material()
	{
	}

	inline void setAmbient(const glm::vec3& ambient) { m_Ambient = ambient; }
	inline const glm::vec3& getAmbient() const { return m_Ambient; }

	inline void setDiffuse(const glm::vec4& diffuse) { m_Diffuse = diffuse; }
	inline const glm::vec4& getDiffuse() const { return m_Diffuse; }

	inline void setSpecular(const glm::vec3& specular) { m_Specular = specular; }
	inline const glm::vec3& getSpecular() const { return m_Specular; }

	inline void setShininess(const float shininess) { m_Shininess = shininess; }
	inline float getShininess() const { return m_Shininess; }

private:
	glm::vec3 m_Ambient;
	glm::vec4 m_Diffuse;
	glm::vec3 m_Specular;
	float m_Shininess;
};

