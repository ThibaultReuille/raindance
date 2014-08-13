#pragma once

#include <raindance/Core/Headers.hh>

class Light
{
public:
	enum Type
	{
		POSITIONAL = 0,
		DIRECTIONAL = 1
	};

	Light()
	{
		m_Type = POSITIONAL;
		m_Position = glm::vec3(0.0, 0.0, 0.0);
		m_Color = glm::vec3(1.0, 1.0, 1.0);
		m_Direction = glm::vec3(0.0, 1.0, 0.0);
	}

	~Light()
	{
	}

	inline void setType(Type type) { m_Type = type; }
	inline Type getType() const { return m_Type; }

	inline void setPosition(const glm::vec3& position) { m_Position = position; }
	inline const glm::vec3& getPosition() const { return m_Position; }

	inline void setDirection(const glm::vec3& direction) { m_Direction = direction; }
	inline const glm::vec3& getDirection() const { return m_Direction; }

	inline void setColor(const glm::vec3& color) { m_Color = color; }
	inline const glm::vec3& getColor() const { return m_Color; }

private:
	Type m_Type;
	glm::vec3 m_Position;
	glm::vec3 m_Direction;
	glm::vec3 m_Color;
};

